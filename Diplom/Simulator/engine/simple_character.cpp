/***************************************************************************************\
*   File:                                                                               *
*       simple_charecter.cpp                                                            *
*                                                                                       *
*   Abstract:                                                                           *
*       Simple character to test overall functionality over character interconnect      *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 02-May-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"


#include "sfx\inc\waveFormat.h"
#include "sfx\inc\sound.h"
#include "sfx\inc\soundWorld.h"

#include "gfx\inc\sprite.h"
#include "gfx\inc\texture.h"

#include "gfx\inc\font.h"
#include "gfx\inc\surface.h"

#include "character.h"
#include "simple_character.h"
#include "scene.h"
#include "movemap.h"
#include "environment.h"

BOOL
CSimpleCharacter::Init()
{
    //this->m_bMouseDown = FALSE;

    LONG maxX = this->m_pScene->GetMaxX();
    LONG maxY = this->m_pScene->GetMaxY();

    unsigned int a = 0x100;
    unsigned int b = 0xFFFFFF00;
    a -= b;

    ITexture *pTexture = this->m_pScene->GetTexture(12);

    if (pTexture == NULL)
        return FALSE;

    for (int i = 0; i < ARRAYSIZE(this->m_pDotSprites); i++)
    {
        m_pDotSprites[i] = NULL;
    }

    for (int i = 0; i < ARRAYSIZE(this->m_pDotSprites); i++)
    {    
        m_pDotSprites[i] = new CSprite(pTexture);

        if (m_pDotSprites[i] == NULL)
        {
            pTexture->Dereference();
            goto __clean_sprites;
        }

        BOOL _return = m_pDotSprites[i]->Init();

        if (_return == FALSE)
        {
            delete m_pDotSprites[i];
            m_pDotSprites[i] = NULL;
            pTexture->Dereference();
            goto __clean_sprites;
        }
    }

    pTexture->Dereference();

    ITexture *pTexture3D = this->m_pScene->GetTexture(10);

    if (pTexture3D == NULL)
        goto __clean_sprites;

    m_pTextureSprite = new CSprite3D(pTexture3D);
    m_pTextureSprite2 = new CSprite(pTexture3D);

    m_pTextureSprite2->Init();

    if (m_pTextureSprite == NULL) {
        goto __dereference_texture3D;
    }

    if (m_pTextureSprite->Init() != TRUE)
    {
        delete m_pTextureSprite;
        goto __dereference_texture3D;
    }

    LONG textureX = pTexture3D->Width();
    LONG textureY = pTexture3D->Height();

    pTexture3D->Dereference();


    this->m_Objects[0] = Coord3D(maxX / 2 - textureX / 2, maxY / 3 - textureY / 2, 20); // left up
    this->m_Objects[1] = Coord3D(maxX / 2 + textureX / 2 - 1, maxY / 3 - textureY / 2, 20); // right up
    this->m_Objects[2] = Coord3D(maxX / 2 - textureX / 2, maxY / 3 + textureY / 2 - 1, 20); // left down
    this->m_Objects[3] = Coord3D(maxX / 2 + textureX / 2 - 1, maxY / 3 + textureY / 2 - 1, 20); // rigth down
    this->m_Objects[4] = Coord3D(maxX / 2 + textureX / 2 - 1, maxY / 3 + textureY / 2 - 1 + 40, 20); // rigth down

    return TRUE;

__dereference_texture3D:

    pTexture3D->Dereference();

__clean_sprites:

    for (int i = 0; i < ARRAYSIZE(this->m_pDotSprites); i++)
    {
        if (m_pDotSprites[i] != NULL)
        {
            m_pDotSprites[i]->Free();
            delete m_pDotSprites[i];
        }
    }

    m_pTextureSprite->Free();
    delete m_pTextureSprite;

    return FALSE;
}

void
CSimpleCharacter::Free()
{
    for (int i = 0; i < ARRAYSIZE(this->m_Objects); i++)
    {    
        m_pDotSprites[i]->Free();
        delete m_pDotSprites[i];
    }

    m_pTextureSprite->Free();
    delete m_pTextureSprite;

    m_pTextureSprite2->Free();
    delete m_pTextureSprite2;
}

void
CSimpleCharacter::Draw(ISurface *surface)
{
    /*ITexture *texture = m_pTextureSprite->GetTextureReference();

    if (texture != NULL)
    {

        //this->m_Objects[0] = Coord3D(maxX / 2 - 50, maxY / 3 - 50, 40); // left up
        //this->m_Objects[1] = Coord3D(maxX / 2 + 50, maxY / 3 - 50, 40); // right up
        //this->m_Objects[2] = Coord3D(maxX / 2 - 50, maxY / 3 + 50, 40); // left down
        //this->m_Objects[3] = Coord3D(maxX / 2 + 50, maxY / 3 + 50, 40); // rigth down

        LONG textureX = texture->Width() - 1;
        LONG textureY = texture->Height() - 1;
        VERTEX  vertexes[3];

        vertexes[0].coord = m_Objects[0].getCoords();
        vertexes[0].u = 0; vertexes[0].v = 0;

        vertexes[1].coord = m_Objects[1].getCoords();
        vertexes[1].u = (float)textureX; vertexes[1].v = 0;

        vertexes[2].coord = m_Objects[3].getCoords();
        vertexes[2].u = (float)textureX; vertexes[2].v = (float)textureY;
        vertexes[2].coord.x--;  // BUG BUG BUG

        m_pTextureSprite->Draw(surface, vertexes, TRUE);

        vertexes[0].coord = m_Objects[0].getCoords();
        vertexes[0].u = 0; vertexes[0].v = 0;
        vertexes[1].coord = m_Objects[2].getCoords();
        vertexes[1].u = 0; vertexes[1].v = (float)textureY;
        vertexes[1].coord.x++;

        vertexes[2].coord = m_Objects[3].getCoords();
        vertexes[2].u = (float)textureX; vertexes[2].v = (float)textureY;

        m_pTextureSprite->Draw(surface, vertexes, TRUE);

        texture->Dereference();
    }*/

    LONG xsize = this->m_Objects[3].m_coord.x - this->m_Objects[0].m_coord.x;

    /*RECT  tSpriteRect1 = { 
        this->m_Objects[0].m_coord.x + xsize + 20,
        this->m_Objects[0].m_coord.y,
        this->m_Objects[0].m_coord.x + 2 * xsize + 20,
        this->m_Objects[3].m_coord.y };

    m_pTextureSprite2->Draw(surface, &tSpriteRect1, TRUE);*/

    IFont *drawFont = m_pScene->GetFont(0);

    for (int i = 0; i < ARRAYSIZE(this->m_Objects); i++)
    {
        RECT  tSpriteRect = {
            this->m_Objects[i].m_coord.x,  // left
            this->m_Objects[i].m_coord.y,  // top
            this->m_Objects[i].m_coord.x + 100,  // right
            this->m_Objects[i].m_coord.y + 100 }; // bottom

        m_pDotSprites[i]->Draw(surface, &tSpriteRect, FALSE, RGB(0, 5, 5));

        POINT  circle;

        circle.x = this->m_Objects[i].m_coord.x + 50;
        circle.y = this->m_Objects[i].m_coord.y + 50;

        surface->Circle(circle, 200, RGB(0x0, 0, 0xFF));

        drawFont->PrintF(surface, this->m_Objects[i].m_coord.x + 6, this->m_Objects[i].m_coord.y + 6, RGB(255,0,0), 0, _T("%d"), i);
    }


    //
    // make links
    //

    if (ARRAYSIZE(this->m_Objects) > 0)
    {
        POINT  from;

        from.x = this->m_Objects[0].m_coord.x + 50;
        from.y = this->m_Objects[0].m_coord.y + 50;
            

        for (int i = 1; i < ARRAYSIZE(this->m_Objects); i++)
        {

            POINT  to;

            to.x = this->m_Objects[i].m_coord.x + 50;
            to.y = this->m_Objects[i].m_coord.y + 50;


            surface->Line(from, to, RGB(0xFF, 0, 0));

            from = to;

        }

    }


    drawFont->Dereference();

}

