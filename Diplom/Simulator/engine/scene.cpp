/***************************************************************************************\
*   File:                                                                               *
*       scene.cpp                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       current played scene interface                                                  *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"
#include <math.h>

#include "sfx\inc\waveFormat.h"
#include "sfx\inc\sound.h"
#include "sfx\inc\riffFile.h"
#include "sfx\inc\soundWorld.h"


#include "gfx\inc\texture.h"
#include "gfx\inc\sprite.h"
#include "gfx\inc\font.h"

#include "scene.h"
#include "environment.h"
#include "gfx\inc\surface.h"
#include "movemap.h"
#include "character.h"
#include "mouse.h"

#include "fs\inc\fileSystem.h"
#include "fs\inc\fileStream.h"



/*
    characters
*/
#include "ghost_character.h"
#include "drawing_character.h"
#include "simple_character.h"

ISprite *
IScene::GetSprite(int i)
{
    ISprite *pSprite = 0;

    if ((pSprite = (ISprite *)m_SpritesArray[i]->Reference()) == 0)
    {
        return NULL;
    }

    return pSprite;
}

ITexture *
IScene::GetTexture(int index)
{
    ITexture *pTexture = m_TextureArray[index];

    if (pTexture->Reference() == 0)
        return 0;

    return pTexture;
}


BOOL
CGhostScene::Init()
{

    this->m_pFileSystem = new CFileSystem();

    if (this->m_pFileSystem == NULL)
    {
        return FALSE;
    }

    if (this->m_pFileSystem->Init() != TRUE)
    {
        goto __destroy_fs;
    }



    this->m_MaxX = this->m_pSharedEnvironment->m_gfxInfo.GetMaxX();
    this->m_MaxY = this->m_pSharedEnvironment->m_gfxInfo.GetMaxY();
    this->m_MinX = this->m_pSharedEnvironment->m_gfxInfo.GetMinX();
    this->m_MinY = this->m_pSharedEnvironment->m_gfxInfo.GetMinY();

    BOOL _return = this->InitializeTextures();

    if (_return == FALSE)
    {
        goto __finalyze_fs;
    }

    _return = this->InitializeCursors();

    if (_return == FALSE)
    {
        goto __FREE_TEXTURES;
    }

    _return = this->InitializeSprites();

    if (_return == FALSE)
    {
        goto __FREE_CURSORS;
    }

    _return = this->InitializeMaps();

    if (_return == FALSE)
    {
        goto __FREE_SPRITES;
    }

    _return = this->InitializeCharacters();

    if (_return == FALSE)
    {
        goto __FREE_MAPS;
    }

    _return = this->InitializeSounds();

    if (_return == FALSE)
    {
        goto __FREE_CHARACTERS;
    }

    _return = this->InitializeFonts();

    if (_return == FALSE)
    {
        goto __FREE_SOUNDS;
    }

    this->m_pSoundWorld = new CSoundWorld();

    if (this->m_pSoundWorld == 0)
    {
        goto __FREE_FONTS;
    }

    if (this->m_pSoundWorld->Init() != TRUE)
    {
        goto __RELEASE_SOUND_WORLD;
    }

    return TRUE;

__RELEASE_SOUND_WORLD:
    delete this->m_pSoundWorld;

__FREE_FONTS:
    this->ShutdownFonts();

__FREE_SOUNDS:
    this->ShutdownSounds();

__FREE_CHARACTERS:
    this->ShutdownCharacters();

__FREE_MAPS:
    this->ShutdownMaps();

__FREE_SPRITES:
    this->ShutdownSprites();

__FREE_CURSORS:
    this->ShutdownCursors();

__FREE_TEXTURES:
    this->ShutdownTextures();


__finalyze_fs:
    this->m_pFileSystem->Free();

__destroy_fs:
    delete[] (UCHAR *)this->m_pFileSystem;

    return FALSE;

}

void
CGhostScene::Free()
{
    this->m_pSoundWorld->Free();
    delete this->m_pSoundWorld;

    this->ShutdownFonts();
    this->ShutdownSounds();
    this->ShutdownCharacters();
    this->ShutdownMaps();
    this->ShutdownSprites();

    this->ShutdownCursors();
    this->ShutdownTextures();

    this->m_pFileSystem->Free();
    delete[] (UCHAR *) this->m_pFileSystem;

}

