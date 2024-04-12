/***************************************************************************************\
*   File:                                                                               *
*       logger.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       logger implementation                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 25-Apr-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"
#include <math.h>


#include "logger.h"
#include "environment.h"
#include "gfx\inc\surface.h"
#include "mouse.h"
#include "gfx\inc\font.h"
#include "gfx\inc\texture.h"
#include "gfx\inc\sprite.h"

#include "fs\inc\fileSystem.h"
#include "fs\inc\fileStream.h"

static Vertex  vertex_const[8];

BOOL
CWindowLogger::Init()
{
    vertex_const[0].x = -100; vertex_const[0].y =  100; vertex_const[0].z =  100; vertex_const[0].u = 0;   vertex_const[0].v = 0;
    vertex_const[1].x =  100; vertex_const[1].y =  100; vertex_const[1].z =  100; vertex_const[1].u = 255; vertex_const[1].v = 0;
    vertex_const[2].x =  100; vertex_const[2].y = -100; vertex_const[2].z =  100; vertex_const[2].u = 255; vertex_const[2].v = 255;
    vertex_const[3].x = -100; vertex_const[3].y = -100; vertex_const[3].z =  100; vertex_const[3].u = 0;   vertex_const[3].v = 255;
    vertex_const[4].x =  100; vertex_const[4].y =  100; vertex_const[4].z = -100; vertex_const[4].u = 255; vertex_const[4].v = 0;
    vertex_const[5].x =  100; vertex_const[5].y = -100; vertex_const[5].z = -100; vertex_const[5].u = 0;   vertex_const[5].v = 255;
    vertex_const[6].x = -100; vertex_const[6].y = -100; vertex_const[6].z = -100; vertex_const[6].u = 0;   vertex_const[6].v = 0;
    vertex_const[7].x = -100; vertex_const[7].y =  100; vertex_const[7].z = -100; vertex_const[7].u = 255; vertex_const[7].v = 255;

    this->m_texture = reinterpret_cast<UCHAR *>(
        LocalAlloc(LMEM_FIXED, 256 * 256));

    if (this->m_texture == NULL)
        return FALSE;

    this->makeTexture(&this->m_texture);
	this->m_bChangeAngle = TRUE;

    this->m_pFileSystem = new CFileSystem();

    if (this->m_pFileSystem == NULL)
    {
        return FALSE;
    }

    if (this->m_pFileSystem->Init() != TRUE)
    {
        goto __destroy_fs;
    }

    BOOL _return = this->InitializeFonts();

    if (_return == FALSE)
    {
        goto __finalyze_fs;
    }

    _return = this->InitializeTextures();

    if (_return == FALSE)
    {
        goto __FREE_FONTS;
    }

    _return = this->InitializeSprites();

    if (_return == FALSE)
    {
        goto __FREE_TEXTURES;
    }

    return TRUE;

__FREE_TEXTURES:
    this->ShutdownTextures();

__FREE_FONTS:
    this->ShutdownFonts();

__finalyze_fs:
    this->m_pFileSystem->Free();

__destroy_fs:
    delete[] (UCHAR *)this->m_pFileSystem;

    return FALSE;
}

void
CWindowLogger::Free()
{

    this->ShutdownFonts();

    this->ShutdownSprites();
    this->ShutdownTextures();

    this->m_pFileSystem->Free();
    delete[] (UCHAR *) this->m_pFileSystem;

	LocalFree(this->m_texture);

}

BOOL
CWindowLogger::InitializeFonts()
{
    BOOL _return = FALSE;

    IDataStream *pDataStream = this->m_pFileSystem->OpenFileRead(_T("data\\fonts\\app866.fon"), 0, TRUE);

    if (pDataStream == NULL)
    {
        return FALSE;
    }

    IFont *pFont = new CFont(8);

    if (pFont == NULL)
    {
        pDataStream->Free();
        delete pDataStream;
        return FALSE;
    }

    _return = pFont->Init(pDataStream);

    if (_return == FALSE)
    {
        pDataStream->Free();
        delete pDataStream;
        delete pFont;
        return FALSE;
    }

    pFont->Reference();

    m_Font = pFont;

    return TRUE;
}

VOID
CWindowLogger::ShutdownFonts()
{
    m_Font->Dereference();
    m_Font->Free();
    delete m_Font;
}

const TCHAR  *g_LogTextures[] = {
    _T("data\\textures\\pngtest.png"),
    _T("data\\textures\\gifgrid.gif")
};

BOOL
CWindowLogger::InitializeTextures()
{
    BOOL _return = FALSE;

    for (unsigned int i = 0; i < ARRAY_SIZE(g_LogTextures); i++)
    {
        IDataStream *pDataStream = this->m_pFileSystem->OpenFileRead(g_LogTextures[i], 0, TRUE);

        if (pDataStream == NULL)
            return FALSE;

        ITexture *pTexture = new CTexture();

        if (pTexture == NULL)
        {
            pDataStream->Free();
            delete pDataStream;
            return FALSE;
        }

        _return = pTexture->Init(pDataStream);

        if (_return == FALSE)
        {
            pDataStream->Free();
            delete pDataStream;
            delete pTexture;
            return FALSE;
        }

        pTexture->Reference();

        m_TextureArray[i] = pTexture;
    }

    return TRUE;
}

VOID
CWindowLogger::ShutdownTextures()
{
    for (unsigned int i = 0; i < ARRAY_SIZE(g_LogTextures); i++)
    {
        m_TextureArray[i]->Free();
        delete m_TextureArray[i];
    }
}


const int g_LogSpriteAssigments[] = {
    0, /* PNG picture */
    1, /* GIF picture */
};

