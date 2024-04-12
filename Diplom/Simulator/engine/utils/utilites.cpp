/***************************************************************************************\
*   File:                                                                               *
*       utilites.cpp                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       sprite renderer                                                                 *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 4-Jan-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 63 $
#include "stdafx.h"
#include "winfont.h"
#define XMD_H
#undef FAR
#include <jpeglib.h>
#include "inc\libgif.h"
#include "png.h"
#include "pngstruct.h"
#include "pnginfo.h"

#include "windows.h"
#include <memory.h>

#include "..\fs\inc\filestream.h"


// partialy from: http://doxygen.reactos.org/d7/d3b/win32ss_2drivers_2font_2bmfd_2font_8c_source.html
//                http://doxygen.reactos.org/d8/d12/bmfd_8h_source.html
#pragma pack(push)
#pragma pack(1)
//
// Define OS/2 executable resource information structure.
//

#define FONT_DIRECTORY 0x8007
#define FONT_RESOURCE 0x8008

typedef struct _RESOURCE_TYPE_INFORMATION {
    USHORT Ident;
    USHORT Number;
    LONG Proc;
} RESOURCE_TYPE_INFORMATION, *PRESOURCE_TYPE_INFORMATION;

//
// Define OS/2 executable resource name information structure.
//

typedef struct _RESOURCE_NAME_INFORMATION {
    USHORT Offset;
    USHORT Length;
    USHORT Flags;
    USHORT Ident;
    USHORT Handle;
    USHORT Usage;
} RESOURCE_NAME_INFORMATION, *PRESOURCE_NAME_INFORMATION;


#pragma pack(pop)

unsigned char *
RtlFindNewElf(unsigned char *buffer, SIZE_T len)
/*++

   Routine:
        RtlFindNewElf()

   Description:
        Search for new ELF after DOS header

   Arguments:
        buffer   - [in] buffer with data

        len      - [in] length of a data

   Return:
        unsigned char * - pointer in buffer where is data

--*/
{

    if (len < (sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS)))
    {
        return 0;
    }

    IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER *)buffer;

    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return NULL;
    }

    if ((pDosHeader->e_lfanew + sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_FILE_HEADER) + sizeof(DWORD)) > len)
    {
        return NULL;
    }

    return (buffer + pDosHeader->e_lfanew);
}

