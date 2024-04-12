/*
*   File: 
*       DefaultWindow.cpp
*
*   Abstract:
*       Base interface for windows classes implementation
*
*   Author: 
*       Gennady Shabanov (gennady.shabanov@gmail.com) 31-Oct-2008
*
*   Revision History:
*/
// $Revision: 132 $
#include "stdafx.h"
#include "resource.h"
#include "DefaultWindow.h"

#undef MAX_LOADSTRING
#define MAX_LOADSTRING 100

#define INT_WND_RECT_WIDTH      640
#define INT_WND_RECT_HEIGHT     480

HMODULE      CDefaultWindow::m_hmUxTheme = 0;
LONG         CDefaultWindow::m_lUxThemeRef = 0;

CDefaultWindow::CDefaultWindow(HINSTANCE hInstance, IN HWND hwndParent, TCHAR *ptClassName)
: m_hInstance(hInstance)
, m_ptClassName(ptClassName)
, m_hwndWnd(NULL)
, m_hwndParent(hwndParent)
, m_ptWindowTitle(NULL)
, m_hkCurrentCursor(NULL)
, m_dwThreadId(::GetCurrentThreadId())
, m_IsAppThemed(NULL)
, m_hwndLostFocus(NULL)
/*++
    Routine:
        ~CDefaultWindow()

    Descrition:
        class constructor
--*/
{
    if (m_hmUxTheme == NULL)
    {
        m_hmUxTheme = LoadLibrary(_T("UxTheme.dll"));
        m_lUxThemeRef = 1;
    }
    else
    {
        m_lUxThemeRef++;
    }

    if (m_hmUxTheme != NULL)
    {

        this->m_IsAppThemed  = reinterpret_cast<_IsAppThemed>(
            ::GetProcAddress(m_hmUxTheme, "IsAppThemed"));

        this->m_OpenThemeData       = reinterpret_cast<_OpenThemeData>(
            ::GetProcAddress(m_hmUxTheme, "OpenThemeData"));

        this->m_GetThemeInt         = reinterpret_cast<_GetThemeInt>(
            ::GetProcAddress(m_hmUxTheme, "GetThemeInt"));

        this->m_GetThemeMargins     = reinterpret_cast<_GetThemeMargins>(
            ::GetProcAddress(m_hmUxTheme, "GetThemeMargins"));

        this->m_GetThemePosition     = reinterpret_cast<_GetThemePosition>(
            ::GetProcAddress(m_hmUxTheme, "GetThemePosition"));

        this->m_GetThemePartSize = reinterpret_cast<_GetThemePartSize>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemePartSize"));

        this->m_GetThemeSysSize = reinterpret_cast<_GetThemeSysSize>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemeSysSize"));

        this->m_GetThemeSysColor = reinterpret_cast<_GetThemeSysColor>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemeSysColor"));

        this->m_GetThemeSysFont = reinterpret_cast<_GetThemeSysFont>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemeSysFont"));

        this->m_GetThemeSysInt = reinterpret_cast<_GetThemeSysInt>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemeSysInt"));

        this->m_GetThemeRect = reinterpret_cast<_GetThemeRect>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemeRect"));

        this->m_CloseThemeData      = reinterpret_cast<_CloseThemeData>(
            ::GetProcAddress(this->m_hmUxTheme, "CloseThemeData"));

        this->m_GetThemeSysColor = reinterpret_cast<_GetThemeSysColor>(
            ::GetProcAddress(this->m_hmUxTheme, "GetThemeSysColor"));

        this->m_DrawThemeBackground = reinterpret_cast<_DrawThemeBackground>(
            ::GetProcAddress(this->m_hmUxTheme, "DrawThemeBackground"));

        this->m_DrawThemeText = reinterpret_cast<_DrawThemeText>(
            ::GetProcAddress(this->m_hmUxTheme, "DrawThemeText"));

    }

    memset(&this->m_InflatingRect, 0, sizeof(RECT));
}

BOOL
CDefaultWindow::GetClientRect(LPRECT lpRect)
{
    RECT   cl_rect;

    if (::GetClientRect(this->m_hwndWnd, &cl_rect) != TRUE)
        return FALSE;

    cl_rect.top += this->m_InflatingRect.top;
    cl_rect.bottom -= this->m_InflatingRect.bottom;
    cl_rect.left += this->m_InflatingRect.left;
    cl_rect.right -= this->m_InflatingRect.right;

    *lpRect = cl_rect;

    return TRUE;
}

BOOL
CDefaultWindow::IsAppThemed(VOID)
{
    if (m_hmUxTheme != NULL && m_IsAppThemed != NULL)
        return m_IsAppThemed();
    else
        return FALSE;
}

CDefaultWindow::~CDefaultWindow()
/*++
    Routine:
        ~CDefaultWindow()

    Descrition:
        class destructor
--*/
{

    if (m_hmUxTheme != NULL)
    {
        m_lUxThemeRef--;

        if (m_lUxThemeRef == 0)
        {
            FreeLibrary(m_hmUxTheme);
            m_hmUxTheme = 0;
        }
    }
}

