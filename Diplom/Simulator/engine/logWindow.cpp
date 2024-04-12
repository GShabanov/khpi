/***************************************************************************************\
*   File: 
*       logWindow.cpp
*
*   Abstract:
*       Main user window implementation
*
*   Author: 
*       Gennady Shabanov (gennady.shabanov@gmail.com) 11-Feb-2014
*
*   Revision History:
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"

#include "defaultWindow.h"
#include "logWindow.h"

#include "gameEngine.h"
#include "environment.h"
#include "gfx\inc\drawEngine.h"


#undef MAX_LOADSTRING
#define MAX_LOADSTRING 100

#define BUTTONS_OPTIONS (BO_STARFORCE_SUPPORT + BO_CONFIGURATION_MENU + BO_ABOUT_PROGRAM)


BOOL
CLogWindow::Create(
    HINSTANCE hInstance,
    TCHAR   *tcClassName,
    RECT* lpRect,
    CGameEngine* pGameEngine,
    CLogWindow** ppNewInstance
    )
/*++

   Routine:
        Create()

   Description:
        Creates and initializes new class

   Arguments:
        hInstance           - [in] instance for current APP module


        ppNewInstance       - [out] created instance if success

   Return:
        BOOL
--*/
{

    CLogWindow *This;

    This = reinterpret_cast<CLogWindow *>(
        new UCHAR[sizeof(CLogWindow)]);

    if (This == NULL)
        return FALSE;

    This->CLogWindow::CLogWindow(hInstance, tcClassName);

    This->m_pGameEngine = pGameEngine;

    This->m_ptWindowTitle = "LOGS"; //NULL;//pConfiguration->m_szWindowName;

    if (This->InitWindowInstance(lpRect) == FALSE)
        goto __error_class_destructor;

    *ppNewInstance = This;

    return TRUE;

__error_class_destructor:
    This->CLogWindow::~CLogWindow();
    delete[] This;

    return FALSE;
}

VOID
CLogWindow::Destroy()
/*++

   Routine:
        DestroyInstance()

   Description:
        Destroy previosly created class

   Arguments:
        pInstance     - [in] class instance to destroy

   Return:
        BOOL
--*/
{

    this->DestroyWindowInstance();

    this->CLogWindow::~CLogWindow();

    delete[] (UCHAR *)this;

}

CLogWindow::CLogWindow(HINSTANCE hInstance, TCHAR *ptClassName)
: CDefaultWindow(hInstance, NULL, ptClassName)
/*++
    Routine:
        CMainWindow()

    Descrition:
        class constructor
--*/
{
    m_bTermitating = FALSE;
    m_bMouseCaptured = FALSE;
    m_bMouseClicked = FALSE;
    m_bMouseClick = FALSE;

}

CLogWindow::~CLogWindow()
/*++
    Routine:
        ~CLogWindow()

    Descrition:
        class destructor
--*/
{
}

BOOL
CLogWindow::InitWindowInstance(IN LPRECT Rect)
/*++

   Routine:
        InitWindowInstance()

   Description:
        Saves instance handle and creates main window

   Comment:
        In this function, we save the instance handle in a global variable and
        create and display the main program window.
--*/
{

    //
    //  default contsruction
    //
    if (TRUE != CDefaultWindow::InitWindowInstance(
        NULL, Rect, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS))
    {
        return FALSE;
    }

    //subclass the window
    this->m_WindowSubclass = reinterpret_cast<WNDPROC>(
        (LONG_PTR)SetWindowLong(this->m_hwndWnd, GWLP_WNDPROC, (LONG)(LONG_PTR)&WindowSubclass));


    ::ShowWindow(this->m_hwndWnd, SW_SHOWNORMAL);

    //::UpdateWindow(this->m_hwndWnd);

    //
    //  normal exit
    //
    return TRUE;
}

BOOL
CLogWindow::DestroyWindowInstance()
/*++

   Routine:
        DestroyWindowInstance()

   Description:
        Destroy tray icon and destroy windows class
--*/
{
    // remove main window subclassing
    SetWindowLong(this->m_hwndWnd, GWLP_WNDPROC, (LONG)(LONG_PTR)this->m_WindowSubclass);

    CDefaultWindow::DestroyWindowInstance();

    return TRUE;
}


VOID
CLogWindow::InvalidateClientArea()
{
	RECT rect;

	::GetClientRect(this->m_hwndWnd, &rect);

	::InvalidateRect(this->m_hwndWnd, &rect, FALSE);
}


MSG_HANDLER_FUNCTION(CLogWindow, WM_COMMAND)
/*++

   Routine:
        WM_COMMAND_Dispatch()

   Description:
        dispach for windows controls
--*/
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);
    return S_OK;
}

MSG_HANDLER_FUNCTION(CLogWindow, WM_PAINT)
/*++

   Routine:
        WM_PAINT Message handler

   Description:
        dispach for windows controls
--*/
{

    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    
    RECT        clRect;

    this->GetClientRect(&clRect);

	this->m_pGameEngine->GetLogsDraw()->CopyToWnd(&clRect);

    return S_OK;
}

MSG_HANDLER_FUNCTION(CLogWindow, WM_DESTROY)
/*++

    Routine:
        CMainWindow::OnClose

    Descrition:
        Processing exit event

    Arguments:

    Return:
        LRESULT

--*/
{
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(wParam);

    return ::DefWindowProc(this->m_hwndWnd, WM_DESTROY, wParam, lParam);
}