BOOL
Load_Font(IDataStream *pStream, unsigned int preferedWidth,  FONT_INFO *fontInfo)
{
    if (pStream->Reference() == NULL)
    {
        return FALSE;
    }

    SIZE_T len = pStream->Length();
    unsigned char *fbuffer = (unsigned char *)malloc(len);

    pStream->Seek(0, IDataStream::FS_SEEK_SET);
    if (pStream->Read(fbuffer, len) < len)
    {
        pStream->Dereference();
        return FALSE;
    }

    IMAGE_OS2_HEADER *imageOs2Header = (IMAGE_OS2_HEADER *)RtlFindNewElf(fbuffer, len);

    if (imageOs2Header == NULL)
    {
        free(fbuffer);
        pStream->Dereference();
        return FALSE;
    }

    if (imageOs2Header->ne_magic != IMAGE_OS2_SIGNATURE)
    {
        free(fbuffer);
        pStream->Dereference();
        return FALSE;
    }

    //
    // Check if the resource table exists.
    //

    if ((imageOs2Header->ne_restab - imageOs2Header->ne_rsrctab) == 0) {
        free(fbuffer);
        pStream->Dereference();
        return FALSE;
    }


    //
    // Compute address of resource table and search the table for a font
    // resource.
    //

    RESOURCE_TYPE_INFORMATION *TableAddress =
        (PRESOURCE_TYPE_INFORMATION)((PUCHAR)imageOs2Header + imageOs2Header->ne_rsrctab);

    RESOURCE_TYPE_INFORMATION *TableEnd =
        (PRESOURCE_TYPE_INFORMATION)((PUCHAR)imageOs2Header + imageOs2Header->ne_restab);

    ULONG scaleFactor = *(SHORT *)TableAddress;

    TableAddress = (RESOURCE_TYPE_INFORMATION *)((USHORT *)TableAddress + 1);

    while ((TableAddress < TableEnd) &&
           (TableAddress->Ident != 0) &&
           (TableAddress->Ident != FONT_RESOURCE)) {

        TableAddress =
                (PRESOURCE_TYPE_INFORMATION)((PUCHAR)(TableAddress + 1) +
                    (TableAddress->Number * sizeof(RESOURCE_NAME_INFORMATION)));
    }

    if ((TableAddress >= TableEnd) || (TableAddress->Ident != FONT_RESOURCE)) {
        free(fbuffer);
        pStream->Dereference();
        return FALSE;
    }

    //
    // Compute address of resource name information and check if the resource
    // is within the file.
    //
    RESOURCE_NAME_INFORMATION *TableName = (PRESOURCE_NAME_INFORMATION)(TableAddress + 1);

    OEM_FONT_FILE_HEADER *fontHeaderPrevios = NULL;
    OEM_FONT_FILE_HEADER *fontHeaderCurrent = NULL;
    OEM_FONT_FILE_HEADER *fontHeader = NULL;

    for (ULONG i = 0; i < TableAddress->Number; i++)
    {

        if (len < ((TableName->Offset << scaleFactor) + sizeof(OEM_FONT_FILE_HEADER))) {
            TableName++;
            continue;
        }

        //
        // Compute the address of the font file header and check if the header
        // contains correct information.
        //
        OEM_FONT_FILE_HEADER *FontHeader = (POEM_FONT_FILE_HEADER)((PCHAR)fbuffer +
                                                (TableName->Offset << scaleFactor));
        if ((FontHeader->Version != OEM_FONT_VERSION) ||
            (FontHeader->Type != OEM_FONT_TYPE) ||
            (FontHeader->Italic != OEM_FONT_ITALIC) ||
            (FontHeader->Underline != OEM_FONT_UNDERLINE) ||
            (FontHeader->StrikeOut != OEM_FONT_STRIKEOUT) ||
            (FontHeader->CharSet != OEM_FONT_CHARACTER_SET) ||
            (FontHeader->PixelWidth > 32))
        {
            if (fontHeaderPrevios != NULL)
            {
                fontHeader = fontHeaderPrevios;
            }
            else
            {
                free(fbuffer);
                pStream->Dereference();
                return FALSE;
            }
        }

        fontHeaderCurrent = FontHeader;

        if (fontHeaderPrevios != NULL)
        {
            if (fontHeaderCurrent->PixelWidth > preferedWidth && 
                fontHeaderPrevios->PixelWidth <= preferedWidth)
            {
                fontHeader = fontHeaderPrevios;
                break;
            }
        }

        fontHeaderPrevios = fontHeaderCurrent;

        TableName++;
    }

    if (fontHeader == NULL || 
        fontHeader->FirstCharacter > fontHeader->LastCharacter)
    {
        free(fbuffer);
        pStream->Dereference();
        return FALSE;
    }

    FONT_MAP *fontMap = (FONT_MAP *)(fontHeader + 1);

    fontInfo->charCount = fontHeader->LastCharacter - fontHeader->FirstCharacter;
    fontInfo->BytesPerChar = (fontHeader->PixelHeight * ROUND_UP(fontHeader->PixelWidth, 8)) >> 3;

    fontInfo->glyphData = (unsigned char *)malloc(fontInfo->BytesPerChar * fontInfo->charCount);

    if (fontInfo->glyphData == NULL)
    {
        free(fbuffer);
        pStream->Dereference();
        return FALSE;
    }


    {
        unsigned char *dst = fontInfo->glyphData;
        SIZE_T bytesPerChar = fontInfo->BytesPerChar;

        for (ULONG i = 0, j = fontInfo->charCount; i < j; i++)
        {
            unsigned char *src = (UCHAR *)((PCHAR)fontHeader + fontMap[i].Offset);

            memcpy(dst, src, bytesPerChar);

            dst = (unsigned char *)dst + bytesPerChar;
        }
    }

    fontInfo->PixelHeight = fontHeader->PixelHeight;
    fontInfo->PixelWidth = fontHeader->PixelWidth;
    fontInfo->FirstCharacter = fontHeader->FirstCharacter;
    fontInfo->LastCharacter = fontHeader->LastCharacter;

    free(fbuffer);
    pStream->Dereference();

    return TRUE;

}

