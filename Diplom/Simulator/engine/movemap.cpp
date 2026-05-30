/***************************************************************************************\
*   File:                                                                               *
*       movemap.cpp                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       maps for movement and height renderer                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"
#include "movemap.h"
#include "gfx\inc\texture.h"

BOOL
CMoveMap::Init(RECT *dimensions, COLORREF baseColor)
{
    this->m_pTexture->Reference();
    this->m_width = (unsigned int)(dimensions->right - dimensions->left);
    this->m_height = (unsigned int)(dimensions->bottom - dimensions->top);

    BOOL _return = this->Actualize(baseColor);

    this->m_pTexture->Dereference();
    return _return;
}

void
CMoveMap::Free()
{
    this->m_pTexture->Dereference();
    free(this->m_pHeightBuffer);
}

DWORD
CMoveMap::GetValue(unsigned int x, unsigned int y)
{
    if (x >= m_width)
        return 0;

    if (y >= m_height)
        return 0;

    return (DWORD)*(this->m_pHeightBuffer + (y * this->m_width) + x);
}


BOOL
CMoveMap::Actualize(COLORREF baseColor)
{

    UNREFERENCED_PARAMETER(baseColor);

    COLORREF *pTexture = (COLORREF *)this->m_pTexture->Reference();

    COLORREF *pMapBuffer = (COLORREF *)malloc(this->m_width * this->m_height * sizeof(COLORREF));

    if (pMapBuffer == NULL)
    {
        this->m_pTexture->Dereference();
        return FALSE;
    }

    unsigned int textureWidth = this->m_pTexture->Width();
    unsigned int textureHeight = this->m_pTexture->Height();

    unsigned int width = this->m_width;
    unsigned int height = this->m_height;

    int d2y_i = textureHeight << 1;
    int d2y_d = height << 1;

    int d2x_i = textureWidth << 1;
    int d2x_d = width << 1;

    int yError = 0;

    COLORREF *_yScan = pTexture;

    unsigned int tY = 0;

    for (unsigned int _y = 0; _y < height; _y++)
    {

        unsigned int tX = 0;
        int xError = 0;
        COLORREF *_xScan = _yScan;

        for (unsigned int _x = 0; _x < width; _x++)
        {

            xError += d2x_i;

            while ((xError > 0) && (tX < (textureWidth - 1)))
            {
                tX++;
                _xScan++;
                xError -= d2x_d;
            }

            COLORREF  color = *_xScan & 0x00FFFFFF;

            //DWORD bRSat = GetRValue(baseColor);

            DWORD bRSat = GetRValue(color);
            DWORD bGSat = GetGValue(color);
            DWORD bBSat = GetBValue(color);

            *(pMapBuffer + (_y * width) + _x) = 0;

            if (bGSat >= 200)
            {
                *(pMapBuffer + (_y * width) + _x) = color;
            }
            if (bRSat >= 20)
            {
                *(pMapBuffer + (_y * width) + _x) = color;
            }
            if (bBSat >= 20)
            {
                *(pMapBuffer + (_y * width) + _x) = color;
            }
        }

        yError += d2y_i;

        while ((yError > 0) && (tY < (textureHeight - 1)))
        {
            tY++;
            _yScan += textureWidth;
            yError -= d2y_d;
        }

    }

    this->m_pTexture->Dereference();

    this->m_pHeightBuffer = pMapBuffer;

    return TRUE;

}