void
CGhostScene::Draw(class ISurface *surface)
{

    RECT clean = {0, 0, 1024, 768 };

    ISprite **pSprites = this->m_SpritesArray;

    (pSprites[0])->Draw(surface, &clean, TRUE);
    //(pSprites[3])->Draw(surface, &clean, FALSE, RGB(128, 128, 128));
    //(pSprites[5])->Draw(surface, &clean, FALSE, RGB(128, 128, 128));

    if (m_Fonts[0]->Reference() != NULL)
    {
        /*SIZE_T fontDX = m_Fonts[0]->Width() + 1;
        SIZE_T fontDY = m_Fonts[0]->Height() + 1;
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                m_Fonts[0]->DrawCharacter(surface, (UCHAR)(i * 16 + j), 
                    350 + j * fontDX, 
                    20 + i * fontDY, RGB(255, 0, 0), NULL);
            }
        }*/

        //m_Fonts[0]->DrawCharacter(surface, 8, 350, 20, RGB(255, 0, 0), NULL);

        m_Fonts[0]->Dereference();
    }

    ICharacter **pCharacters = this->m_CharacterArray;

    while (*pCharacters != 0)
    {
        (*pCharacters)->Draw(surface);

        pCharacters++;
    }


    BOOL bVisible = this->m_pSharedEnvironment->m_gfxInfo.IsMouseVisible();
    LONG mouseX = this->m_pSharedEnvironment->m_gfxInfo.GetMouseX();
    LONG mouseY = this->m_pSharedEnvironment->m_gfxInfo.GetMouseY();


    if (bVisible)
    {
        IMouse *pMouse = this->GetCurrentMouse();

        pMouse->Draw(surface);

        surface->SetPixel(mouseX, mouseY, RGB(255, 0, 0));
        surface->SetPixel(mouseX + 1, mouseY + 1, RGB(255, 0, 0));
        surface->SetPixel(mouseX - 1, mouseY + 1, RGB(255, 0, 0));
        surface->SetPixel(mouseX + 1, mouseY - 1, RGB(255, 0, 0));
        surface->SetPixel(mouseX - 1, mouseY - 1, RGB(255, 0, 0));
    }
}

BOOL
CGhostScene::EmitSound(int iIndex)
{
    IWaveSource *pWaveSource = this->GetSound(iIndex);

    if (pWaveSource == 0)
        return FALSE;

    ISound *pSound = new CSoundWaveFile();

    if (pSound == 0)
    {
        pWaveSource->Dereference();
        return FALSE;
    }

    if (pSound->Init(pWaveSource) != TRUE)
    {
        delete pSound;
        pWaveSource->Dereference();
        return FALSE;
    }

    if (this->GetSoundWorld()->EmitSound(pSound) != TRUE)
    {
        pSound->Free();
        delete pSound;
        pWaveSource->Dereference();
        return FALSE;
    }

    pWaveSource->Dereference();

    return TRUE;
}


void 
CGhostScene::Dispatch(CSharedEnvironment *environment)
{

    IMouse *pMouse = this->GetCurrentMouse();

    pMouse->Dispatch(environment);

    ICharacter **pCharacters = this->m_CharacterArray;

    while (*pCharacters != 0)
    {
        (*pCharacters)->Dispatch(environment);

        pCharacters++;
    }
}

BOOL
CGhostScene::InitializeCursors()
{
    m_Cursors[0] = new CMouse(this);

    if (m_Cursors[0] == 0)
    {
        return FALSE;
    }

    if (m_Cursors[0]->Init(m_TextureArray[0]) != TRUE)
    {
        delete[] m_Cursors[0];
        return FALSE;
    }

    return TRUE;
}

VOID
CGhostScene::ShutdownCursors()
{
    m_Cursors[0]->Free();
    delete[] m_Cursors[0];
}

const TCHAR  *g_fonts[] = {
    _T("data\\fonts\\app866.fon"),
};

BOOL
CGhostScene::InitializeFonts()
{
    BOOL _return = FALSE;

    for (unsigned int i = 0; i < ARRAY_SIZE(g_fonts); i++)
    {
        IDataStream *pDataStream = this->m_pFileSystem->OpenFileRead(g_fonts[i], 0, TRUE);

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

        m_Fonts[i] = pFont;
    }

    return TRUE;
}

VOID
CGhostScene::ShutdownFonts()
{
    m_Fonts[0]->Free();
    delete m_Fonts[0];
}

const TCHAR  *g_sounds[] = {
    _T("data\\sounds\\doorbl2.wav"),
};

BOOL
CGhostScene::InitializeSounds()
{
    BOOL _return = FALSE;

    for (unsigned int i = 0; i < ARRAY_SIZE(g_sounds); i++)
    {
        IDataStream *pDataStream = this->m_pFileSystem->OpenFileRead(g_sounds[i], 0, TRUE);

        if (pDataStream == NULL)
        {
            return FALSE;
        }

        IWaveSource *pWaveSource = new CRiffWaveSource(0);

        if (pWaveSource == NULL)
        {
            pDataStream->Free();
            delete pDataStream;
            return FALSE;
        }

        _return = pWaveSource->Init(pDataStream);

        pDataStream->Free();
        delete pDataStream;

        if (_return == FALSE)
        {
            delete pWaveSource;
            return FALSE;
        }

        m_Sounds[i] = pWaveSource;
    }

    return TRUE;
}

VOID
CGhostScene::ShutdownSounds()
{
    m_Sounds[0]->Free();
    delete m_Sounds[0];
}

