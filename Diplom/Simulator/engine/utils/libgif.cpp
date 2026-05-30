/***************************************************************************************\
*   File:                                                                               *
*       libgif.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       gif file parsing                                                                *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-May-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 66 $

#include "stdafx.h"
#include "inc\libgif.h"
#include "gif_lib.h"
#include "..\fs\inc\filestream.h"

int
GifInputFunc(GifFileType *gft, GifByteType *buffer, int size)
{
    if (size < 0)
        return 0;

    if (((IDataStream *)gft->UserData)->Read(buffer, size) < (SIZE_T)size)
    {
        return 0;
    }

    return size;

}


BOOL 
LoadGIFBuff(IDataStream *pStream, GFX_INFO *gfxInfo)
{
    int ExtCode;
    int Error = 0;

    GifRecordType RecordType;
    GifByteType *Extension;
    GifFileType *gifFile;

    //int ImageNum = 0;
    

    if ((gifFile = DGifOpen(pStream, (InputFunc)&GifInputFunc, &Error)) == NULL)
    {
        return FALSE;
    }

#if 0
    GifRowType *ScreenBuffer;

    /* 
    * Allocate the screen as vector of column of rows. Note this
    * screen is device independent - it's the screen defined by the
    * GIF file parameters.
    */
    if ((ScreenBuffer = (GifRowType *)malloc(gifFile->SHeight * sizeof(GifRowType))) == NULL)
    {
        goto __gif_close_file;
    }

    SIZE_T size = gifFile->SWidth * sizeof(GifPixelType);             /* Size in bytes one row.*/

    if ((ScreenBuffer[0] = (GifRowType) malloc(size)) == NULL) /* First row. */
    {
        goto __gif_free_screen_buffer;
    }

    for (GifWord i = 0; i < gifFile->SWidth; i++)  /* Set its color to BackGround. */
    {
        ScreenBuffer[0][i] = (unsigned char)gifFile->SBackGroundColor;
    }

    for (GifWord i = 1; i < gifFile->SHeight; i++) 
    {
        /* Allocate the other rows, and set their color to background too: */
        if ((ScreenBuffer[i] = (GifRowType) malloc(size)) == NULL)
        {
            goto __gif_free_screen_buffer;
        }

        memcpy(ScreenBuffer[i], ScreenBuffer[0], size);
    }
