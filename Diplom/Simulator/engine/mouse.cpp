/***************************************************************************************\
*   File:                                                                               *
*       mouse.cpp                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       character overall dispatch function                                             *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Jun-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"

#include "sfx\inc\waveFormat.h"
#include "sfx\inc\sound.h"
#include "sfx\inc\soundWorld.h"

#include "gfx\inc\font.h"
#include "mouse.h"
#include "scene.h"
#include "environment.h"
#include "gfx\inc\texture.h"
#include "gfx\inc\surface.h"
#include "movemap.h"



BOOL
CMouse::Init(ITexture *texture)
{
    if (texture->Reference() == 0)
        return FALSE;

    this->m_pTexture = texture;

    this->m_sizeX = this->m_pTexture->Width();
    this->m_sizeY = this->m_pTexture->Height();

    return TRUE;
}

VOID
CMouse::Free()
{
    this->m_pTexture->Dereference();
}

VOID
CMouse::Draw(class ISurface *surface)
{
    COLORREF *pTexture = (COLORREF *)this->m_pTexture->Reference();

    if (pTexture == 0)
    {
        return;
    }

    unsigned int textureWidth = this->m_pTexture->Width();
    //unsigned int textureHeight = this->m_pTexture->Height();

    unsigned int drawWidth = (unsigned int)this->m_sizeY;
    unsigned int drawHeight = (unsigned int)this->m_sizeY;

    LONG _yScan = 0;
    LONG _frameOffset = (this->m_lCurrentFrame * this->m_sizeY);

    for (unsigned int _y = 0; _y < drawHeight; _y++)
    {
        for (unsigned int _x = 0; _x < drawWidth; _x++)
        {
            COLORREF  color = *((COLORREF *)pTexture + _yScan + _x + _frameOffset);

            if (color != 0)
            {
                surface->SetPixel((unsigned int)this->m_X + _x * 2 + 0 - (drawWidth / 2), (unsigned int)this->m_Y + _y * 2 + 0 - (drawWidth / 2), color);
                surface->SetPixel((unsigned int)this->m_X + _x * 2 + 1 - (drawWidth / 2), (unsigned int)this->m_Y + _y * 2 + 0 - (drawWidth / 2), color);
                surface->SetPixel((unsigned int)this->m_X + _x * 2 + 0 - (drawWidth / 2), (unsigned int)this->m_Y + _y * 2 + 1 - (drawWidth / 2), color);
                surface->SetPixel((unsigned int)this->m_X + _x * 2 + 1 - (drawWidth / 2), (unsigned int)this->m_Y + _y * 2 + 1 - (drawWidth / 2), color);
            }
        }

        _yScan += textureWidth;
    }

    this->m_pTexture->Dereference();
}


VOID
CMouse::Dispatch(CSharedEnvironment *environment)
{
    //
    // set cursor position according to current coordinates
    //

    this->m_X = environment->m_gfxInfo.GetMouseX();
    this->m_Y = environment->m_gfxInfo.GetMouseY();

    IMoveMap *pActiveZonesMap = this->m_pScene->GetActiveZoneMap();

    if (GetBValue(pActiveZonesMap->GetValue(this->m_X, this->m_Y)) != 0)
    {
        this->m_bAnimate = TRUE;

        if (environment->GetMouseLClick() == TRUE)
        {
            IScene *pScene = environment->GetCurrentScene();

            if (pScene != 0)
            {
                OutputDebugString(_T("emit"));
                pScene->EmitSound(0);
            }
        }
    }
    else
    {
        this->m_bAnimate = FALSE;
        this->m_lCurrentTicks = 0;
        this->m_lFrameIncrement = 1;
        this->m_lCurrentFrame = 0;
    }

    this->m_lCurrentTicks++;

    if (this->m_lCurrentTicks >= 5)
    {
        this->m_lCurrentTicks = 0;

        if (this->m_lCurrentFrame == 0)
        {
            this->m_lFrameIncrement = 1;
        }

        this->m_lCurrentFrame += this->m_lFrameIncrement;

        if (this->m_lCurrentFrame == (this->m_sizeX / this->m_sizeY - 1))
        {
            this->m_lFrameIncrement = -1;
        }
    }
}