const int characterAssigments[] = {
    0, /* ghost */
    1, /* ball */
    2, /* drawing */
};

BOOL
CGhostScene::InitializeCharacters()
{
    ICharacter* pCharacter;
    BOOL _return = FALSE;

    /*pCharacter = new CGhostCharacter(this);

    if (pCharacter == NULL)
    {
        return FALSE;
    }

    _return = pCharacter->Init();

    if (_return == FALSE)
    {
        delete pCharacter;
        return FALSE;
    }*/

    //this->m_CharacterArray[0] = pCharacter;

    /*pCharacter = new CDrawingCharacter(this);

    if (pCharacter == NULL)
    {
        return FALSE;
    }

    _return = pCharacter->Init();

    if (_return == FALSE)
    {
        delete pCharacter;
        return FALSE;
    }

    this->m_CharacterArray[2] = pCharacter;*/

    pCharacter = new CSimpleCharacter(this);

    if (pCharacter == NULL)
    {
        return FALSE;
    }

    _return = pCharacter->Init();

    if (_return == FALSE)
    {
        delete pCharacter;
        return FALSE;
    }

    this->m_CharacterArray[0] = pCharacter;

    this->m_CharacterArray[1] = 0;

    return TRUE;
}

VOID
CGhostScene::ShutdownCharacters()
{
    int i = 0;
    while (this->m_CharacterArray[i] != 0)
    {
        this->m_CharacterArray[i]->Free();
        delete this->m_CharacterArray[i];

        i++;
    }
}


const int mapsAssigments[] = {
    6, /* height map */
    7, /* move map */
    8, /* zone map */
};

BOOL
CGhostScene::InitializeMaps()
{
    for (int i = 0; i < ARRAY_SIZE(mapsAssigments); i++)
    {
        IMoveMap *pMaps = new CMoveMap(m_TextureArray[mapsAssigments[i]]);

        if (pMaps == NULL)
        {
            return FALSE;
        }

        RECT  mapRect = {0, 0, 640, 480};

        BOOL _return = pMaps->Init(&mapRect, 0);

        if (_return == FALSE)
        {
            delete pMaps;
            return FALSE;
        }

        this->m_MapsArray[i] = pMaps;
    }

    return TRUE;
}

VOID
CGhostScene::ShutdownMaps()
{
    for (int i = 0; i < ARRAY_SIZE(mapsAssigments); i++)
    {
        this->m_MapsArray[i]->Free();
        delete this->m_MapsArray[i];
    }
}


const int spriteAssigments[] = {
    11, /* fone picture */
    4, /* ghost picture */
    5, /* ball picture */
    6, /* height map picture */
    7, /* move map picture */
    8, /* zone map picture */
    9, /* test JPEG */
    12, /* drone */
//    11 /* Drawing   */
};



BOOL
CGhostScene::InitializeSprites()
{
    for (int i = 0; i < ARRAY_SIZE(spriteAssigments); i++)
    {
        ISprite *pSprite = new CSprite(m_TextureArray[spriteAssigments[i]]);

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
CGhostScene::ShutdownSprites()
{
    for (int i = 0; i < ARRAY_SIZE(spriteAssigments); i++)
    {
        this->m_SpritesArray[i]->Free();
        delete this->m_SpritesArray[i];
    }
}

const TCHAR  *g_textures[] = {
    _T("data\\cursors\\no.bmp"),                 // 0
    _T("data\\textures\\gurevich_1m.jpg"),       // 1
    _T("data\\textures\\picture3.jpg"),          // 2
    _T("data\\textures\\picture2.jpg"),          // 3
    _T("data\\textures\\ghost.jpg"),             // 4
    _T("data\\textures\\BALLBLUE.GIF"),          // 5
    //_T("data\\textures\\treescap-interlaced.gif"),
    //_T("data\\textures\\treescap.gif"),
    _T("data\\maps\\height_map.jpg"),            // 6
    _T("data\\maps\\move_map.jpg"),              // 7
    _T("data\\maps\\zone_map.jpg"),              // 8
    _T("data\\textures\\pngtest.png"),           // 9
    _T("data\\textures\\150px.png"),             // 10
    //_T("data\\textures\\Drawing1.jpg")           // 11
    _T("data\\textures\\212-home.jpeg"),       // 11
    _T("data\\textures\\drone.gif"),       // 12
    //_T("data\\textures\\msie-uninit.jpg"),
};
BOOL
CGhostScene::InitializeTextures()
{
    BOOL _return = FALSE;

    for (unsigned int i = 0; i < ARRAY_SIZE(g_textures); i++)
    {
        IDataStream *pDataStream = this->m_pFileSystem->OpenFileRead(g_textures[i], 0, TRUE);

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
CGhostScene::ShutdownTextures()
{
    for (unsigned int i = 0; i < ARRAY_SIZE(g_textures); i++)
    {
        m_TextureArray[i]->Free();
        delete m_TextureArray[i];
    }
}