void
CSimpleCharacter::Dispatch(CSharedEnvironment *environment)
{
    UNREFERENCED_PARAMETER(environment);
    LONG mouseX = environment->m_gfxInfo.GetMouseX();
    LONG mouseY = environment->m_gfxInfo.GetMouseY();

    BOOL bVisible = environment->m_gfxInfo.IsMouseVisible();

    //LONG maxX = environment->m_gfxInfo.GetMaxX();
    //LONG maxY = environment->m_gfxInfo.GetMaxY();

    BOOL bMouseDown = environment->GetMouseLDown();

    if (!bVisible)
    {
        return;
    }

    if (bMouseDown)
    {
        for (int i = 0; i < ARRAYSIZE(this->m_Objects); i++)
        {
            if (this->m_Objects[i].m_bCaptured == TRUE)
            {

                this->m_Objects[i].m_coord.x += (mouseX - this->m_Objects[i].m_capturedAt.x);

                if (this->m_Objects[i].m_coord.x + 110 > environment->m_gfxInfo.GetMaxX())
                {
                    this->m_Objects[i].m_coord.x = environment->m_gfxInfo.GetMaxX() - 110;
                }

                this->m_Objects[i].m_coord.y += (mouseY - this->m_Objects[i].m_capturedAt.y);

                if (this->m_Objects[i].m_coord.y + 150 > environment->m_gfxInfo.GetMaxY())
                {
                    this->m_Objects[i].m_coord.y = environment->m_gfxInfo.GetMaxY() - 150;
                }

                this->m_Objects[i].m_capturedAt.x = mouseX;
                this->m_Objects[i].m_capturedAt.y = mouseY;

                return;
            }
        }

        for (int i = 0; i < ARRAYSIZE(this->m_Objects); i++)
        {
            if (this->m_Objects[i].m_bCaptured == FALSE)
            {
                if ((mouseX >= this->m_Objects[i].m_coord.x) &&
                    (mouseY >= this->m_Objects[i].m_coord.y) &&
                    (mouseX < (this->m_Objects[i].m_coord.x + 100)) &&
                    (mouseY < (this->m_Objects[i].m_coord.y + 100)))
                {
                    this->m_Objects[i].m_bCaptured = TRUE;
                    this->m_Objects[i].m_capturedAt.x = mouseX;
                    this->m_Objects[i].m_capturedAt.y = mouseY;
                    break;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < ARRAYSIZE(this->m_Objects); i++)
        {
            this->m_Objects[i].m_bCaptured = FALSE;
            this->m_Objects[i].m_capturedAt.x = 0;
            this->m_Objects[i].m_capturedAt.y = 0;
        }
    }



}