BOOL LoadJPGBuff(IDataStream *pStream, GFX_INFO *gfxInfo);
BOOL LoadBMPBuff(IDataStream *pStream, GFX_INFO *gfxInfo);
BOOL LoadPNGBuff(IDataStream *pStream, GFX_INFO *gfxInfo);

BOOL
Load_Picture(
    IDataStream *pStream,
    GFX_INFO  *gfxInfo)
{

    if (pStream->Reference() == 0)
    {
        return FALSE;
    }

    GFX_INFO   gfxInfoLocal = { 0, };

    pStream->Rewind();

    BOOL _return = LoadJPGBuff(
        pStream,
        &gfxInfoLocal);

    if (_return == TRUE)
    {
        *gfxInfo = gfxInfoLocal;
        pStream->Dereference();
        return TRUE;
    }

    pStream->Rewind();

    _return = LoadBMPBuff(
        pStream,
        &gfxInfoLocal);

    if (_return == TRUE)
    {
        *gfxInfo = gfxInfoLocal;
        pStream->Dereference();
        return TRUE;
    }

    pStream->Rewind();

    _return = LoadPNGBuff(
        pStream,
        &gfxInfoLocal);

    if (_return == TRUE)
    {
        *gfxInfo = gfxInfoLocal;
        pStream->Dereference();
        return TRUE;
    }

    pStream->Rewind();

    _return = LoadGIFBuff(
        pStream,
        &gfxInfoLocal);

    if (_return == TRUE)
    {
        *gfxInfo = gfxInfoLocal;
        pStream->Dereference();
        return TRUE;
    }

    pStream->Dereference();

    return FALSE;
}

#define PNG_BYTES_TO_CHECK 4

BOOL PNGCBAPI
Png_ReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
   png_size_t check;

   if (png_ptr == NULL)
      return FALSE;

   /* fread() returns 0 on error, so it is OK to store this in a png_size_t
    * instead of an int, which is what fread() actually returns.
    */
   check = ((IDataStream *)png_ptr->io_ptr)->Read(data, length);
   //check = fread(data, 1, length, png_voidcast(png_FILE_p, png_ptr->io_ptr));

   if (check != length)
   {
      return png_error(png_ptr, "Read Error");
   }

   return TRUE;
}

int PNGCBAPI
Png_Error(png_structp, png_const_charp)
{
    return FALSE;
}

BOOL 
LoadPNGBuff(
    IDataStream *pStream,
    GFX_INFO      *gfxInfo) 
{
    if (pStream == 0 || gfxInfo == 0 || pStream->Length() < 10)
    {
        return FALSE;
    }

    png_image image; /* The control structure used by libpng */

    /* Initialize the 'png_image' structure. */
    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    unsigned char sign[4];

    if (pStream->Read(sign, PNG_BYTES_TO_CHECK) < PNG_BYTES_TO_CHECK)
    {
        return FALSE;
    }

    pStream->Rewind();

    if (png_sig_cmp(sign, (png_size_t)0, PNG_BYTES_TO_CHECK) != 0)
    {
        return FALSE;
    }

    png_structp png_ptr;
    png_infop info_ptr;
    //unsigned int sig_read = 0;
    //png_uint_32 width, height;
    //int bit_depth, color_type, interlace_type;

    /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        return FALSE;
    }

    /* Allocate/initialize the memory for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return FALSE;
    }

    /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */

    /*if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* Free all of the memory associated with the png_ptr and info_ptr /
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a problem reading the file /
        return (ERROR);
    }*/

    /* One of the following I/O initialization methods is REQUIRED */
#ifdef streams /* PNG file I/O method 1 */
    /* Set up the input control if you are using standard C streams */
    png_init_io(png_ptr, fp);

#else no_streams /* PNG file I/O method 2 */
    /* If you are using replacement read functions, instead of calling
    * png_init_io() here you would call:
    */
    //png_set_read_fn(png_ptr, (void *)user_io_ptr, user_read_fn);
    /* where user_io_ptr is a structure you want available to the callbacks */