MSG_HANDLER_FUNCTION(CLogWindow, WM_SIZE)
/*++

    Routine:
        CMainWindow::ResizeClient

    Descrition:
        Resize clients

    Arguments:

        dwWidth  - [in] client area width

        dwHeigth - [in] client area heigth

    Return:
        LRESULT

--*/
{
    UNREFERENCED_PARAMETER(wParam);

    //INT     iWidth = LOWORD(lParam) - this->m_InflatingRect.left - this->m_InflatingRect.right;
    //INT     iHeigth = HIWORD(lParam) - this->m_InflatingRect.top - this->m_InflatingRect.bottom;
    //ULONG   ulLogoCaption = INT_LOGOTYPE_WND_HEIGHT + this->m_ulCaptionHeight;

    ResizeClient(LOWORD(lParam), HIWORD(lParam));

    return S_OK;

}

VOID 
CLogWindow::ResizeClient(DWORD dwWidth, DWORD  dwHeight)
{
    RECT    wndRect;

    wndRect.top    = this->m_InflatingRect.top;
    wndRect.bottom = dwHeight - this->m_InflatingRect.bottom; // - this->m_InflatingRect.top;
    wndRect.left   = this->m_InflatingRect.left;
    wndRect.right  = dwWidth - this->m_InflatingRect.right; //this->m_InflatingRect.left;

}

MSG_HANDLER_FUNCTION(CLogWindow, WM_ACTIVATE)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    CDefaultWindow *pDefaultWindow = this->m_pGameEngine->GetMainWindow();

    SwitchToThisWindow(pDefaultWindow->GetWindow(), TRUE);
    EnableWindow(pDefaultWindow->GetWindow(), TRUE);
    ShowWindow(pDefaultWindow->GetWindow(), SW_SHOWDEFAULT);

    //SetActiveWindow(pDefaultWindow->GetWindow());
    //SetFocus(pDefaultWindow->GetWindow());
    //SetForegroundWindow(pDefaultWindow->GetWindow());

    return S_OK;
}

MSG_HANDLER_FUNCTION(CLogWindow, WM_MOUSEMOVE)
{
    UNREFERENCED_PARAMETER(wParam);
    POINT pt;
	
    pt.x = (short)LOWORD(lParam);
    pt.y = (short)HIWORD(lParam);

    this->m_bMouseCaptured = TRUE;

    /*this->m_pShraredEnvironent->LockExclusive();

    this->m_pShraredEnvironent->m_gfxInfo.SetMousePos(pt.x, pt.y, this->m_bMouseCaptured);

    this->m_pShraredEnvironent->Unlock();*/

    //while (::ShowCursor(FALSE) >= 0);

    return S_OK;
}

MSG_HANDLER_FUNCTION(CLogWindow, WM_NCMOUSEMOVE)
{
    UNREFERENCED_PARAMETER(wParam);
    POINT pt;
	
    pt.x = (short)LOWORD(lParam);
    pt.y = (short)HIWORD(lParam);

    this->m_bMouseCaptured = FALSE;

    /*this->m_pShraredEnvironent->LockExclusive();

    this->m_pShraredEnvironent->m_gfxInfo.SetMousePos(pt.x, pt.y, this->m_bMouseCaptured);

    this->m_pShraredEnvironent->Unlock();

    while (::ShowCursor(TRUE) < 0);*/

    return S_OK;
}



LRESULT
CLogWindow::WindowProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
/*++

    Routine:
        WindowProc(unsigned, WORD, LONG)

    Descrition:
        Processes messages for the main window.

    Arguments:
        WM_COMMAND  - process the application menu
        WM_PAINT    - Paint the main window
        WM_DESTROY  - post a quit message and return

--*/
{
    switch (message)
    {
    HANDLE_MSG(WM_COMMAND);
    HANDLE_MSG(WM_PAINT);
    HANDLE_MSG(WM_SIZE);
    HANDLE_MSG(WM_ACTIVATE);
    //HANDLE_MSG(WM_DESTROY);
    HANDLE_MSG(WM_MOUSEMOVE);
    HANDLE_MSG(WM_NCMOUSEMOVE);

    /*case WM_SETFOCUS:
        {
            for (int i = 1; i < 5; i++) {
                VM_SetVarToDrRegister(this->m_pAntidebugContext, i, 0);
            }
        }

        return SendMessage(this->m_pDriveListWindow->GetWindow(), WM_SETFOCUS, 0, 0);*/

    default:;
    }

    return ::DefWindowProc(this->m_hwndWnd, message, wParam, lParam);
}

LRESULT
CLogWindow::WindowSubclass(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CLogWindow *lpThis = reinterpret_cast<CLogWindow *>(
        (LONG_PTR)GetWindowLong(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_ACTIVATEAPP:
    case WM_NCACTIVATE:
        {
            LRESULT ret = ::CallWindowProc(lpThis->m_WindowSubclass, lpThis->m_hwndWnd, message, wParam, lParam);
            return ret;
        }


    case WM_MOUSEMOVE:
        ::SetCursor(lpThis->m_hkCurrentCursor);


    default:;
    }

    return ::CallWindowProc(lpThis->m_WindowSubclass, hWnd, message, wParam, lParam);
}