#endif
    GifPixelType *ScreenBuffer;

    /* Size in pixels of device independed sceen.*/
    SIZE_T size = gifFile->SHeight * gifFile->SWidth;

    /* 
    * Allocate the screen as polygon. Note this
    * screen is device independent - it's the screen defined by the
    * GIF file parameters.
    */
    if ((ScreenBuffer = (GifPixelType *)malloc(size * sizeof(GifPixelType))) == NULL)
    {
        goto __gif_close_file;
    }

    GifPixelType *p = ScreenBuffer;

    for (SIZE_T i = 0; i < size; i++)  /* Set its color to BackGround. */
    {
        *p++ = (unsigned char)gifFile->SBackGroundColor;
    }


    /* Scan the content of the GIF file and load the image(s) in: */
    do
    {

        if (DGifGetRecordType(gifFile, &RecordType) == GIF_ERROR)
        {
            goto __gif_free_screen_buffer;
        }

        switch (RecordType)
        {
        case IMAGE_DESC_RECORD_TYPE:
            if (DGifGetImageDesc(gifFile) == GIF_ERROR)
            {
                goto __gif_free_screen_buffer;
            }


            if (gifFile->Image.Left + gifFile->Image.Width > gifFile->SWidth ||
                gifFile->Image.Top + gifFile->Image.Height > gifFile->SHeight)
            {
                goto __gif_free_screen_buffer;
            }

            if (gifFile->Image.Interlace)
            {
                GifWord Row = gifFile->Image.Top; /* Image Position relative to Screen. */
                GifWord Col = gifFile->Image.Left;
                GifWord Width = gifFile->Image.Width;
                GifWord Height = gifFile->Image.Height;

                unsigned int InterlacedOffset[] = { 0, 4, 2, 1 };   // The way Interlaced image should.
                unsigned int InterlacedJumps[] = { 8, 8, 4, 2 };    // be read - offsets and jumps...

                p = ScreenBuffer + Col + (Row * gifFile->SWidth);

                /* Need to perform 4 passes on the images: */
                for (int i = 0; i < 4; i++)
                {
                    GifWord swidth = gifFile->SWidth;

                    GifPixelType *pr = p + InterlacedOffset[i] * swidth;

                    GifWord jump = InterlacedJumps[i];
                    GifWord delta = jump * swidth;

                    for (GifWord j = 0; j < Height; j += jump)
                    {
                        if (DGifGetLine(gifFile, pr, Width) == GIF_ERROR)
                        {
                            goto __gif_free_screen_buffer;
                        }

                        pr += delta;
                    }

                }

#if 0
                /* Need to perform 4 passes on the images: */
                for (int i = 0; i < 4; i++)
                {
                    for (unsigned int j = Row + InterlacedOffset[i]; j < Row + Height;
                        j += InterlacedJumps[i])
                    {
                        if (DGifGetLine(gifFile, &ScreenBuffer[j][Col], Width) == GIF_ERROR)
                        {
                            goto __gif_free_screen_buffer;
                        }
                    }
                }
#endif // if 0
            }
            else
            {
                GifWord Row = gifFile->Image.Top; /* Image Position relative to Screen. */
                GifWord Col = gifFile->Image.Left;
                GifWord Width = gifFile->Image.Width;
                GifWord Height = gifFile->Image.Height;

                p = ScreenBuffer + Col + (Row * gifFile->SWidth);

                for (GifWord i = 0, j = gifFile->SWidth; i < Height; i++)
                {
                    if (DGifGetLine(gifFile, p, Width) == GIF_ERROR)
                    {
                        goto __gif_free_screen_buffer;
                    }

                    p += j;
#if 0
                    if (DGifGetLine(gifFile, &ScreenBuffer[Row++][Col], Width) == GIF_ERROR)
                    {
                        goto __gif_free_screen_buffer;
                    }
#endif
                }
            }
            break;

        case EXTENSION_RECORD_TYPE:
            /* Skip any extension blocks in file: */
            if (DGifGetExtension(gifFile, &ExtCode, &Extension) == GIF_ERROR)
            {
                goto __gif_free_screen_buffer;
            }

            while (Extension != NULL)
            {
                if (DGifGetExtensionNext(gifFile, &Extension) == GIF_ERROR)
                {
                    goto __gif_free_screen_buffer;
                }
            }
            break;

        case TERMINATE_RECORD_TYPE:
            break;
        default:            /* Should be trapped by DGifGetRecordType. */
            break;
        }

    } while (RecordType != TERMINATE_RECORD_TYPE);

    /* Lets dump it - set the global variables required and do it: */
    ColorMapObject *ColorMap = (gifFile->Image.ColorMap
        ? gifFile->Image.ColorMap
        : gifFile->SColorMap);

    if (ColorMap == NULL)
    {
        goto __gif_free_screen_buffer;
    }


    gfxInfo->height = gifFile->SHeight;
    gfxInfo->width = gifFile->SWidth;

    GifColorType *ColorMapEntry = &ColorMap->Colors[gifFile->SBackGroundColor];

    gfxInfo->bgColor = gifFile->SBackGroundColor;
    //gfxInfo->bgColor = RGB(ColorMapEntry->Red, ColorMapEntry->Green, ColorMapEntry->Blue);

    SIZE_T  picDataSize = gifFile->SWidth * gifFile->SHeight * sizeof(COLORREF);
    gfxInfo->pic = (unsigned char *)malloc(picDataSize);

    if (gfxInfo->pic == 0)
    {
        goto __gif_free_screen_buffer;
    }

    memset(gfxInfo->pic, 0, picDataSize);

    p = ScreenBuffer;

    for (GifWord i = 0, height = gifFile->SHeight; i < height; i++)
    {

        for (GifWord j = 0, width = gifFile->SWidth; j < width; j++)
        {
            ColorMapEntry = ColorMap->Colors + *p++;

            *((COLORREF *)gfxInfo->pic + i * width + j) = RGB(
                ColorMapEntry->Red,
                ColorMapEntry->Green,
                ColorMapEntry->Blue);
        }
#if 0
        GifRowType GifRow = ScreenBuffer[i];

        for (GifWord j = 0, width = gifFile->SWidth; j < width; j++)
        {

            ColorMapEntry = ColorMap->Colors + GifRow[j];

            *((COLORREF *)gfxInfo->pic + i * width + j) = RGB(
                ColorMapEntry->Red,
                ColorMapEntry->Green,
                ColorMapEntry->Blue);
        }
#endif // if 0
    }


    //gifFile->SBackGroundColor

    /*DumpScreen2RGB(OutFileName, OneFileFlag,
        ColorMap,
        ScreenBuffer, 
        GifFile->SWidth, GifFile->SHeight);*/

    (void)free(ScreenBuffer);

    DGifCloseFile(gifFile);

    return TRUE;

__gif_free_screen_buffer:
    free(ScreenBuffer);

__gif_close_file:
    DGifCloseFile(gifFile);

    return FALSE;
}

typedef struct _GifHeader {
  // Header
  BYTE Signature[3];     /* Header Signature (always "GIF") */
  BYTE Version[3];       /* GIF format version("87a" or "89a") */
  // Logical Screen Descriptor
  WORD ScreenWidth;      /* Width of Display Screen in Pixels */
  WORD ScreenHeight;     /* Height of Display Screen in Pixels */
  BYTE Packed;           /* Screen and Color Map Information */
  BYTE BackgroundColor;  /* Background Color Index */
  BYTE AspectRatio;      /* Pixel Aspect Ratio */
} GIFHEAD;

// ColorTableSize = 3L * (1L << (SizeOfGlobalColorTable + 1));
typedef struct _GifColorTable {
  BYTE Red;          /* Red Color Element       */
  BYTE Green;        /* Green Color Element     */
  BYTE Blue;         /* Blue Color Element      */
} GIFCOLORTABLE;

//NumberOfGlobalColorTableEntries =
//    (1L << (SizeOfTheGlobalColorTable + 1));

// PixelAspectRatio = (AspectRatio + 15) / 64;

// Packed contains the following five subfields of data (bit 0 is the LSB):
//
// Bit 0 	Local Color Table Flag
// Bit 1 	Interlace Flag
// Bit 2 	Sort Flag
// Bits 3-4 	Reserved
// Bits 5-7 	Size of Local Color Table Entry

typedef struct _GifImageDescriptor {
  BYTE Separator;    /* Image Descriptor identifier */
  WORD Left;         /* X position of image on the display */
  WORD Top;          /* Y position of image on the display */
  WORD Width;        /* Width of the image in pixels */
  WORD Height;       /* Height of the image in pixels */
  BYTE Packed;       /* Image and Color Table Data Information */
} GIFIMGDESC;

typedef struct _GifColorTableLocal {
  BYTE Red;            /* Red Color Element */
  BYTE Green;          /* Green Color Element */
  BYTE Blue;           /* Blue Color Element */
} GIFCOLORTABLE_LOCAL;