ATOM
CDefaultWindow::RegisterWindowClass(IN TCHAR *ptClassName, IN HINSTANCE hInstance)
/*++
    Routine:
        RegisterWindowClass()

    Descrition:
        Registers tray icon window class.
--*/
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); 

    wcex.style          = NULL; //CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)CDefaultWindow::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor        = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = ptClassName;
    wcex.hIconSm        = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return ::RegisterClassEx(&wcex);
}

BOOL
CDefaultWindow::IsWindowAccessible(HWND hWnd)
{
    if (hWnd == NULL)
        return FALSE;

    LONG style = GetWindowLong(hWnd, GWL_STYLE);

    if (style & WS_TABSTOP)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

HWND
CDefaultWindow::GetNextTabbedWindow(HWND hWnd)
{
    HWND hwndNextFocused = ::GetWindow(hWnd, GW_HWNDNEXT);

    while (IsWindowAccessible(hwndNextFocused) == FALSE)
    {
        if (hwndNextFocused == NULL)
        {
            hwndNextFocused = ::GetWindow(hWnd, GW_HWNDFIRST);
        }
        else
        {
            hwndNextFocused = ::GetWindow(hwndNextFocused, GW_HWNDNEXT);
        }

    }

    return hwndNextFocused;
}

HWND
CDefaultWindow::GetPreviosTabbedWindow(HWND hWnd)
{
    HWND hwndPrevFocused = ::GetWindow(hWnd, GW_HWNDPREV);

    while (IsWindowAccessible(hwndPrevFocused) == FALSE)
    {
        if (hwndPrevFocused == NULL)
        {
            hwndPrevFocused = ::GetWindow(hWnd, GW_HWNDLAST);
        }
        else
        {
            hwndPrevFocused = ::GetWindow(hwndPrevFocused, GW_HWNDPREV);
        }
    }

    return hwndPrevFocused;
}

BOOL
CDefaultWindow::InitWindowInstance(IN HWND hwndParent, IN LPRECT Rect, DWORD dwStyle)
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
    this->m_hkCurrentCursor = ::LoadCursor(NULL, IDC_ARROW);

    if ((Rect == NULL) || 
        ((Rect->left & CW_USEDEFAULT) == CW_USEDEFAULT) || 
        ((Rect->top & CW_USEDEFAULT) == CW_USEDEFAULT))
    {
        this->m_hwndWnd = ::CreateWindowEx(
            NULL, this->m_ptClassName, this->m_ptWindowTitle, 
            dwStyle,
            CW_USEDEFAULT, CW_USEDEFAULT,
            INT_WND_RECT_WIDTH, INT_WND_RECT_HEIGHT,
            hwndParent, NULL, this->m_hInstance, (LPVOID)this);
    }
    else
    {
        this->m_hwndWnd = ::CreateWindowEx(
            NULL, this->m_ptClassName, this->m_ptWindowTitle, 
            dwStyle,
            Rect->left, Rect->top,
            Rect->right - Rect->left, Rect->bottom - Rect->top,
            hwndParent, NULL, this->m_hInstance, (LPVOID)this);
    }

    if (NULL == this->m_hwndWnd)
        return FALSE;


    return TRUE;

}


BOOL
CDefaultWindow::DestroyWindowInstance()
{
    ::SetWindowLong(this->m_hwndWnd, GWLP_USERDATA, (LONG)(LONG_PTR)NULL);
    ::DestroyWindow(this->m_hwndWnd);

    this->m_hwndWnd = NULL;
    return TRUE;
}

LRESULT
CDefaultWindow::WM_THEMECHANGE_Dispatch()
/*++

    Routine:
        WM_THEMECHANGE_Dispatch()

    Descrition:
        change theme dispatch

    Arguments:

--*/
{
    return S_OK;
}

LRESULT CALLBACK 
CDefaultWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
/*++

    Routine:
        WndProc(HWND, unsigned, WORD, LONG)

    Descrition:
        Processes messages for the main window.

    Arguments:
        hWnd     - [in] window to witch call is
        message  - [in] message to this window
        wParam   - [in] high word of a message parameters
        lParam   - [in] low word of a message parameters
--*/
{
    register CDefaultWindow *pWndClass = reinterpret_cast<CDefaultWindow*>(
        (LONG_PTR)::GetWindowLong(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_NCCREATE:
        {
            LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
            pWndClass = static_cast<CDefaultWindow*>(lpcs->lpCreateParams);

#ifdef _WIN64
            ::SetWindowLong(hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)pWndClass);
#else
            ::SetWindowLong(hWnd, GWL_USERDATA, (LONG)(LONG_PTR)pWndClass);
#endif /* _WIN64 */

            pWndClass->m_hwndWnd = hWnd;

        }
        break;

    }

    if (pWndClass != NULL)
    {
        return pWndClass->WindowProc(message, wParam, lParam);
    }
    else
    {
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
}