BOOL
CWindowLogger::InitializeSprites()
{
    for (int i = 0; i < ARRAY_SIZE(g_LogSpriteAssigments); i++)
    {
        ISprite *pSprite = new CSprite(m_TextureArray[g_LogSpriteAssigments[i]]);

        if (pSprite == NULL)
        {
            return FALSE;
        }

        BOOL _return = pSprite->Init();

        if (_return == FALSE)
        {
            delete pSprite;
            return FALSE;
        }

        this->m_SpritesArray[i] = pSprite;
    }

    return TRUE;
}

VOID
CWindowLogger::ShutdownSprites()
{
    for (int i = 0; i < ARRAY_SIZE(g_LogSpriteAssigments); i++)
    {
        this->m_SpritesArray[i]->Free();
        delete this->m_SpritesArray[i];
    }
}


void
CWindowLogger::Draw(class ISurface *surface)
{
    surface->Cleanup(RGB(10, 10, 10));

    if (m_Font->Reference() != NULL)
    {

        m_Font->PrintF(surface, 1, 1, RGB(0,255,0), 0, _T("Angle: %0.3f rad"), this->m_pSharedEnvironment->m_gfxInfo.m_fi);
        m_Font->PrintF(surface, 1, m_Font->Height() + 1, RGB(255,0,0), 0, _T("Frame speed: %0.8d mcs"), this->m_pSharedEnvironment->m_gfxInfo.m_DrawPerfomance.LowPart);
        m_Font->PrintF(surface, 1, m_Font->Height() * 2 + 1, RGB(255,0,0), 0, _T("pefromance: %0.4d.%0.4d "), 
        this->m_pSharedEnvironment->m_gfxInfo.m_dwPerfomanceSec, this->m_pSharedEnvironment->m_gfxInfo.m_dwPerfomanceMsec);

        m_Font->Dereference();
    }

    //drawCube(surface);

    if (this->m_SpritesArray[0]->Reference() != NULL)
    {
        unsigned int maxX = surface->Width();
        //unsigned int maxY = surface->Height();
        unsigned int dimX_png = this->m_TextureArray[0]->Width();
        unsigned int dimY_png = this->m_TextureArray[0]->Height();
        unsigned int dimX_gif = this->m_TextureArray[1]->Width();
        unsigned int dimY_gif = this->m_TextureArray[1]->Height();
        RECT  drawThere = {(LONG)(maxX - dimX_png), 0, maxX, dimY_png};
        this->m_SpritesArray[0]->Draw(surface, &drawThere, FALSE, 0);
        RECT drawThere1 = { (LONG)(maxX - dimX_gif), dimY_png, maxX, dimY_png + dimY_gif};
        this->m_SpritesArray[1]->Draw(surface, &drawThere1, FALSE, 0);

        this->m_SpritesArray[0]->Dereference();
    }

    /*BOOL bVisible = this->m_pSharedEnvironment->m_gfxInfo.IsMouseVisible();
    LONG mouseX = this->m_pSharedEnvironment->m_gfxInfo.GetMouseX();
    LONG mouseY = this->m_pSharedEnvironment->m_gfxInfo.GetMouseY();*/
}

