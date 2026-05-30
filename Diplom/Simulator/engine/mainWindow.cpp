/***************************************************************************************\
*   File: 
*       mainWindow.cpp
*
*   Abstract:
*       Main user window implementation
*
*   Author: 
*       Gennady Shabanov (gennady.shabanov@gmail.com) 12-Feb-2014
*
*   Revision History:
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"

#include "defaultWindow.h"
#include "mainWindow.h"

#include "gameEngine.h"
#include "environment.h"
#include "gfx\inc\drawEngine.h"


#undef MAX_LOADSTRING
#define MAX_LOADSTRING 100

#define BUTTONS_OPTIONS (BO_STARFORCE_SUPPORT + BO_CONFIGURATION_MENU + BO_ABOUT_PROGRAM)


BOOL
CMainWindow::Create(
                    HINSTANCE                hInstance,
                    TCHAR                    *tcClassName,
                    RECT                     *lpRect,
                    CGameEngine             *pGameEngine,
                    CMainWindow             **ppNewInstance)
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

    CMainWindow *This;

    This = new CMainWindow(hInstance, tcClassName);

    if (This == NULL)
        return FALSE;

    //This->CMainWindow::CMainWindow(hInstance, tcClassName);

    This->m_pGameEngine = pGameEngine;

    This->m_ptWindowTitle = "TITLE"; //NULL;//pConfiguration->m_szWindowName;

    INITCOMMONCONTROLSEX    icce;
    icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icce.dwICC = 
        ICC_BAR_CLASSES      |
        ICC_LISTVIEW_CLASSES |
        ICC_PROGRESS_CLASS   |
        ICC_STANDARD_CLASSES |
        ICC_TREEVIEW_CLASSES;

    InitCommonControlsEx(&icce);

    if (This->InitWindowInstance(lpRect) == FALSE)
        goto __error_class_destructor;

    *ppNewInstance = This;

    return TRUE;

__error_class_destructor:
    This->CMainWindow::~CMainWindow();
    delete[] This;

    return FALSE;
}

VOID
CMainWindow::Destroy()
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

    this->CMainWindow::~CMainWindow();

    delete[] (UCHAR *)this;

}

CMainWindow::CMainWindow(HINSTANCE hInstance, TCHAR *ptClassName)
: CDefaultWindow(hInstance, NULL, ptClassName)
/*++
    Routine:
        CMainWindow()

    Descrition:
        class constructor
--*/
{
    m_pGameEngine = 0;
    m_bMouseCaptured = FALSE;

}

CMainWindow::~CMainWindow()
/*++
    Routine:
        ~CMainWindow()

    Descrition:
        class destructor
--*/
{
}

BOOL
CMainWindow::InitWindowInstance(IN LPRECT Rect)
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
        NULL, Rect, WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS))
    {
        return FALSE;
    }

    //subclass the window
    this->m_WindowSubclass = reinterpret_cast<WNDPROC>(
        (LONG_PTR)SetWindowLong(this->m_hwndWnd, GWLP_WNDPROC, (LONG)(LONG_PTR)&WindowSubclass));


    INT Parts[2];
    Parts[0] = 80;
    Parts[1] = -1;



    ::ShowWindow(this->m_hwndWnd, SW_SHOWNORMAL);

    SetForegroundWindow(this->m_hwndWnd);

    //::UpdateWindow(this->m_hwndWnd);


    //
    //  normal exit
    //
    return TRUE;
}

BOOL
CMainWindow::DestroyWindowInstance()
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
CMainWindow::InvalidateClientArea()
{
    RECT rect;

    ::GetClientRect(this->m_hwndWnd, &rect);

    ::InvalidateRect(this->m_hwndWnd, &rect, FALSE);
}


MSG_HANDLER_FUNCTION(CMainWindow, WM_COMMAND)
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

MSG_HANDLER_FUNCTION(CMainWindow, WM_PAINT)
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

    this->m_pGameEngine->GetDrawEngine()->CopyToWnd(&clRect);

    return S_OK;
}

MSG_HANDLER_FUNCTION(CMainWindow, WM_DESTROY)
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

    ::PostQuitMessage(0);

    return ::DefWindowProc(this->m_hwndWnd, WM_DESTROY, wParam, lParam);
}

MSG_HANDLER_FUNCTION(CMainWindow, WM_SIZE)
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
CMainWindow::ResizeClient(DWORD dwWidth, DWORD  dwHeight)
{
    RECT    wndRect;

    wndRect.top    = this->m_InflatingRect.top;
    wndRect.bottom = dwHeight - this->m_InflatingRect.bottom; // - this->m_InflatingRect.top;
    wndRect.left   = this->m_InflatingRect.left;
    wndRect.right  = dwWidth - this->m_InflatingRect.right; //this->m_InflatingRect.left;

}

MSG_HANDLER_FUNCTION(CMainWindow, WM_USERCALLBACK)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    return S_OK;
}