#endif no_streams /* Use only one I/O method! */

    png_set_read_fn(png_ptr, (void *)pStream, &Png_ReadData);

    /* If we have already read some of the signature */
    //png_set_sig_bytes(png_ptr, sig_read);

    png_set_error_fn(png_ptr, (void *)pStream, &Png_Error, &Png_Error);

    png_uint_32  width, height;
//    int bit_depth, color_type, interlace_type;

#define hilevel
#ifdef hilevel
    /*
    * If you have enough memory to read in the entire image at once,
    * and you need to specify only transforms that can be controlled
    * with one of the PNG_TRANSFORM_* bits (this presently excludes
    * quantizing, filling, setting background, and doing gamma
    * adjustment), then you can read the entire image (including
    * pixels) into the info structure with this call:
    */
    if (png_read_png(png_ptr, info_ptr, NULL /*PNG_TRANSFORM_BGR*/, NULL) != TRUE)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

#else
    /* OK, you're doing it the hard way, with the lower-level functions */

    /* The call to png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).  REQUIRED
    */
    if (png_read_info(png_ptr, info_ptr) != TRUE)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

    if (png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
        &interlace_type, NULL, NULL) != TRUE)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

    /* Set up the data transformations you want.  Note that these are all
    * optional.  Only call them if you want/need them.  Many of the
    * transformations only work on specific types of images, and many
    * are mutually exclusive.
    */

    /* Tell libpng to strip 16 bit/color files down to 8 bits/color.
    * Use accurate scaling if it's available, otherwise just chop off the
    * low byte.
    */
#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
#else
    png_set_strip_16(png_ptr);
#endif

    /* Strip alpha bytes from the input data without combining with the
    * background (not recommended).
    */
    png_set_strip_alpha(png_ptr);

    /* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
    * byte into separate bytes (useful for paletted and grayscale images).
    */
    png_set_packing(png_ptr);

    /* Change the order of packed pixels to least significant bit first
    * (not useful if you are using png_set_packing). */
    png_set_packswap(png_ptr);

    /* Expand paletted colors into true RGB triplets */
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    /* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    /* Expand paletted or RGB images with transparency to full alpha channels
    * so the data will be available as RGBA quartets.
    */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0)
        png_set_tRNS_to_alpha(png_ptr);

    /* Set the background color to draw transparent and alpha images over.
    * It is possible to set the red, green, and blue components directly
    * for paletted images instead of supplying a palette index.  Note that
    * even if the PNG file supplies a background, you are not required to
    * use it - you should use the (solid) application background if it has one.
    */

    png_color_16 my_background, *image_background;

    if (png_get_bKGD(png_ptr, info_ptr, &image_background) != 0)
        png_set_background(png_ptr, image_background,
        PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
    else
        png_set_background(png_ptr, &my_background,
        PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);

    /* Some suggestions as to how to get a screen gamma value
    *
    * Note that screen gamma is the display_exponent, which includes
    * the CRT_exponent and any correction for viewing conditions
    */
    double screen_gamma = PNG_DEFAULT_sRGB;  /* A good guess for a PC monitor
                                          in a dimly lit room */

    /* Tell libpng to handle the gamma conversion for you.  The final call
    * is a good guess for PC generated images, but it should be configurable
    * by the user at run time by the user.  It is strongly suggested that
    * your application support gamma correction.
    */

    int intent;

    if (png_get_sRGB(png_ptr, info_ptr, &intent) != 0)
        png_set_gamma(png_ptr, screen_gamma, PNG_DEFAULT_sRGB);
    else
    {
        double image_gamma;
        if (png_get_gAMA(png_ptr, info_ptr, &image_gamma) != 0)
            png_set_gamma(png_ptr, screen_gamma, image_gamma);
        else
            png_set_gamma(png_ptr, screen_gamma, 0.45455);
    }

#if 0 //def PNG_READ_QUANTIZE_SUPPORTED
    /* Quantize RGB files down to 8 bit palette or reduce palettes
    * to the number of colors available on your screen.
    */
    if ((color_type & PNG_COLOR_MASK_COLOR) != 0)
    {
        int num_palette;
        png_colorp palette;

        /* This reduces the image to the application supplied palette */
        if (/* We have our own palette */)
        {
            /* An array of colors to which the image should be quantized */
            png_color std_color_cube[MAX_SCREEN_COLORS];

            png_set_quantize(png_ptr, std_color_cube, MAX_SCREEN_COLORS,
                MAX_SCREEN_COLORS, NULL, 0);
        }
        /* This reduces the image to the palette supplied in the file */
        else if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) != 0)
        {
            png_uint_16p histogram = NULL;

            png_get_hIST(png_ptr, info_ptr, &histogram);

            png_set_quantize(png_ptr, palette, num_palette,
                max_screen_colors, histogram, 0);
        }
    }
