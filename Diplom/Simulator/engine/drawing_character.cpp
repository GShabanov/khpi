/***************************************************************************************\
*   File:                                                                               *
*       ghost_character.cpp                                                             *
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

#include "character.h"
#include "drawing_character.h"
#include "scene.h"
#include "gfx\inc\surface.h"
#include "movemap.h"
#include "gfx\inc\sprite.h"
#include "gfx\inc\texture.h"
#include "environment.h"

BOOL
CDrawingCharacter::Init()
{

    IMoveMap *pHeightMap = this->m_pScene->GetHeightMap();
    IMoveMap *pMoveMap = this->m_pScene->GetMoveMap();

    ITexture *ghostTexture = this->m_pScene->GetTexture(11);

    if (ghostTexture == NULL)
        return FALSE;

    this->m_pSprite = new CSprite(ghostTexture);

    if (this->m_pSprite == NULL)
    {
        ghostTexture->Dereference();
        return FALSE;
    }

    if (this->m_pSprite->Init() == FALSE)
    {
        ghostTexture->Dereference();
        delete this->m_pSprite;
        return FALSE;
    }

    ghostTexture->Dereference();

    this->m_lCurrentBlending = 0;
    this->m_lBlendingInc = 1;
    this->m_bMouseDown = FALSE;

    LONG maxX = this->m_pScene->GetMaxX();
    LONG maxY = this->m_pScene->GetMaxY();

    this->m_X = maxX / 2;
    this->m_Y = maxY / 2;

    for (LONG y = 0; y < maxY; y++)
    {
        for (LONG x = 0; x < maxX; x++)
        {
            if (GetGValue(pMoveMap->GetValue(x, y)) != 0)
            {
                this->m_X = x;
                this->m_Y = y;
                goto __exit;
            }
        }
    }

__exit:

    this->m_dwCurrentHeight = GetRValue(pHeightMap->GetValue(this->m_X, this->m_Y)) * 5;


    return TRUE;
}

void
CDrawingCharacter::Free()
{
    this->m_pSprite->Free();
    delete this->m_pSprite;
}

void
CDrawingCharacter::Draw(ISurface *surface)
{
    RECT  tSpriteRect = {
        this->m_X - this->m_dwCurrentHeight,  // left
        this->m_Y - this->m_dwCurrentHeight,  // top
        this->m_X + this->m_dwCurrentHeight,  // right
        this->m_Y + this->m_dwCurrentHeight}; // bottom

    /*tSpriteRect.left = this->m_X - this->m_dwCurrentHeight;
    tSpriteRect.right = this->m_X + this->m_dwCurrentHeight;
    tSpriteRect.top = this->m_Y - this->m_dwCurrentHeight;
    tSpriteRect.bottom = this->m_Y + this->m_dwCurrentHeight;*/

    BYTE blendValue = (BYTE)(80 + this->m_lCurrentBlending);

    this->m_pSprite->Draw(surface, &tSpriteRect, FALSE, RGB(blendValue, blendValue, blendValue));
}

void
CDrawingCharacter::Dispatch(CSharedEnvironment *environment)
{
    this->m_lCurrentBlending += this->m_lBlendingInc;

    if (this->m_lCurrentBlending >= 50)
    {
        this->m_lBlendingInc = -1;
    }

    if (this->m_lCurrentBlending <= 0)
    {
        this->m_lBlendingInc = 1;
    }

    LONG  dx = 0;
    LONG  dy = 0;

    LONG mouseX = 0;
    LONG mouseY = 0;

    BOOL bVisible = environment->m_gfxInfo.IsMouseVisible();
    LONG maxX = environment->m_gfxInfo.GetMaxX();
    LONG maxY = environment->m_gfxInfo.GetMaxY();

    //BOOL bMouseClicked = environment->GetMouseLClick();

    if (!bVisible)
        return;

    if (this->m_X < mouseX) {
        dx = 1;
    }

    if (this->m_X > mouseX) {
        dx = -1;
    }

    if (this->m_Y < mouseY) {
        dy = 1;
    }

    if (this->m_Y > mouseY) {
        dy = -1;
    }

    if (((this->m_X + dx) >= maxX) || 
        ((this->m_X + dx) < 0))
    {
        dx = 0;
    }

    if (((this->m_Y + dy) >= maxY) || 
        ((this->m_Y + dy) < 0))
    {
        dy = 0;
    }


    IMoveMap *pMoveMap = this->m_pScene->GetMoveMap();
    /*if (bMouseClicked)
    {
        MakeTrackedPath();
    }*/
    DWORD dwValue = 0;

    dwValue = GetGValue(pMoveMap->GetValue(this->m_X + dx, this->m_Y + dy));

    if (dwValue != 0)
    {
        this->m_X += dx;
        this->m_Y += dy;
    }
    else
    {

        dwValue = GetGValue(pMoveMap->GetValue(this->m_X + dx, this->m_Y));

        if (dwValue != 0)
        {
            this->m_X += dx;
        }
        else
        {
            dwValue = GetGValue(pMoveMap->GetValue(this->m_X, this->m_Y + dy));

            if (dwValue != 0)
            {
                this->m_Y += dy;
            }
        }
    }


    IMoveMap *pHeightMap = this->m_pScene->GetHeightMap();

    /*m_dwCurrentHeight = GetRValue(pHeightMap->GetValue(this->m_X, this->m_Y)) * 10;

    if (m_dwCurrentHeight == 0)
    {
        m_dwCurrentHeight = 20;
    }
    else
    {
        m_dwCurrentHeight = (m_dwCurrentHeight >> 3) + 20;
    }*/

}
