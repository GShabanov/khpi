// 2dSample.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "2dSample.h"
#include <math.h>

#define MAX_LOADSTRING 100

#ifndef _USE_MATH_DEFINES

#define M_PI       (float)3.14159265358979323846   // pi

#endif // _USE_MATH_DEFINES

float radians(float degrees)
{
    return degrees * static_cast<float>(0.01745329251994329576923690768489);
}

float _abs(float value) {
    return value < 0.0f ? -value : value;
}

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

HWND hButtonCW1;
HWND hButtonCCW1;
HWND hButtonCW2;
HWND hButtonCCW2;

float  angle1 = radians(290);
float  angle2 = radians(10);

float AB = 25.96f;
float AD = 95.00f;
float BC = 104.33f;
float CD = 29.27f;

float AB2 = AB * AB;
float AD2 = AD * AD;
float BC2 = BC * BC;
float CD2 = CD * CD;



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
    hButtonCW1 = 
        CreateWindow(_T("BUTTON"), _T("CW"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 20, 100, 20, hWnd, NULL, hInst, NULL);

    hButtonCCW1 =
        CreateWindow(_T("BUTTON"), _T("CCW"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 40, 100, 20, hWnd, NULL, hInst, NULL);

    hButtonCW2 =
        CreateWindow(_T("BUTTON"), _T("CW"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 70, 100, 20, hWnd, NULL, hInst, NULL);

    hButtonCCW2 =
        CreateWindow(_T("BUTTON"), _T("CCW"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 90, 100, 20, hWnd, NULL, hInst, NULL);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT
OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    //
    // змінюємо кут 1
    //
    if ((HWND)lParam == hButtonCW1)
    {
        angle1 += 0.1f;

        if (angle1 > 6.28f)
        {
            angle1 = 0;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }

    //
    // змінюємо кут 1
    //
    if ((HWND)lParam == hButtonCCW1)
    {
        angle1 -= 0.1f;

        if (angle1 < 0.0f)
        {
            angle1 = 6.28f;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }

    //
    // змінюємо кут 2
    //
    if ((HWND)lParam == hButtonCW2)
    {
        angle2 += 0.1f;

        if (angle2 > 6.28f)
        {
            angle2 = 0;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }

    //
    // змінюємо кут 2
    //
    if ((HWND)lParam == hButtonCCW2)
    {
        angle2 -= 0.1f;

        if (angle2 < 0.0f)
        {
            angle2 = 6.28f;
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


bool
CalcAngles(float BD2, float angle1, float angle2, float& gamma, float& lambda, float& fi)
{
    float alfa1 = angle1 + angle2;

    float BD = sqrtf(BD2);

    //-----------------------
    // first triangle
    //-----------------------
    float a = (BD2 + AB2 - AD2) / (2 * BD * AB);  // betta1
    float b = (BD2 + AD2 - AB2) / (2 * BD * AD);  // gamma1

    if (a > 1.0 || a < -1.0)
        return false;

    float betta1 = 0.0;
    float gamma1 = 0.0;

    //
    // We calculate the arccosine on the larger side, thus we reduce the calculation error
    //
    if (_abs(a) > _abs(b))
    {
        betta1 = acosf(a);
        gamma1 = M_PI - (alfa1 + betta1);
    }
    else
    {
        gamma1 = acosf(b);
        betta1 = M_PI - (alfa1 + gamma1);
    }


    //-----------------------
    // Second triangle
    //-----------------------
    float gamma2 = 0.0;

    a = (BD2 + CD2 - BC2) / (2 * BD * CD);

    if (a > 1.0 || a < -1.0)
        return false;

    gamma2 = acosf(a);

    gamma = gamma1 + gamma2;



    float betta2 = 0.0;
    //float lambda = 0.0;

    a = (BD2 + BC2 - CD2) / (2 * BD * BC);   // betta2
    b = (BC2 + CD2 - BD2) / (2 * BC * CD);   // lambda

    if (a > 1.0 || a < -1.0)
        return false;


    if (_abs(a) > _abs(b))
    {
        betta2 = acosf(a);
        lambda = M_PI - (gamma2 + betta2);
    }
    else
    {
        lambda = acosf(b);
        betta2 = M_PI - (gamma2 + lambda);
    }

    return true;

    ////////////////
    /*
    float a = (BD2 + AB2 - AD2) / (2 * BD * AB);  // gamma
    float b = (BD2 + AD2 - AB2) / (2 * BD * AD);  // fi

    if (a > 1.0f || a < -1.0f)
    {
        return false;
    }

    float gamma1 = 0.0f;
    float fi1 = 0.0f;

    //
    // We calculate the arccosine on the larger side, thus we reduce the calculation error
    //
    if (_abs(a) > _abs(b))
    {
        gamma1 = acosf(a);
        fi1 = M_PI - (alfa1 + gamma1);
    }
    else
    {
        fi1 = acosf(b);
        gamma1 = M_PI - (alfa1 + fi1);
    }

    //-----------------------
    // Second triangle
    //-----------------------
    float gamma2 = 0.0;

    a = (BD2 + BC2 - CD2) / (2 * BD * BC);

    if (a > 1.0 || a < -1.0)
    {
        return false;
    }

    gamma2 = acosf(a);

    gamma = M_PI - (gamma1 + gamma2);



    float lamda1 = 0.0f;
    float fi2 = 0.0f;

    a = (BD2 + CD2 - BC2) / (2 * BD * CD);   // fi2
    b = (BC2 + CD2 - BD2) / (2 * BC * CD);   // lambda

    if (a > 1.0 || a < -1.0)
        return false;


    if (_abs(a) > _abs(b))
    {
        fi2 = acosf(a);
        lamda1 = M_PI - (gamma2 + fi2);
    }
    else
    {
        lamda1 = acosf(b);
        fi2 = M_PI - (gamma2 + lamda1);
    }


    lambda = (float)(M_PI - lamda1);

    //
    // check if to close
    //
    if ((gamma1 + gamma2) < 0.9)
        return false;


    //
    // Last calculations. We need the outer angles of the triangle.
    //
    fi = M_PI - (fi1 + fi2);*/

    return true;
}

LRESULT
OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //
    // початкова точка
    //
    LONG  baseX = 400;
    LONG  baseY = 400;


    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);


    LOGBRUSH brush;

    brush.lbStyle = BS_SOLID;
    brush.lbColor = RGB(255, 0, 0);
    brush.lbHatch = BS_HATCHED;


    HBRUSH hBrush = CreateBrushIndirect(&brush);

    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    
    Ellipse(hdc, baseX - 5, baseY - 5, baseX + 5, baseY + 5);

    SelectObject(hdc, hOldBrush);

    //
    // перша лінія (стріла)
    //
    MoveToEx(hdc, (int)baseX, (int)baseY, (LPPOINT)NULL);

    float  Dx = AD * cosf(angle1);
    float  Dy = AD * sinf(angle1);

    LONG toX = (LONG)(baseX + Dx * 2);
    LONG toY = (LONG)(baseY + Dy * 2);

    LineTo(hdc, toX, toY);
    Ellipse(hdc, toX - 5, toY - 5, toX + 5, toY + 5);

    //
    // друга лінія (крівошип)
    //
    MoveToEx(hdc, (int)baseX, (int)baseY, (LPPOINT)NULL);

    float  Bx = AB * cosf(angle2);
    float  By = AB * sinf(angle2);

    toX = (LONG)(baseX + Bx * 2);
    toY = (LONG)(baseY + By * 2);

    LineTo(hdc, toX, toY);
    Ellipse(hdc, toX - 5, toY - 5, toX + 5, toY + 5);

    //
    // відстань BD
    // 
    float BD2 = powf(Bx - Dx, 2) + powf(By - Dy, 2);
    float BD = sqrtf(BD2);

    float gamma, fi, lambda;

    if (CalcAngles(BD2, angle1, angle2, gamma, lambda, fi) != true) {
        EndPaint(hWnd, &ps);

        return 0;
    }

    MoveToEx(hdc, (int)(baseX + Bx * 2), (int)(baseY + By * 2), (LPPOINT)NULL);

    float  Cx = Bx + BC * cosf(gamma - angle2);
    float  Cy = By + BC * sinf(gamma - angle2);

    toX = (LONG)(baseX + Cx * 2);
    toY = (LONG)(baseY + Cy * 2);

    LineTo(hdc, toX, toY);
    Ellipse(hdc, toX - 5, toY - 5, toX + 5, toY + 5);


    MoveToEx(hdc, (int)(baseX + Cx * 2), (int)(baseY + Cy * 2), (LPPOINT)NULL);

    //float  D2x = Cx + (CD + 50.0f) * cosf(lambda - (gamma - angle2));
    //float  D2y = Cy + (CD + 50.0f) * sinf(lambda - (gamma - angle2));

    float  D2x = Cx + (CD + 50.0f) * cosf(angle2);
    float  D2y = Cy + (CD + 50.0f) * sinf(angle2);

    LineTo(hdc, (int)(baseX + D2x * 2), (int)(baseY + D2y * 2));


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