#endif /* READ_QUANTIZE */

    /* Invert monochrome files to have 0 as white and 1 as black */
    //png_set_invert_mono(png_ptr);

    /* If you want to shift the pixel values from the range [0,255] or
    * [0,65535] to the original [0,7] or [0,31], or whatever range the
    * colors were originally in:
    */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT) != 0)
    {
        png_color_8p sig_bit_p;

        png_get_sBIT(png_ptr, info_ptr, &sig_bit_p);
        png_set_shift(png_ptr, sig_bit_p);
    }

    /* Flip the RGB pixels to BGR (or RGBA to BGRA) */
    if ((color_type & PNG_COLOR_MASK_COLOR) != 0)
        png_set_bgr(png_ptr);

    /* Swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
    png_set_swap_alpha(png_ptr);

    /* Swap bytes of 16 bit files to least significant byte first */
    png_set_swap(png_ptr);

    /* Add filler (or alpha) byte (before/after each RGB triplet) */
    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    int number_passes = 1;
#ifdef PNG_READ_INTERLACING_SUPPORTED
    /* Turn on interlace handling.  REQUIRED if you are not using
    * png_read_image().  To see how to handle interlacing passes,
    * see the png_read_row() method below:
    */
    number_passes = png_set_interlace_handling(png_ptr);
#else
    number_passes = 1;
#endif /* READ_INTERLACING */


    /* Optional call to gamma correct and add the background to the palette
    * and update info structure.  REQUIRED if you are expecting libpng to
    * update the palette for you (ie you selected such a transform above).
    */
    png_read_update_info(png_ptr, info_ptr);

    png_uint_32 height = info_ptr->height;

    /* Allocate the memory to hold the image using the fields of info_ptr. */

    /* The easiest way to read the image: */
    png_bytep row_pointers[height];

    /* Clear the pointer array */
    for (png_uint_32 row = 0; row < height; row++)
        row_pointers[row] = NULL;

    for (png_uint_32 row = 0; row < height; row++)
        row_pointers[row] = png_malloc(png_ptr, png_get_rowbytes(png_ptr,
        info_ptr));

    /* Now it's time to read the image.  One of these methods is REQUIRED */
#ifdef entire /* Read the entire image in one go */
    png_read_image(png_ptr, row_pointers);

#else no_entire /* Read the image one or more scanlines at a time */
    /* The other way to read images - deal with interlacing: */

    for (int pass = 0; pass < number_passes; pass++)
    {
#ifdef single /* Read the image a single row at a time */
        for (y = 0; y < height; y++)
        {
            png_read_rows(png_ptr, &row_pointers[y], NULL, 1);
        }

#else no_single /* Read the image several rows at a time */
        for (png_uint_32 y = 0; y < height; y += number_of_rows)
        {
#ifdef sparkle /* Read the image using the "sparkle" effect. */
            png_read_rows(png_ptr, &row_pointers[y], NULL,
                number_of_rows);
#else no_sparkle /* Read the image using the "rectangle" effect */
            png_read_rows(png_ptr, NULL, &row_pointers[y],
                number_of_rows);
#endif no_sparkle /* Use only one of these two methods */
        }

        /* If you want to display the image after every pass, do so here */
#endif no_single /* Use only one of these two methods */
    }
