
// CSecondTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCGraph.h"
#include "SecondTab.h"
#include "Task1impl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSecondTab dialog

CSecondTab::CSecondTab(CSolver<double>* solver)
    : CTabTemplate(solver)
    , m_graphCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CSecondTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CSecondTab message handlers
BOOL
CSecondTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;

    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    this->GetClientRect(&clRect);
    clRect.DeflateRect(1, 1);


    m_graphCtrl = new CGraphControl();

    clRect.right -= 110;

    m_graphCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        clRect, this);

    clRect.left = clRect.right + 1;
    clRect.right += 109;
    clRect.bottom = clRect.top + 30;



    m_InitializeButton.Create(
        _T("Ініціалізувати"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, clRect, this, NULL);

    //
    // experimental data filling
    //
    m_graphCtrl->setHorizontalName(CString("x1"));
    m_graphCtrl->setVerticalName(CString("x2"));
    m_graphCtrl->setChartName(CString("Графіки функцій"));
    m_graphCtrl->setAbsoluteAxis(true);

    m_solver->SetDefaults();
    m_solver->BuildGraph(1.5, 2.5, 0.00001);


    if (m_solver->IsInitialized())
    {

        DispayExperimentalData();
    }


    return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSecondTab::OnDestroy()
{
    m_graphCtrl->DestroyWindow();
    delete m_graphCtrl;

    __super::OnDestroy();
}

void
CSecondTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;

        GetClientRect(clRect);

        clRect.top += 33;
        clRect.bottom = clRect.top + 20;
        clRect.left = clRect.right - 108;
        clRect.right -= 1;

        TCHAR  text[100];
        _stprintf(text, _T("x1 = %f"), m_solver->m_x1);
        dc->DrawText(CString(text), clRect, 0);

        clRect.top += 21;
        clRect.bottom = clRect.top + 20;

        _stprintf(text, _T("x2 = %f"), m_solver->m_x2);
        dc->DrawText(CString(text), clRect, 0);

        clRect.top += 21;
        clRect.bottom = clRect.top + 20;

        _stprintf(text, _T("ітерацій = %d"), (int)m_solver->m_Iterations.GetSize());
        dc->DrawText(CString(text), clRect, 0);


        this->EndPaint(&paintStruct);
    }
}


void
CSecondTab::DispayExperimentalData()
{

    CLinedGraph   dataGraph1(CString("графіки функцій 1"), true);
    CLinedGraph   dataGraph2(CString("графіки функцій 2"), true);
    CLinedGraph   dataGraph3(CString("Крива сходимості"), true);
    CDottedGraph  dataGraph4(CString(""), true);

    dataGraph1.SetColor(RGB(0xFF, 0x20, 0x20));
    dataGraph1.SetVisible(true);
    dataGraph1.SetThickness(2);

    dataGraph2.SetColor(RGB(0x20, 0xFF, 0x20));
    dataGraph2.SetVisible(true);
    dataGraph2.SetThickness(2);

    dataGraph3.SetColor(RGB(0x80, 0x80, 0xFF));
    dataGraph3.SetVisible(true);
    dataGraph3.SetThickness(2);

    dataGraph4.SetColor(RGB(0xFF, 0x20, 0x20));
    dataGraph4.SetVisible(true);
    dataGraph4.SetDotSize(3);


    CArray<CGraph::GRAPH_POINT>  Datas1;
    CArray<CGraph::GRAPH_POINT>  Datas2;
    CArray<CGraph::GRAPH_POINT>  Datas3;


    for (unsigned i = 0; i < ARRAYSIZE(m_solver->m_Function1Data); i++)
    {
        CGraph::GRAPH_POINT  a;

        a.x = (double)m_solver->m_Function1Data[i].x1;
        a.y = (double)m_solver->m_Function1Data[i].x2;

        Datas1.Add(a);
    }

    dataGraph1.UpdateValues(Datas1);

    for (unsigned i = 0; i < ARRAYSIZE(m_solver->m_Function2Data); i++)
    {
        CGraph::GRAPH_POINT  a;

        a.x = (double)m_solver->m_Function2Data[i].x1;
        a.y = (double)m_solver->m_Function2Data[i].x2;

        Datas2.Add(a);
    }

    dataGraph2.UpdateValues(Datas2);

    for (unsigned i = 0; i < m_solver->m_Iterations.GetSize(); i++)
    {
        CGraph::GRAPH_POINT  a;

        a.x = (double)m_solver->m_Iterations[i].x1;
        a.y = (double)m_solver->m_Iterations[i].x2;

        Datas3.Add(a);
    }

    dataGraph3.UpdateValues(Datas3);
    dataGraph4.UpdateValues(Datas3);

    m_graphCtrl->InsertGraph(0, dataGraph1);
    m_graphCtrl->InsertGraph(1, dataGraph2);
    m_graphCtrl->InsertGraph(2, dataGraph3);
    m_graphCtrl->InsertGraph(3, dataGraph4);
}

BOOL
CSecondTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_InitializeButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        m_solver->SetDefaults();
        m_solver->BuildGraph(1.7, 1.8, 0.00001);

        DispayExperimentalData();

    }

    return __super::OnCommand(wParam, lParam);


}


void
CSecondTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;

    __super::OnSize(nType, cx, cy);

    if (m_graphCtrl == NULL)
        return; // Return if window is not created yet.

    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.right -= 110;

    m_graphCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

    clRect.left = clRect.right + 1;
    clRect.right += 109;
    clRect.bottom = clRect.top + 30;

    m_InitializeButton.SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