void 
CWindowLogger::Dispatch(CSharedEnvironment *environment)
{
    dispatchCube();
    UNREFERENCED_PARAMETER(environment);
}


void
CWindowLogger::drawCube(class ISurface *surface)
{
    for (int i = 0; i < 6; i++)
    {

        int idx1 = (i << 1) + 0;
        int idx2 = (i << 1) + 1;

        if (m_faces[idx1].nz > 0)
        {
            TriangleTextured(surface, &m_faces[idx1]);
            TriangleTextured(surface, &m_faces[idx2]);
            //Triangle32(this->m_BackBufferDc, &faces[idx1]);
            //Triangle32(this->m_BackBufferDc, &faces[idx2]);
        }
    }
}

void
CWindowLogger::dispatchCube()
{
    memcpy(m_vertex, vertex_const, sizeof(m_vertex));

    /*for (int i = 0; i < 4; i++)
    {
        RotateTexure(&_vertex[i], fi);
    }*/

    {
        // quater 1 (face)
        m_faces[0].v1 = &m_vertex[0];
        m_faces[0].v2 = &m_vertex[1];
        m_faces[0].v3 = &m_vertex[3];
        m_faces[0].color = RGB(20, 255, 20);
		m_faces[0].texture = this->m_texture;
        m_faces[1].v1 = &m_vertex[1];
        m_faces[1].v2 = &m_vertex[2];
        m_faces[1].v3 = &m_vertex[3];
        m_faces[1].color = RGB(20, 255, 20);
		m_faces[1].texture = this->m_texture;
        // quater 2 (unface)
        m_faces[2].v1 = &m_vertex[6];
        m_faces[2].v2 = &m_vertex[5];
        m_faces[2].v3 = &m_vertex[4];
        m_faces[2].color = RGB(20, 20, 255);
        m_faces[2].texture = this->m_texture;
        m_faces[3].v1 = &m_vertex[4];
        m_faces[3].v2 = &m_vertex[7];
        m_faces[3].v3 = &m_vertex[6];
        m_faces[3].color = RGB(20, 20, 255);
        m_faces[3].texture = this->m_texture;
        // quater 3 (right)
        m_faces[4].v1 = &m_vertex[1];
        m_faces[4].v2 = &m_vertex[5];
        m_faces[4].v3 = &m_vertex[2];
        m_faces[4].color = RGB(255, 255, 20);
        m_faces[4].texture = this->m_texture;
        m_faces[5].v1 = &m_vertex[1];
        m_faces[5].v2 = &m_vertex[4];
        m_faces[5].v3 = &m_vertex[5];
        m_faces[5].color = RGB(255, 255, 20);
        m_faces[5].texture = this->m_texture;
        // quater 4 (left)
        m_faces[6].v1 = &m_vertex[0];
        m_faces[6].v2 = &m_vertex[3];
        m_faces[6].v3 = &m_vertex[7];
        m_faces[6].color = RGB(255, 20, 255);
        m_faces[6].texture = this->m_texture;
        m_faces[7].v1 = &m_vertex[3];
        m_faces[7].v2 = &m_vertex[6];
        m_faces[7].v3 = &m_vertex[7];
        m_faces[7].color = RGB(255, 20, 255);
        m_faces[7].texture = this->m_texture;
        // quater 5 (top)
        m_faces[8].v1 = &m_vertex[0];
        m_faces[8].v2 = &m_vertex[7];
        m_faces[8].v3 = &m_vertex[1];
        m_faces[8].color = RGB(20, 20, 20);
        m_faces[8].texture = this->m_texture;
        m_faces[9].v1 = &m_vertex[1];
        m_faces[9].v2 = &m_vertex[7];
        m_faces[9].v3 = &m_vertex[4];
        m_faces[9].color = RGB(20, 20, 20);
        m_faces[9].texture = this->m_texture;
        // quater 6 (bottom)
        m_faces[10].v1 = &m_vertex[3];
        m_faces[10].v2 = &m_vertex[5];
        m_faces[10].v3 = &m_vertex[6];
        m_faces[10].color = RGB(255, 255, 255);
        m_faces[10].texture = this->m_texture;
        m_faces[11].v1 = &m_vertex[3];
        m_faces[11].v2 = &m_vertex[2];
        m_faces[11].v3 = &m_vertex[5];
        m_faces[11].color = RGB(255, 255, 255);
        m_faces[11].texture = this->m_texture;
    }

    for (int i = 0; i < 8; i++)
    {
        xRotateVertex(&m_vertex[i], this->m_pSharedEnvironment->m_gfxInfo.m_fi);
        yRotateVertex(&m_vertex[i], this->m_pSharedEnvironment->m_gfxInfo.m_fi);
        zRotateVertex(&m_vertex[i], this->m_pSharedEnvironment->m_gfxInfo.m_fi);

        projectVertex(&m_vertex[i]);
    }

    calcNormals(m_faces, 12, m_vertex, 8);

    // sort by Z (bubble sort :-) )
    for (register int i = 0; i < 6; i++)
    {
        for (register int j = i; j < 6; j++)
        {
            int idx1 = (i << 1);
            int idx2 = (i << 1) + 1;
            int jdx1 = (j << 1);
            int jdx2 = (j << 1) + 1;

            LONG z1 = (m_faces[idx1].v1->z + m_faces[idx1].v2->z + m_faces[idx1].v3->z + 
                       m_faces[idx2].v1->z + m_faces[idx2].v2->z + m_faces[idx2].v3->z) / 6;

            LONG z2 = (m_faces[jdx1].v1->z + m_faces[jdx1].v2->z + m_faces[jdx1].v3->z + 
                       m_faces[jdx2].v1->z + m_faces[jdx2].v2->z + m_faces[jdx2].v3->z) / 6;

            if (z1 > z2)
            {
                Face temp = m_faces[idx1];
                m_faces[idx1] = m_faces[jdx1];
                m_faces[jdx1] = temp;

                temp = m_faces[idx2];
                m_faces[idx2] = m_faces[jdx2];
                m_faces[jdx2] = temp;
            }
        }
    }

    if (this->m_bChangeAngle)
    {
        this->m_pSharedEnvironment->m_gfxInfo.m_fi = this->m_pSharedEnvironment->m_gfxInfo.m_fi + (float)0.01;

        if (this->m_pSharedEnvironment->m_gfxInfo.m_fi > 6.28) this->m_pSharedEnvironment->m_gfxInfo.m_fi = 0;
    }

}

