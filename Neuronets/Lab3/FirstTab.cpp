
// CFirstTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCGraph.h"
#include "FirstTab.h"
#include "Lab3impl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CFirstTab::CFirstTab(CSolver<double>* solver)
    : CTabTemplate(solver)
    , m_graphCtrl(NULL)
{
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

    clRect.top += 100;
    clRect.right -= 200;

    m_graphCtrl = new CGraphControl();

    m_graphCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        clRect, this);

    clRect.left = clRect.right + 10;
    clRect.right += 110;
    clRect.bottom = clRect.top + 30;



    m_InitializeButton.Create(
        _T("Ініціалізувати"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, clRect, this, NULL);

    //
    // experimental data filling
    //
    m_graphCtrl->setHorizontalName(CString("x1"));
    m_graphCtrl->setVerticalName(CString("x2"));
    m_graphCtrl->setChartName(CString("Еспериментальні данні"));


    m_solver->InitializeTrainingData();

    if (m_solver->IsTrainingData())
    {


        DispayExperimentalData();
    }

    m_graphCtrl->setAbsoluteAxis(true);


    return TRUE;  // return TRUE  unless you set the focus to a control

}

void CFirstTab::OnDestroy()
{
    m_graphCtrl->DestroyWindow();
    delete m_graphCtrl;

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

        clRect.top = 20;
        clRect.bottom = 70;
        clRect.left = clRect.left = clRect.Width() / 2 - (16 * sizeof("Лабораторна робота, Шабанов Г.В. ІКМ-М222К") / 3);
        clRect.right = clRect.left + 500;

        dc->DrawText(CString(_T("Лабораторна робота, Шабанов Г.В. ІКМ-М222К")), clRect, 0);

        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}


void
CFirstTab::DispayExperimentalData()
{

    CColorDottedGraph   experimentalDataGraph(CString("експериментальні дані"), true);

    experimentalDataGraph.SetColor(RGB(0xFF, 0x20, 0x20));
    experimentalDataGraph.SetVisible(true);
    experimentalDataGraph.SetDotSize(2);


    CArray<CGraph::GRAPH_POINT2>  experimentData;

    for (unsigned i = 0; i < ARRAYSIZE(m_solver->m_TrainingData); i++)
    {
        CGraph::GRAPH_POINT2  a;

        a.x = (double)m_solver->m_TrainingData[i].x1;
        a.y = (double)m_solver->m_TrainingData[i].x2;

        if (m_solver->m_TrainingData[i].type == true)
        {
            a.color = RGB(0, 0xFF, 0);
        }
        else
        {
            a.color = RGB(0xFF, 0, 0);
        }

        experimentData.Add(a);
    }

    experimentalDataGraph.UpdateValues(experimentData);

    CLinedGraph   Line1(CString(""), true);
    CLinedGraph   Line2(CString(""), true);

    Line1.SetColor(RGB(0x20, 0xFF, 0xFF));
    Line1.SetVisible(true);
    Line2.SetColor(RGB(0x20, 0xFF, 0xFF));
    Line2.SetVisible(true);


    CArray<CGraph::GRAPH_POINT>  lineData1;
    CArray<CGraph::GRAPH_POINT>  lineData2;

    //lineData1.Add({-1, -0.5});
    //lineData1.Add({0.5, 1});
    lineData1.Add({ 0, 0.5 });
    lineData1.Add({ 0.5, 1 });

    Line1.UpdateValues(lineData1);

    //lineData2.Add({ -0.5, -1 });
    //lineData2.Add({ 1, 0.5 });

    lineData2.Add({ 0.5, 0 });
    lineData2.Add({ 1, 0.5 });

    Line2.UpdateValues(lineData2);

    m_graphCtrl->InsertGraph(0, experimentalDataGraph);
    m_graphCtrl->InsertGraph(1, Line1);
    m_graphCtrl->InsertGraph(2, Line2);
}

BOOL
CFirstTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_InitializeButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        m_solver->InitializeTrainingData();

        DispayExperimentalData();

    }

    return __super::OnCommand(wParam, lParam);


}


void
CFirstTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;

    __super::OnSize(nType, cx, cy);

    if (m_graphCtrl == NULL)
        return; // Return if window is not created yet.

    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 100;
    clRect.right -= 200;


    m_graphCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

    clRect.left = clRect.right + 10;
    clRect.right += 110;
    clRect.bottom = clRect.top + 30;

    m_InitializeButton.SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