MSG_HANDLER_FUNCTION(CMainWindow, WM_MOUSEMOVE)
{
    UNREFERENCED_PARAMETER(wParam);
    POINT pt;
	
    pt.x = (short)LOWORD(lParam);
    pt.y = (short)HIWORD(lParam);

    this->m_bMouseCaptured = TRUE;

    CSharedEnvironment *pEnvironment = this->m_pGameEngine->GetEnvironment();

    pEnvironment->LockExclusive();

    pEnvironment->m_gfxInfo.SetMousePos(pt.x, pt.y, this->m_bMouseCaptured);

    pEnvironment->Unlock();

    while (::ShowCursor(FALSE) >= 0);

    return S_OK;
}

MSG_HANDLER_FUNCTION(CMainWindow, WM_NCMOUSEMOVE)
{
    UNREFERENCED_PARAMETER(wParam);
    POINT pt;
	
    pt.x = (short)LOWORD(lParam);
    pt.y = (short)HIWORD(lParam);

    this->m_bMouseCaptured = FALSE;

    CSharedEnvironment *pEnvironment = this->m_pGameEngine->GetEnvironment();

    pEnvironment->LockExclusive();

    pEnvironment->m_gfxInfo.SetMousePos(pt.x, pt.y, this->m_bMouseCaptured);

    pEnvironment->Unlock();

    while (::ShowCursor(TRUE) < 0);

    return S_OK;
}

MSG_HANDLER_FUNCTION(CMainWindow, WM_LBUTTONDOWN)
{
    UNREFERENCED_PARAMETER(wParam);

    POINT pt;
	
    pt.x = (short)LOWORD(lParam);
    pt.y = (short)HIWORD(lParam);

    CSharedEnvironment *pEnvironment = this->m_pGameEngine->GetEnvironment();

    pEnvironment->LockExclusive();

    pEnvironment->m_gfxInfo.SetMousePos(pt.x, pt.y, this->m_bMouseCaptured);
    pEnvironment->SetMouseLDown(TRUE);

    pEnvironment->Unlock();

    return S_OK;
}

MSG_HANDLER_FUNCTION(CMainWindow, WM_LBUTTONUP)
{
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(wParam);

    CSharedEnvironment *pEnvironment = this->m_pGameEngine->GetEnvironment();

    pEnvironment->LockExclusive();

    pEnvironment->SetMouseLDown(FALSE);

    pEnvironment->Unlock();

    return S_OK;
}



LRESULT
CMainWindow::WindowProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
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
    HANDLE_MSG(WM_DESTROY);
    HANDLE_MSG(WM_MOUSEMOVE);
    HANDLE_MSG(WM_NCMOUSEMOVE);
    HANDLE_MSG(WM_LBUTTONDOWN);
    HANDLE_MSG(WM_LBUTTONUP);

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
CMainWindow::WindowSubclass(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CMainWindow *lpThis = reinterpret_cast<CMainWindow *>(
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

#if 0
BOOL
DrawThemeClientEdge(HTHEME hTheme, HWND hWnd, HRGN hRgnUpdate = NULL, HBRUSH hBrush = NULL, int nPartID = 0, int nStateID = 0)
{
    HDC hDC = GetWindowDC(hWnd);

    if (hDC) {
        return FALSE;
    }

    // Get border size
    int cxBorder = GetSystemMetrics(SM_CXBORDER);
    int cyBorder = GetSystemMetrics(SM_CYBORDER);

    if (SUCCEEDED(::GetThemeInt(hTheme, nPartID, nStateID, TMT_SIZINGBORDERWIDTH, &cxBorder)))
        cyBorder = cxBorder;

    RECT rect;
    ::GetWindowRect(hWnd, &rect);

    // Remove the client edge from the update region
    int cxEdge = GetSystemMetrics(SM_CXEDGE);
    int cyEdge = GetSystemMetrics(SM_CYEDGE);
    ::InflateRect(&rect, -cxEdge, -cyEdge);

    HRGN rgn = CreateRectRgnIndirect(&rect);

    if(rgn == NULL) {
        ReleaseDC(hWnd, hDC);
        return FALSE;
    }

    if(hRgnUpdate != NULL)
        CombineRgn(rgn, hRgnUpdate, rgn, RGN_AND);

    ::OffsetRect(&rect, -rect.left, -rect.top);

    ::OffsetRect(&rect, cxEdge, cyEdge);
    dc.ExcludeClipRect(&rect);
    ::InflateRect(&rect, cxEdge, cyEdge);

    ::DrawThemeBackground(hTheme, dc, nPartID, nStateID, &rect, NULL);

    // Use background brush too, since theme border might not cover everything
    if(cxBorder < cxEdge && cyBorder < cyEdge)
    {
        if(hBrush == NULL)
// need conditional code because types don't match in winuser.h
#ifdef _WIN64
            hBrush = (HBRUSH)::GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
#else
            hBrush = (HBRUSH)UlongToPtr(::GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND));
#endif

        ::InflateRect(&rect, cxBorder - cxEdge, cyBorder - cyEdge);
        //dc.FillRect(&rect, hBrush);
    }

    //::DefWindowProc(hWnd, WM_NCPAINT, (WPARAM)rgn.m_hRgn, 0L);

    return TRUE;
}

#endif // if 0