#endif no_entire /* Use only one of these two methods */

    /* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
    png_read_end(png_ptr, info_ptr);
#endif hilevel

    /* At this point you have read the entire image */

    height = info_ptr->height;
    width = info_ptr->width;

    unsigned char *out;
    int nSize;

    //row_stride = info_ptr->width * sizeof(DWORD); // cinfo.output_width * cinfo.output_components;

    nSize = width * height * sizeof(DWORD);
    out = reinterpret_cast<unsigned char*>(malloc(nSize+1));
    memset(out, 0, nSize+1);

    for (unsigned int y = 0; y < height - 1; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            
            COLORREF color = *(((COLORREF *)info_ptr->row_pointers[y]) + x);
            *((COLORREF *)out + y * width + x) = color;
            //COLORREF color = *(DWORD *)(pictureData + ((biHeight - _y - 1) * biWidth + _x) * (biBitCount >> 3));
            //*(bbuf + (biHeight - _y - 1) * biWidth + _x) = color;
            //png_read_rows(png_ptr, &row_pointers[y], NULL, 1);
        }
    }

    gfxInfo->pic = out;
    gfxInfo->width = (unsigned int)width;
    gfxInfo->height = (unsigned int)height;
    gfxInfo->bgColor = RGB(0, 0, 0);
    

    /* Clean up after the read, and free any memory allocated - REQUIRED */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);


    return TRUE;
}


BOOL 
LoadJPGBuff(
    IDataStream *pStream,
    GFX_INFO      *gfxInfo) 
{

    if (pStream == 0 || gfxInfo == 0 || pStream->Length() < 10)
    {
        return FALSE;
    }
    /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */

    struct jpeg_error_mgr jerr;
    /* More stuff */
    JSAMPARRAY buffer;	/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */
    unsigned char *out;
    unsigned char *bbuf;
    int nSize;

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);

    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    unsigned char *fbuffer = (unsigned char *)malloc(pStream->Length());

    if (fbuffer == 0)
    {
        return FALSE;
    }

    if (pStream->Read(fbuffer, pStream->Length()) < pStream->Length())
    {
        free(fbuffer);
        return FALSE;
    }

    jpeg_stdio_src(&cinfo, fbuffer, pStream->Length());

    /* Step 3: read file parameters with jpeg_read_header() */

    int _return = jpeg_read_header(&cinfo, TRUE);

    if (_return != JPEG_HEADER_OK)
    {
        free(fbuffer);
        jpeg_destroy_decompress(&cinfo);
        return FALSE;
    }
    /* We can ignore the return value from jpeg_read_header since
    *   (a) suspension is not possible with the stdio data source, and
    *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
    * See libjpeg.doc for more info.
    */

    /* Step 4: set parameters for decompression */
    //cinfo.do_block_smoothing = FALSE;

    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

    /* Step 5: Start decompressor */

    (void) jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */ 
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;

    nSize = cinfo.output_width*cinfo.output_height*cinfo.output_components;
    out = reinterpret_cast<unsigned char*>(malloc(nSize+1));
    memset(out, 0, nSize+1);

    gfxInfo->pic = out;
    gfxInfo->width = (unsigned int)cinfo.output_width;
    gfxInfo->height = (unsigned int)cinfo.output_height;
    gfxInfo->bgColor = RGB(0, 0, 0);

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        bbuf = ((out + (row_stride * cinfo.output_scanline)));
        buffer = &bbuf;
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    }

    // clear all the alphas to 255
    /*{
    int	i, j;
    byte	*buf;

    buf = *pic;

    j = cinfo.output_width * cinfo.output_height * 4;
    for ( i = 3 ; i < j ; i+=4 ) {
    buf[i] = 255;
    }
    }*/

    /* Step 7: Finish decompression */

    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* After finish_decompress, we can close the input file.
    * Here we postpone it until after no more JPEG errors are possible,
    * so as to simplify the setjmp error logic above.  (Actually, I don't
    * think that jpeg_destroy can do an error exit, but why assume anything...)
    */
    free (fbuffer);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    /* And we're done! */

    return TRUE;
}

