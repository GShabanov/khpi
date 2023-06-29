
// CFirstTab.cpp : implementation file
//

#include "pch.h"
#include <afxdialogex.h>
#include "FirstTab.h"
#include "resource.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CFirstTab::CFirstTab()
    : CTabTemplate()
    , m_Angle(this)
    , m_Velocity(this)
    , m_Time(this)
    , m_bInPicture(FALSE)
    , m_hThread(NULL)
    , m_bTerminate(FALSE)
{

    
    m_solver.Initialize(-50, -1, 80);

    m_angleData.SetSize(50 * 10);
    m_velocityData.SetSize(50 * 10);
    m_momentData.SetSize(50 * 10);
}


BEGIN_MESSAGE_MAP(CFirstTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CFirstTab message handlers
BOOL
CFirstTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;

    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    this->GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    /*workRect = clRect;

    workRect.top += 70;
    workRect.right = workRect.left + 5 * 64 + 2;
    workRect.bottom = workRect.top + 5 * 64 + 2;*/

    CRect  pictureRect(clRect.Width() / 3 - 100, 70, clRect.Width() / 3 + 100, 270);


    if (m_Satellite.Create(WS_CHILD | WS_VISIBLE, pictureRect, this) != TRUE)
    {
        this->DestroyWindow();
        return FALSE;
    }

    m_Satellite.SetAngle(m_solver.GetAngle());


    CRect  graphRect(clRect.left + 10, 270, clRect.Width() - 10, clRect.bottom - 10);

    if (m_Graph.Create(WS_CHILD | WS_VISIBLE, graphRect, this) != TRUE)
    {
        this->DestroyWindow();
        return FALSE;
    }

    //
    // experimental data filling
    //
    m_Graph.setHorizontalName(CString("час (t)"));
    m_Graph.setVerticalName(CStringW(L"φ, ω, μ"));
    m_Graph.setChartName(CString("Графік кута, кутової швидкості та приведенного моменту"));

    m_Graph.setHorizontalMax(80.0);
    m_Graph.setAbsoluteAxis(true);


    CRect  buttonRect(clRect.Width() / 2 + 105, 72, clRect.Width() / 2 + 205, 102);

    m_Start.Create(
        _T("Стабілізація"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, buttonRect, this, NULL);

    buttonRect.top = buttonRect.bottom + 2;
    buttonRect.bottom = buttonRect.top + 30;
    
    m_Reset.Create(
        _T("Скинути"), WS_CHILD | WS_VISIBLE, buttonRect, this, NULL);


    buttonRect.top = buttonRect.bottom + 5;
    buttonRect.bottom = buttonRect.top + 20;

    m_Angle.Create(
        WS_CHILD | WS_VISIBLE | ES_CENTER, buttonRect, this, NULL);

    m_Angle.SetTextColor(RGB(0x20, 0xFF, 0x20));

    m_Angle.SetValue(-50.0);


    buttonRect.top = buttonRect.bottom + 2;
    buttonRect.bottom = buttonRect.top + 20;

    m_Velocity.Create(
        WS_CHILD | WS_VISIBLE | ES_CENTER, buttonRect, this, NULL);

    m_Velocity.SetTextColor(RGB(0x20, 0x20, 0xFF));

    m_Velocity.SetValue(-1.0);


    buttonRect.top = buttonRect.bottom + 2;
    buttonRect.bottom = buttonRect.top + 20;

    m_Time.Create(
        WS_CHILD | WS_VISIBLE | ES_CENTER, buttonRect, this, NULL);

    m_Time.SetTextColor(RGB(0x80, 0x80, 0xFF));

    m_Time.SetValue(80.0);


    return TRUE;  // return TRUE  unless you set the focus to a control

}


void
CFirstTab::OnDestroy()
{
    m_bTerminate = TRUE;

    HANDLE hThread = (HANDLE)InterlockedExchangePointer((void**)&this->m_hThread, NULL);

    if (hThread != NULL)
    {

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    m_Graph.DestroyWindow();
    m_Satellite.DestroyWindow();
    __super::OnDestroy();
}


void
CFirstTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;

    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        CRect  textRect = clRect;

        textRect.top = 20;
        textRect.bottom = 70;
        textRect.left = textRect.left = textRect.Width() / 2 - (16 * sizeof("Лабораторна робота 2, Шабанов Г.В. ІКМ-М222К") / 3);
        textRect.right = textRect.left + 500;

        dc->DrawText(CString(_T("Лабораторна робота 2, Шабанов Г.В. ІКМ-М222К")), textRect, 0);

        dc->SelectObject(oldFont);


        CRect  textRect2(clRect.Width() / 2 - 20, 140, clRect.Width() / 2 + 105, 162);

        dc->DrawText(CString(_T("початковий кут")), textRect2, 0);

        textRect2.top = textRect2.bottom + 2;
        textRect2.bottom = textRect2.top + 20;

        dc->DrawText(CString(_T("кутова швикість")), textRect2, 0);

        textRect2.top = textRect2.bottom + 2;
        textRect2.bottom = textRect2.top + 20;

        dc->DrawText(CString(_T("час")), textRect2, 0);

        this->EndPaint(&paintStruct);
    }
}


