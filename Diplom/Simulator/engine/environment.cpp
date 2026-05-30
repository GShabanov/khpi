/***************************************************************************************\
*   File:                                                                               *
*       environment.cpp                                                                 *
*                                                                                       *
*   Abstract:                                                                           *
*       shared globals environment values                                               *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 13-Jan-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 68 $
#include "stdafx.h"
#include "environment.h"
#include "fastmath.h"

#include "gfx\inc\font.h"

#include "sfx\inc\waveFormat.h"
#include "sfx\inc\sound.h"
#include "scene.h"


BOOL
CSharedEnvironment::Initialize()
{
    if (InitFastMath() != TRUE)
        return FALSE;


    this->m_pCurrentScene = 0;

    return TRUE;
}

void
CSharedEnvironment::Finalize()
{
    if (this->m_pCurrentScene != 0)
    {
        this->m_pCurrentScene->Free();

        delete this->m_pCurrentScene;

        this->m_pCurrentScene = 0;
    }

    FinalizeFastMath();

}

BOOL
CSharedEnvironment::LoadScene(int nNumber)
{
    UNREFERENCED_PARAMETER(nNumber);
    //
    // initialize scenes
    //
    IScene  *pScene = new CGhostScene(this);

    if (pScene == 0)
    {
        return FALSE;
    }

    if (pScene->Init() != TRUE)
    {
        delete pScene;
        return FALSE;
    }

    this->m_pCurrentScene = pScene;

    return TRUE;
}

BOOL
CSharedEnvironment::LockExclusive()
{
    return TRUE;
}

BOOL
CSharedEnvironment::LockShared()
{
    return TRUE;
}

void
CSharedEnvironment::Unlock()
{

}

void
CSharedEnvironment::Update()
{
    this->m_bMouseLClick = FALSE;
}

void
CSharedEnvironment::SetMouseLDown(BOOL bDown)
{
    if (bDown)
    {
        this->m_bMouseLDown = TRUE;
    }
    else
    {
        if (this->m_bMouseLDown == TRUE)
        {
            this->m_bMouseLClick = TRUE;
        }

        this->m_bMouseLDown = FALSE;
    }
}

void
CGfxData::SetMousePos(LONG mouseX, LONG mouseY, BOOL mouseVisiable)
{
    this->m_lMouseX = mouseX;
    this->m_lMouseY = mouseY;

    /*if (this->m_Cursors[0] != 0)
    {
        this->m_Cursors[0]->SetMousePos(mouseX, mouseY);
    }*/
    this->m_bMouseVisible = mouseVisiable;
}

