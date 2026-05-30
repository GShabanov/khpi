/***************************************************************************************\
*   File:                                                                               *
*       drawEngine.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       draw default WINAPI mechanism                                                   *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 16-Jan-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Id:  $
#include "stdafx.h"
#include "inc\surface.h"
#include "fastmath.h"
#include <math.h>

ISurface::ISurface(unsigned int width, unsigned int height, int bits)  noexcept
   : IObject()
{

    m_width = width;
    m_height = height;
    m_bits = bits;
}

//------------------------------------------------------------------------------

BOOL
CSurfaceAPI::Init(HDC hDC)
{

    HDC hdcCompatible = CreateCompatibleDC(hDC);

    if (hdcCompatible == NULL)
        return FALSE;

    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, this->m_width, this->m_height);

    if (hBitmap == NULL)
    {
        DeleteDC(hdcCompatible);
        return FALSE;
    }

    HBITMAP previosBitmap = (HBITMAP)SelectObject(hdcCompatible, hBitmap);

    DeleteObject(previosBitmap);

    this->m_BackBufferDc = hdcCompatible;

    return TRUE;
}

void
CSurfaceAPI::Free()
{
    DeleteDC(this->m_BackBufferDc);
}

void 
CSurfaceAPI::SetPixel(Units x, Units y, COLORREF color)
{
    ::SetPixel(this->m_BackBufferDc, (int)x, (int)y, color);
}

void 
CSurfaceAPI::BlendPixel(Units x, Units y, COLORREF color, COLORREF transparency)
{
    const COLORREF oldColor = ::GetPixel((HDC)this->m_BackBufferDc, (int)x, (int)y);

    const DWORD newR = AlphaBlendValues(GetRValue(oldColor), GetRValue(transparency)) +
        AlphaBlendValues(GetRValue(color), (255 - GetRValue(transparency)));

    const DWORD newG = AlphaBlendValues(GetGValue(oldColor), GetGValue(transparency)) +
        AlphaBlendValues(GetGValue(color), (255 - GetGValue(transparency)));

    const DWORD newB = AlphaBlendValues(GetBValue(oldColor), GetBValue(transparency)) +
        AlphaBlendValues(GetBValue(color), (255 - GetBValue(transparency)));

    const COLORREF mixValue = RGB(newR, newG, newB);

    ::SetPixel(this->m_BackBufferDc, (int)x, (int)y, mixValue);
}

COLORREF
CSurfaceAPI::GetPixel(Units x, Units y)
{
    return ::GetPixel((HDC)this->m_BackBufferDc, (int)x, (int)y);
}


void
CSurfaceAPI::Cleanup(COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);

    HBRUSH previosBrush = (HBRUSH)::SelectObject(this->m_BackBufferDc, hBrush);

    PatBlt(this->m_BackBufferDc, 0, 0, m_width, m_height, PATCOPY);

    SelectObject(this->m_BackBufferDc, previosBrush);

    DeleteObject(hBrush);
}

void
CSurfaceAPI::Blt(HDC hDC, RECT *lpRect)
{
    UNREFERENCED_PARAMETER(lpRect);

    BitBlt(
        hDC, 
        0, 0,
        this->m_width, this->m_height,
        this->m_BackBufferDc,
        0, 0, SRCCOPY);
}

//------------------------------------------------------------------------------

extern "C" void __stdcall SetPixel640xNx32(PVOID buffer, unsigned int x, unsigned int y, COLORREF color);
extern "C" void __stdcall SetPixel800xNx32(PVOID buffer, unsigned int x, unsigned int y, COLORREF color);

/*void __stdcall 
SetPixelNxNx32(PVOID buffer, unsigned int x, unsigned int y, COLORREF color)
{
    *(COLORREF *)((unsigned char *)buffer + (((y * 640) + x) << 2)) = color;
}*/