void
CWindowLogger::makeTexture(UCHAR **texture)
{
    UCHAR *d = *texture;

    /*for (i = 0; i < 256; i++)
    for (j = 0; j < 256; j++)
    *d++ = 0x80 ^ ((i ^ j) >> 1);*/
    unsigned char text[2][8][6] = 
    {
        {{'*', '*', 'Z', 'Z', '*', '*'},
         {'*', 'Z', '*', 'Z', '*', '*'},
         {'*', 'Z', '*', '*', '*', '*'},
         {'*', 'Z', '*', '*', '*', '*'},
         {'*', 'Z', 'Z', 'Z', '*', '*'},
         {'*', 'Z', '*', 'Z', '*', '*'},
         {'*', '*', 'Z', 'Z', '*', '*'}},
        {{'*', 'Z', 'Z', 'Z', '*', '*'},
         {'*', 'Z', '*', '*', '*', '*'},
         {'*', 'Z', 'Z', '*', '*', '*'},
         {'*', 'Z', '*', '*', '*', '*'},
         {'*', 'Z', '*', '*', '*', '*'},
         {'*', 'Z', '*', '*', '*', '*'},
         {'*', 'Z', 'Z', 'Z', '*', '*'}}

    };


    for (int i = 0; i < 180; i++)
    {
        for (float rad = 0.0; rad < 6.28; rad += (float)0.001)
        {
            int x = (int)(127 + (float)i * cos(rad));
            int y = (int)(127 + (float)i * sin(rad));

            if (x >= 0 && y >= 0 && x < 256 && y < 256)
            {
                d[y * 256 + x] = (unsigned char)(i);
            }
        }
    }

    for (int ch = 0; ch < 2; ch++)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                d[(50 + i * 2) * 256 + (10 * ch + 50 + j)] = text[ch][i][j];
                d[(51 + i * 2) * 256 + (10 * ch + 50 + j)] = text[ch][i][j];
                d[(50 + i * 2) * 256 + (10 * ch + 51 + j)] = text[ch][i][j];
                d[(51 + i * 2) * 256 + (10 * ch + 51 + j)] = text[ch][i][j];
            }
        }
    }
}