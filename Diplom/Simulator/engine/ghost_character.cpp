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
#include "ghost_character.h"
#include "scene.h"
#include "gfx\inc\surface.h"
#include "movemap.h"
#include "gfx\inc\sprite.h"
#include "gfx\inc\texture.h"
#include "environment.h"

BOOL
CGhostCharacter::Init()
{
    InitializeSListHead(&this->m_TrackingHead);

    IMoveMap *pHeightMap = this->m_pScene->GetHeightMap();
    IMoveMap *pMoveMap = this->m_pScene->GetMoveMap();

    ITexture *ghostTexture = this->m_pScene->GetTexture(4);

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

    this->m_X = 0;
    this->m_Y = 0;

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

    this->m_dwCurrentHeight = GetRValue(pHeightMap->GetValue(this->m_X, this->m_Y));


    return TRUE;
}

void
CGhostCharacter::Free()
{
    this->m_pSprite->Free();
    delete this->m_pSprite;
}

void
CGhostCharacter::Draw(ISurface *surface)
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

    SLIST_ENTRY  *pCurrentEntry = this->m_TrackingHead.Next.Next;

    while (pCurrentEntry != NULL)
    {
        TRACK_ENTRY  *pTrackEntry = CONTAINING_RECORD(pCurrentEntry, TRACK_ENTRY, entry);

        surface->SetPixel(pTrackEntry->point.x, pTrackEntry->point.y, RGB(255, 0, 0));
        
        pCurrentEntry = pCurrentEntry->Next;
    }

    this->m_pSprite->Draw(surface, &tSpriteRect, FALSE, RGB(blendValue, blendValue, blendValue));
}

void
CGhostCharacter::Dispatch(CSharedEnvironment *environment)
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

    LONG mouseX = environment->m_gfxInfo.GetMouseX();
    LONG mouseY = environment->m_gfxInfo.GetMouseY();

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

    m_dwCurrentHeight = GetRValue(pHeightMap->GetValue(this->m_X, this->m_Y));

    if (m_dwCurrentHeight == 0)
    {
        m_dwCurrentHeight = 20;
    }
    else
    {
        m_dwCurrentHeight = (m_dwCurrentHeight >> 3) + 20;
    }

}

void
CGhostCharacter::MakeTrackedPath()
{

    if (this->m_TrackingHead.Next.Next == NULL)
    {
        TRACK_ENTRY  *pTrackEntry = (TRACK_ENTRY *)malloc(sizeof(TRACK_ENTRY));

        if (pTrackEntry == 0)
        {
            return;
        }

        /*this->m_pEvironment->LockShared();

        LONG mouseX = this->m_pEvironment->GetMouseX();
        LONG mouseY = this->m_pEvironment->GetMouseY();

        this->m_pEvironment->Unlock();*/

        pTrackEntry->recursion = 0;
        pTrackEntry->state = 0;
        pTrackEntry->point.x = 0;
        pTrackEntry->point.y = 0; 

        PushEntryList(&this->m_TrackingHead.Next, &pTrackEntry->entry);
        return;
    }

    TRACK_ENTRY  *pTrackEntry = CONTAINING_RECORD(this->m_TrackingHead.Next.Next, TRACK_ENTRY, entry);

    /*if (pTrackEntry->recursion > 50)
    {
        pTrackEntry->state++;
        return;
    }*/

    LONG dx = 0;
    LONG dy = 0;

    switch (pTrackEntry->state)
    {
    case 0:
        dx = -1;
        dy = -1;
        break;
    case 1:
        dx = 0;
        dy = -1;
        break;
    case 2:
        dx = 1;
        dy = -1;
        break;
    case 3:
        dy = 0;
        dx = -1;
        break;
    case 4:
        dy = 0;
        dx = 1;
        break;
    case 5:
        dy = 1;
        dx = -1;
        break;
    case 6:
        dy = 1;
        dx = 0;
        break;
    case 7:
        dy = 1;
        dx = 1;
        break;

    default:
        SLIST_ENTRY *pEntry = PopEntryList(&this->m_TrackingHead.Next);

        free(CONTAINING_RECORD(pEntry, TRACK_ENTRY, entry));
        return;
    }

    if (this->CheckVisit(pTrackEntry->point.x + dx, pTrackEntry->point.y + dy) == TRUE)
    {
        pTrackEntry->state++;
        return;
    }

    //DWORD dwValue = GetGValue(this->m_pMoveMap->GetValue(pTrackEntry->point.x + dx, pTrackEntry->point.y + dy));

    DWORD dwValue = 0;

    if (dwValue == 0)
    {
        pTrackEntry->state++;
        return;
    }

    TRACK_ENTRY *pNewTrackEntry = (TRACK_ENTRY *)malloc(sizeof(TRACK_ENTRY));

    if (pNewTrackEntry == 0)
        return;

    pNewTrackEntry->point.x = pTrackEntry->point.x + dx;
    pNewTrackEntry->point.y = pTrackEntry->point.y + dy;
    pNewTrackEntry->recursion = pTrackEntry->recursion + 1;
    pNewTrackEntry->state = 0;

    PushEntryList(&this->m_TrackingHead.Next, &pNewTrackEntry->entry);

}

BOOL
CGhostCharacter::CheckVisit(LONG x, LONG y)
{
    BOOL bVisited = FALSE;

    SLIST_ENTRY  *pCurrentEntry = this->m_TrackingHead.Next.Next;

    while (pCurrentEntry != NULL)
    {
        TRACK_ENTRY  *pTrackEntry = CONTAINING_RECORD(pCurrentEntry, TRACK_ENTRY, entry);

        if (pTrackEntry->point.x == x &&
            pTrackEntry->point.y == y)
        {
            bVisited = TRUE;
            break;
        }
        
        pCurrentEntry = pCurrentEntry->Next;
    }

    return bVisited;
}