#define BMP_TYPE_HEADER  'MB'
BOOL 
LoadBMPBuff(
    IDataStream *pStream,
    GFX_INFO      *gfxInfo) 
{

    if (pStream == 0 || pStream->Length() <= sizeof(BITMAPFILEHEADER) || gfxInfo == 0)
    {
        return FALSE;
    }

    BITMAPFILEHEADER   bmpFileHeader;

    if (pStream->Read(&bmpFileHeader, sizeof(BITMAPFILEHEADER)) < sizeof(BITMAPFILEHEADER))
    {
        return FALSE;
    }

    /*
    *  check the header
    */
    if (bmpFileHeader.bfType != BMP_TYPE_HEADER ||
        bmpFileHeader.bfSize > pStream->Length())
    {
        return FALSE;
    }

    union {
        BITMAPCOREHEADER  corHeader;
        BITMAPINFOHEADER  infoHeader;
        BITMAPV4HEADER    infoHeader4;
        BITMAPV5HEADER    infoHeader5;
    } infoHeader;

    if (pStream->Read(&infoHeader, sizeof(infoHeader)) == 0)
    {
        return FALSE;
    }

    if (infoHeader.corHeader.bcSize < sizeof(BITMAPCOREHEADER))
    {
        return FALSE;
    }

    DWORD   biWidth = 0;
    DWORD   biHeight = 0;
    int     biBitCount = 0;
    WORD    biPlanes = 0;

    switch (infoHeader.corHeader.bcSize)
    {
    case sizeof(BITMAPCOREHEADER):

        biWidth = infoHeader.corHeader.bcWidth;
        biHeight = infoHeader.corHeader.bcWidth;
        break;

    case sizeof(BITMAPINFOHEADER):

        if (    infoHeader.infoHeader.biBitCount != 1
            &&  infoHeader.infoHeader.biBitCount != 4
            &&  infoHeader.infoHeader.biBitCount != 8
            &&  infoHeader.infoHeader.biBitCount != 16
            &&  infoHeader.infoHeader.biBitCount != 24
            &&  infoHeader.infoHeader.biBitCount != 32)
        {
            return FALSE;
        }

        biWidth = infoHeader.infoHeader.biWidth;
        biHeight = infoHeader.infoHeader.biHeight;
        biPlanes = infoHeader.infoHeader.biPlanes;
        
        if (infoHeader.infoHeader.biCompression != BI_RGB)
            return FALSE;

        biBitCount = infoHeader.infoHeader.biBitCount;

        break;

    case sizeof(BITMAPV4HEADER):
        biWidth = infoHeader.infoHeader4.bV4Width;
        biHeight = infoHeader.infoHeader4.bV4Height;

        if (infoHeader.infoHeader4.bV4V4Compression != BI_RGB)
            return FALSE;

        return FALSE;
        break;

    case sizeof(BITMAPV5HEADER):
        biWidth = infoHeader.infoHeader5.bV5Width;
        biHeight = infoHeader.infoHeader5.bV5Height;

        if (infoHeader.infoHeader5.bV5Compression != BI_RGB)
            return FALSE;

        return FALSE;
        break;

    default:
        return FALSE;
    }

    pStream->Seek(sizeof(BITMAPFILEHEADER) + infoHeader.corHeader.bcSize, IDataStream::FS_SEEK_SET);    

    DWORD *pPictureBuffer = (DWORD *)
        malloc(biWidth * biHeight * sizeof(DWORD));

    if (pPictureBuffer == 0)
        return FALSE;

    SIZE_T rowSize = biWidth * (biBitCount >> 3);

    unsigned char *pRow = (unsigned char *)malloc(rowSize);

    if (pRow == 0)
    {
        free(pPictureBuffer);
        return FALSE;
    }


    for (DWORD  _y = 0; _y < biHeight; _y++)
    {
        if (pStream->Read(pRow, rowSize) < rowSize)
        {
            free(pRow);
            free(pPictureBuffer);
            return FALSE;
        }

        for (DWORD _x = 0; _x < biWidth; _x++)
        {
            COLORREF color = *(DWORD *)(pRow + _x * (biBitCount >> 3));
            //COLORREF color = *(DWORD *)(pictureData + ((biHeight - _y - 1) * biWidth + _x) * (biBitCount >> 3));
            *(pPictureBuffer + (biHeight - _y - 1) * biWidth + _x) = color;
        }
    }

    free(pRow);

    gfxInfo->pic = (unsigned char *)pPictureBuffer;
    gfxInfo->width = (int)biWidth;
    gfxInfo->height = (int)biHeight;
    gfxInfo->bgColor = RGB(0, 0, 0);

    return TRUE;
}