BOOL
CSurfaceRAW::Init(HDC hDC)
{
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, this->m_width, this->m_height);

    if (hBitmap == NULL)
        return FALSE;

    BITMAPINFO      bmpInfo;

    memset(&bmpInfo, 0, sizeof(bmpInfo));

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    if (GetDIBits(hDC, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) == 0)
    {
        goto __delete_bitmap;
    }

    this->m_buffer = VirtualAlloc(0, bmpInfo.bmiHeader.biSizeImage, MEM_COMMIT, PAGE_READWRITE);

    if (this->m_buffer == 0)
    {
        goto __delete_bitmap;
    }


    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biHeight = -bmpInfo.bmiHeader.biHeight;

    if (GetDIBits(hDC, hBitmap, 0, this->m_height, this->m_buffer, &bmpInfo, DIB_RGB_COLORS) == 0)
    {
        goto __delete_buffer;
    }

    //this->m_pfnnSetPixel = &SetPixelNxNx32;

    /*if (this->m_width  == 640 &&
        this->m_height == 480)
    {
        this->m_pfnSetPixel = (SET_PIXEL_PFN)&CSurfaceRAW::iSetPixel640xNx32;
        this->m_pfnGetPixel = (GET_PIXEL_PFN)&CSurfaceRAW::iGetPixel640xNx32;
    }*/

    this->m_hbmp = hBitmap;

    memcpy(&this->m_bmpInfo, &bmpInfo, sizeof(BITMAPINFO));

    return TRUE;

__delete_buffer:
    VirtualFree(this->m_buffer, 0, MEM_RELEASE);

__delete_bitmap:
    DeleteObject(hBitmap);

    return FALSE;
}

void
CSurfaceRAW::Free()
{
    VirtualFree(this->m_buffer, 0, MEM_RELEASE);
    DeleteObject(this->m_hbmp);
}

#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

//#define DIBITS(color)( (((color) & 0x00FF0000) >> 16) | ((color) & 0x0000FF00) | (((color) & 0x000000FF) << 16))

void 
CSurfaceRAW::SetPixel(Units x, Units y, COLORREF color)
{
    if (x >= this->m_width)
        return;

    if (y >= this->m_height)
        return;

    *(COLORREF *)((unsigned char *)this->m_buffer + (((y * this->m_width) + x) * this->m_bits)) = color;
}

void 
CSurfaceRAW::BlendPixel(Units x, Units y, COLORREF color, COLORREF transparency)
{
    COLORREF oldColor = *(COLORREF *)((unsigned char *)this->m_buffer + (((y * this->m_width) + x) * this->m_bits));

    const DWORD newR = AlphaBlendValues(GetRValue(oldColor), GetRValue(transparency)) + 
        AlphaBlendValues(GetRValue(color), (255 - GetRValue(transparency)));

    const DWORD newG = AlphaBlendValues(GetGValue(oldColor), GetGValue(transparency)) +
        AlphaBlendValues(GetGValue(color), (255 - GetGValue(transparency)));

    const DWORD newB = AlphaBlendValues(GetBValue(oldColor), GetBValue(transparency)) +
        AlphaBlendValues(GetBValue(color), (255 - GetBValue(transparency)));

    const COLORREF mixValue = RGB(newR, newG, newB);

    *(COLORREF *)((unsigned char *)this->m_buffer + (((y * this->m_width) + x) * this->m_bits)) = mixValue;
}

COLORREF
CSurfaceRAW::GetPixel(Units x, Units y)
{
    return *(COLORREF *)((unsigned char *)this->m_buffer + (((y * this->m_width) + x) * this->m_bits));
}

void
CSurfaceRAW::SetPixels(Units x, Units y, COLORREF *colors, unsigned int count)
{
    const COLORREF *bufferStart = (const COLORREF *)((unsigned char *)
        this->m_buffer + (((y * this->m_width) + x) * this->m_bits));

    __movsd((unsigned long *)bufferStart, colors, count);
    //memcpy(bufferStart, colors, count << 2);
}