void
CFirstTab::OnSolveClick()
{
    HANDLE  hThread = CreateThread(
        NULL,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(&CFirstTab::SolveThread),
        this,
        NULL,
        NULL);

    if (hThread != NULL)
    {

        InterlockedExchangePointer((void**)&this->m_hThread, hThread);

        m_Start.EnableWindow(FALSE);
        m_Reset.EnableWindow(FALSE);

    }
}

void
CFirstTab::SolveThreadRoutine()
{
    m_solver.StabilizeAtValue(0.0, this);

    HANDLE hThread = (HANDLE)InterlockedExchangePointer((void**)&this->m_hThread, NULL);

    if (hThread != NULL)
    {

        CloseHandle(hThread);

        m_Reset.EnableWindow(TRUE);
    }
}

bool
CFirstTab::UpdateCallback(double t, double phi, double omega, double mu)
{
    if (m_bTerminate == TRUE)
        return false;

    m_Satellite.SetAngle(phi);

    CGraph::GRAPH_POINT  point;

    point.x = t;
    point.y = phi;

    m_angleData.Add(point);

    CDottedGraph   angleGraph(CStringW(L"кут (φ)"), true);
    angleGraph.SetColor(RGB(0xFF, 0x20, 0x20));
    angleGraph.SetVisible(true);
    angleGraph.SetDotSize(1);

    angleGraph.UpdateValues(m_angleData);

    m_Graph.InsertGraph(0, angleGraph);

    point.x = t;
    point.y = omega;

    m_velocityData.Add(point);

    CDottedGraph   velocityGraph(CStringW(L"кутова швидкість (ω)"), true);
    velocityGraph.SetColor(RGB(0x20, 0xFF, 0x20));
    velocityGraph.SetVisible(true);
    velocityGraph.SetDotSize(1);

    velocityGraph.UpdateValues(m_velocityData);

    m_Graph.InsertGraph(1, velocityGraph);

    point.x = t;
    point.y = mu;

    m_momentData.Add(point);

    CLinedGraph   momentGraph(CStringW(L"керуючий момент (μ)"), true);
    momentGraph.SetColor(RGB(0x80, 0x80, 0xFF));
    momentGraph.SetVisible(true);

    momentGraph.UpdateValues(m_momentData);

    m_Graph.InsertGraph(2, momentGraph);

    Sleep(10);

    return true;
}

void
CFirstTab::ControlCallback(CWnd* from, void* data)
{

    double angle = m_Angle.GetDouble();
    double velocity = m_Velocity.GetDouble();
    double time = m_Time.GetDouble();

    m_solver.Initialize(angle, velocity, time);

    m_Satellite.SetAngle(m_solver.GetAngle());
}

void 
CFirstTab::OnResetClick()
{
    m_solver.Initialize(-50, -1, 80);

    m_Satellite.SetAngle(m_solver.GetAngle());

    m_Angle.SetValue(-50.0);
    m_Velocity.SetValue(-1.0);
    m_Time.SetValue(80.0);

    m_angleData.RemoveAll();
    m_velocityData.RemoveAll();
    m_momentData.RemoveAll();

    m_Graph.DeleteGraph(2);
    m_Graph.DeleteGraph(1);
    m_Graph.DeleteGraph(0);

    m_Start.EnableWindow(TRUE);


}

BOOL
CFirstTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{

    if ((HWND)lParam == m_Start.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        OnSolveClick();
        return TRUE;
    }


    if ((HWND)lParam == m_Reset.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        OnResetClick();
        return TRUE;
    }

    return __super::OnCommand(wParam, lParam);


}


void
CFirstTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;

    __super::OnSize(nType, cx, cy);


    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    if (m_Satellite.m_hWnd != NULL)
    {
        CRect  pictureRect(clRect.Width() / 3 - 100, 70, clRect.Width() / 3 + 100, 270);

        m_Satellite.SetWindowPos(this, pictureRect.left, pictureRect.top, pictureRect.Width(), pictureRect.Height(), SWP_NOZORDER);
    }

    if (m_Graph.m_hWnd != NULL)
    {
        CRect  graphRect(clRect.left + 10, 270, clRect.Width() - 10, clRect.bottom - 10);

        m_Graph.SetWindowPos(this, graphRect.left, graphRect.top, graphRect.Width(), graphRect.Height(), SWP_NOZORDER);
    }

    if (m_Start.m_hWnd != NULL)
    {

        CRect  buttonRect(clRect.Width() / 2 + 105, 72, clRect.Width() / 2 + 205, 102);

        m_Start.SetWindowPos(this, buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), SWP_NOZORDER);

        buttonRect.top = buttonRect.bottom + 2;
        buttonRect.bottom = buttonRect.top + 30;

        m_Reset.SetWindowPos(this, buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), SWP_NOZORDER);

        buttonRect.top = buttonRect.bottom + 5;
        buttonRect.bottom = buttonRect.top + 20;

        m_Angle.SetWindowPos(this, buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), SWP_NOZORDER);

        buttonRect.top = buttonRect.bottom + 2;
        buttonRect.bottom = buttonRect.top + 20;

        m_Velocity.SetWindowPos(this, buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), SWP_NOZORDER);

        buttonRect.top = buttonRect.bottom + 2;
        buttonRect.bottom = buttonRect.top + 20;

        m_Time.SetWindowPos(this, buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), SWP_NOZORDER);

    }


}
