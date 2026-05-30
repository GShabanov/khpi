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
#include <new>
#include "inc\drawEngine.h"

#include "environment.h"
#include "..\sfx\inc\waveFormat.h"
#include "..\sfx\inc\sound.h"

#include "inc\font.h"
#include "scene.h"


#include "inc\surface.h"

BOOL
CDrawEngine::Create(IN CDrawEngine  *lpThis, class CSharedEnvironment *pShraredEnvironent, IN HWND hWnd)
{
    lpThis = new (lpThis) CDrawEngine(hWnd, pShraredEnvironent);

    BOOL _return = lpThis->InitializeSurface();

    if (_return == FALSE)
    {
        goto __FREE_CLASS;
    }

    return TRUE;


__FREE_CLASS:
    lpThis->~CDrawEngine();

    return FALSE;
}

VOID
CDrawEngine::Destroy()
{
    this->ShutdownSurface();

    this->~CDrawEngine();

}

VOID
CDrawEngine::CopyToWnd(RECT *lpCopyRect)
{
    PAINTSTRUCT paint_str;

    HDC hdc = ::BeginPaint(this->m_window, &paint_str);

    if (hdc == NULL)
        return;	

    this->m_CurrentSurface->Blt(hdc, lpCopyRect);

    ::EndPaint(this->m_window, &paint_str);

}

VOID
CDrawEngine::DrawingProc(IGameObject  *pGameObject)
{
    this->m_CurrentSurface->Cleanup(RGB(0, 0, 0));

    pGameObject->Draw(this->m_CurrentSurface);
}

BOOL
CDrawEngine::InitializeSurface()
{
    BOOL   _return = TRUE;

    HDC hdc = GetDC(this->m_window);

    if (hdc == NULL)
        return FALSE;


    RECT  wndRect;
    GetClientRect(this->m_window, &wndRect);

    //this->m_CurrentSurface = new CSurfaceAPI(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, 4);
    this->m_CurrentSurface = new CSurfaceRAW(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, 4);

    if (this->m_CurrentSurface == 0)
    {
        ReleaseDC(this->m_window, hdc);
        return FALSE;
    }
    //this->m_CurrentSurface = new CSurfaceRAW(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, 4);

    if (this->m_CurrentSurface->Init(hdc) != TRUE)
    {
        delete this->m_CurrentSurface;
        _return = FALSE;
    }

    ReleaseDC(this->m_window, hdc);

    this->m_Rect = wndRect;

    return _return;

}

VOID
CDrawEngine::ShutdownSurface()
{
    this->m_CurrentSurface->Free();
    delete this->m_CurrentSurface;
}