unsigned int
CSurfaceRAW::GetPixels(Units x, Units y, COLORREF *colors, unsigned int count)
{
    const COLORREF *bufferStart = (const COLORREF *)((unsigned char *)
        this->m_buffer + (((y * this->m_width) + x) * this->m_bits));

    __movsd(colors, bufferStart, count);
    //memcpy(colors, bufferStart, count << 2);

    return count;
}

void 
CSurfaceRAW::SetPixel640xNx32(Units x, Units y, COLORREF color)
{
    *(COLORREF *)((unsigned char *)this->m_buffer + (((y << 9) + (y << 7) + x) << 2)) = color;
}

void 
CSurfaceRAW::BlendPixel640xNx32(Units x, Units y, COLORREF color, COLORREF transparency)
{
    COLORREF oldColor = *(COLORREF *)((unsigned char *)this->m_buffer + (((y << 9) + (y << 7) + x) << 2));

    const DWORD newR = AlphaBlendValues(GetRValue(oldColor), GetRValue(transparency)) + 
        AlphaBlendValues(GetRValue(color), (255 - GetRValue(transparency)));

    const DWORD newG = AlphaBlendValues(GetGValue(oldColor), GetGValue(transparency)) +
        AlphaBlendValues(GetGValue(color), (255 - GetGValue(transparency)));

    const DWORD newB = AlphaBlendValues(GetBValue(oldColor), GetBValue(transparency)) +
        AlphaBlendValues(GetBValue(color), (255 - GetBValue(transparency)));

    const COLORREF mixValue = RGB(newR, newG, newB);

    *(COLORREF *)((unsigned char *)this->m_buffer + (((y << 9) + (y << 7) + x) << 2)) = mixValue;
}

COLORREF
CSurfaceRAW::GetPixel640xNx32(Units x, Units y)
{
    return *(COLORREF *)((unsigned char *)this->m_buffer + (((y << 9) + (y << 7) + x) << 2));
}


extern "C" VOID FillPathern(PVOID buffer, unsigned int count, COLORREF color);

void
CSurfaceRAW::Cleanup(COLORREF color)
{
    _mm_sfence();
    __stosd((unsigned long *)this->m_buffer, color, this->m_height * this->m_width);
    //FillPathern(this->m_buffer, this->m_height * this->m_width, color);
    /*for  (unsigned int i = 0; i < this->m_height; i++)
    {
        unsigned int y_off = i * this->m_width;
        for (unsigned int j = 0; j < this->m_width; j++)
        {
            *((COLORREF *)this->m_buffer + y_off + j) = color;
        }
    }*/

}

extern "C" VOID ConvertForDiBits(PVOID buffer, unsigned int count);

void
CSurfaceRAW::Blt(HDC hDC, RECT *lpRect)
{
    UNREFERENCED_PARAMETER(lpRect);

    const DWORD count = this->m_height * this->m_width;

    /*for (DWORD i = 0; i < count; i++)
    {
        *((ULONG *)this->m_buffer + i) = _byteswap_ulong(*((ULONG *)this->m_buffer + i)) >> 8;
    }*/
    ConvertForDiBits(this->m_buffer, count);

    this->m_bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    SetDIBitsToDevice(
        hDC,					// handle to DC
        0,					    // x-coord of destination upper-left corner
        0,					    // y-coord of destination upper-left corner 
        this->m_width,          // source rectangle width
        this->m_height,         // source rectangle height
        0,				        // x-coord of source lower-left corner
        0,					    // y-coord of source lower-left corner
        0,					    // first scan line in array
        this->m_height,         // number of scan lines
        ((DWORD *)this->m_buffer),// array of DIB bits
        &this->m_bmpInfo,		// bitmap information
        DIB_RGB_COLORS);        // RGB or palette indexes
    
}

