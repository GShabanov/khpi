/***************************************************************************************\
*   File:                                                                               *
*       font.cpp                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       draw text on user output                                                        *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 2-Apr-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 54 $


// http://support.microsoft.com/kb/65123

#include "stdafx.h"
#include "inc\font.h"
#include "inc\surface.h"

#include "..\fs\inc\fileStream.h"

BOOL
CFont::Init(IDataStream *pStream)
{
    if (pStream->Reference() == 0)
    {
        return FALSE;
    }

    this->m_pFontStorage = pStream;

    this->m_lRefCount = 0;

    return TRUE;
}

void
CFont::Free()
{
    this->m_bRemove = TRUE;
    if (InterlockedDecrement(&this->m_lRefCount) <= 0)
    {
        this->m_pFontStorage->Dereference();
        free(this->m_fontInfo.glyphData);
    }
}

void *
CFont::Reference()
{
    if (InterlockedIncrement(&this->m_lRefCount) == 1)
    {

        if ((
            Load_Font(
            this->m_pFontStorage,
            this->m_PreferedWidth,
            &m_fontInfo) == FALSE) || (m_fontInfo.LastCharacter == 0))
        {
            InterlockedDecrement(&this->m_lRefCount);
            return NULL;
        }

        //this->m_pBufferRaw = fontInfo.bits;
        //this->m_Width = gfxInfo.width;
        //this->m_Height = gfxInfo.height;
    }

    return (PVOID)1;
}

BOOL
CFont::Dereference()
{
    if (InterlockedDecrement(&this->m_lRefCount) <= 0)
    {
        free(this->m_fontInfo.glyphData);

        this->m_fontInfo.glyphData = 0;
        return TRUE;
    }

    return FALSE;
}

void
CFont::WaitDereference(HANDLE hWaitObject)
{
    UNREFERENCED_PARAMETER(hWaitObject);
}

BOOL
CFont::DrawCharacter(
    ISurface *surface, 
    UCHAR alpha, 
    LONG x, LONG y, COLORREF rgb, 
    COLORREF transparency)
{
    UNREFERENCED_PARAMETER(transparency);

    if (alpha < m_fontInfo.FirstCharacter || alpha > m_fontInfo.LastCharacter ||
        m_fontInfo.PixelWidth == 0 || m_fontInfo.PixelHeight == 0)
    {
        return FALSE;
    }

    unsigned char *pCharData = ((unsigned char *)m_fontInfo.glyphData + alpha * m_fontInfo.BytesPerChar);

    SIZE_T fontX = m_fontInfo.PixelWidth;
    SIZE_T fontY = m_fontInfo.PixelHeight;

    SIZE_T scansTotal = ROUND_UP(fontX, 8) >> 3;

    SIZE_T charCurrent = 0;
    SIZE_T scansDx = 0;

    COLORREF  tempBuffer[8];

    for (SIZE_T scans = 0; scans < scansTotal; scans++)
    {

        for (ULONG my = 0; my < fontY; my++)
        {

            charCurrent = *pCharData++;

            surface->GetPixels(x + scansDx, y + my, tempBuffer, 8);

            SIZE_T iShift = 128;

            for (ULONG mx = 0; mx < 8; mx++)
            {
                if (charCurrent & iShift)
                {
                    tempBuffer[mx] = rgb;
                }

                iShift >>= 1;
            }

            surface->SetPixels(x + scansDx, y + my, tempBuffer, 8);


        }

        scansDx += 8;
    }

    return TRUE;
}

BOOL
CFont::PrintF(class ISurface *surface, LONG x, LONG y, COLORREF rgb, COLORREF transparency, TCHAR *format, ...)
{
    va_list argptr;

    va_start(argptr, format);

    TCHAR buffer[32000];
    size_t  size = sizeof(buffer) / sizeof(TCHAR);

    size_t ret = _vsntprintf(buffer, size-1, format, argptr);

    buffer[size-1] = '\0';
    if ( ret < 0 || ret >= size ) {

        va_end(argptr);
        return FALSE;
    }

    SIZE_T fontDX = this->m_fontInfo.PixelWidth;
    SIZE_T fontDY = this->m_fontInfo.PixelHeight;

    unsigned int  currentX = x;
    unsigned int  currentY = y;

    TCHAR *_char = buffer;
    while (*_char != 0)
    {
        if (*_char == _T('\n'))
        {
            currentX = x;

            currentY += fontDY;

            if (currentY > surface->Height())
            {
                break;
            }

            _char++;

            continue;
        }

        DrawCharacter(surface, *(UCHAR *)_char, currentX, currentY, rgb, transparency);

        currentX += fontDX;

        if (currentX > surface->Width())
        {
            currentX = x;

            currentY += fontDY;

            if (currentY > surface->Height())
            {
                break;
            }
        }

        _char++;
    }

    va_end(argptr);
    return TRUE;
}