// 2dSample.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "2dSample.h"
#include <math.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT OnCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT OnPaint(HWND, UINT, WPARAM, LPARAM);
LRESULT OnCommand(HWND, UINT, WPARAM, LPARAM);

HWND hButtonCW;
HWND hButtonCCW;

float  angle = 0.0;


int APIENTRY
wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
    )
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2DSAMPLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2DSAMPLE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM
MyRegisterClass(HINSTANCE hInstance)
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2DSAMPLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2DSAMPLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL
InitInstance(HINSTANCE hInstance, int nCmdShow)
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT
OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    hButtonCW = 
        CreateWindow(_T("BUTTON"), _T("CW"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 20, 100, 20, hWnd, NULL, hInst, NULL);

    hButtonCCW =
        CreateWindow(_T("BUTTON"), _T("CCW"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 40, 100, 20, hWnd, NULL, hInst, NULL);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT
OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    //
    // змінюємо кут
    //
    if ((HWND)lParam == hButtonCW)
    {
        angle += 0.1f;

        if (angle > 6.28f)
        {
            angle = 0;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }

    //
    // змінюємо кут
    //
    if ((HWND)lParam == hButtonCCW)
    {
        angle -= 0.1f;

        if (angle < 0.0f)
        {
            angle = 6.28f;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }

    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;

    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT
OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    float radius = 100.0f;
    LONG  baseX = 200;
    LONG  baseY = 200;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);


    //
    // початкова точка
    //
    MoveToEx(hdc, (int)baseX, (int)baseY, (LPPOINT)NULL);

    float  dx = radius * cosf(angle);
    float  dy = radius * sinf(angle);

    //
    // перша лінія
    //
    LineTo(hdc, (int)(baseX + dx), (int)(baseY + dy));


    //
    // друга лінія
    //
    LineTo(hdc, (int)(2 * baseX), (int)(2 * baseY));


    EndPaint(hWnd, &ps);

    return 0;
}



LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
{
    switch (message)
    {
    case WM_CREATE:
        return OnCreate(hWnd, message, wParam, lParam);

    case WM_COMMAND:
        return OnCommand(hWnd, message, wParam, lParam);

    case WM_PAINT:
        return OnPaint(hWnd, message, wParam, lParam);

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK
About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//
// Message handler for about box.
//
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