void CSurfaceRAW::Line(POINT& from, POINT& to, COLORREF color)
{
    POINT   current = from;
    //
    // gain DX delta and normalize value
    //
    LONG  dx = (LONG)(to.x - from.x);
    LONG  noerror_inc = 1;

    if (dx < 0)
    {
        noerror_inc = -noerror_inc;
        dx = -dx;
    }

    //
    // gain DY delta and normalize value
    //
    LONG dy = (LONG)(to.y - from.y);
    LONG error_inc = 1; //this->m_lLineWidth;

    if (dy < 0)
    {
        error_inc = -error_inc;
        dy = -dy;
    }

    //
    // here we change leading variable
    //
    BOOL    bX = TRUE;

    if (dx < dy)
    {
        noerror_inc = InterlockedExchange(&error_inc, noerror_inc);
        dy = InterlockedExchange(&dx, dy);
        bX = FALSE;
    }

    LONG inc_error = dy << 2;
    LONG dec_error = -(dx << 2);
    register LONG error = 0;

    register LONG i = 0;

    do
    {
        //*buffer = color.rgb;
        //SetPixel(dc, current.x, current.y, color);

        *((unsigned long*)this->m_buffer + ((current.y * this->m_width) + current.x)) = color;

        i++;

        error += inc_error;

        if (error > 0)
        {
            error += dec_error;
            //buffer = buffer + error_inc;
            bX ? current.y += error_inc : current.x += error_inc;
        }

        //buffer = buffer + noerror_inc;
        bX ? current.x += noerror_inc : current.y += noerror_inc;

    } while (i <= dx);
}

void
CSurfaceRAW::Circle(POINT& position, Units radius, COLORREF color)
{
    double  rad = 0.0;

    long dx;
    long dy;


    while (rad < 6.28)
    {

        dx = radius * cos(rad);
        dy = radius * sin(rad);

        long  x = position.x + dx;
        long  y = position.y + dy;

        if (x >= 0 && y >= 0 &&
            x - 1 < this->m_width &&
            y - 1 < this->m_height)
        {

            *((unsigned long*)this->m_buffer + (y * this->m_width) + x) = color;
            *((unsigned long*)this->m_buffer + (y * this->m_width) + x + 1) = color;
            *((unsigned long*)this->m_buffer + (y * this->m_width) + x - 1) = color;
            *((unsigned long*)this->m_buffer + ((y + 1) * this->m_width) + x) = color;
            //*((unsigned long*)this->m_buffer + ((y - 1) * this->m_width) + x) = color;
        }

        rad += 0.005;
    }
}

//---------------------------------------------------------------------------------------------------
// DirectX surface
//---------------------------------------------------------------------------------------------------
BOOL
CSurfaceDirectX::Init(HDC hDC)
{
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, this->m_width, this->m_height);

    if (hBitmap == NULL)
        return FALSE;

    BITMAPINFO      bmpInfo;

    memset(&bmpInfo, 0, sizeof(bmpInfo));

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    if (GetDIBits(hDC, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) == 0)
    {
        goto __delete_bitmap;
    }

    this->m_buffer = VirtualAlloc(0, bmpInfo.bmiHeader.biSizeImage, MEM_COMMIT, PAGE_READWRITE);

    if (this->m_buffer == 0)
    {
        goto __delete_bitmap;
    }


    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biHeight = -bmpInfo.bmiHeader.biHeight;

    if (GetDIBits(hDC, hBitmap, 0, this->m_height, this->m_buffer, &bmpInfo, DIB_RGB_COLORS) == 0)
    {
        goto __delete_buffer;
    }

    //this->m_pfnnSetPixel = &SetPixelNxNx32;


    this->m_hbmp = hBitmap;

    memcpy(&this->m_bmpInfo, &bmpInfo, sizeof(BITMAPINFO));

    return TRUE;

__delete_buffer:
    VirtualFree(this->m_buffer, 0, MEM_RELEASE);

__delete_bitmap:
    DeleteObject(hBitmap);

    return FALSE;
}

void
CSurfaceDirectX::Free()
{
    VirtualFree(this->m_buffer, 0, MEM_RELEASE);
    DeleteObject(this->m_hbmp);
}
