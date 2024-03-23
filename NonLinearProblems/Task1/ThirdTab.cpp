
// CThirdTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCGraph.h"
#include "ThirdTab.h"
#include "Task1impl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSecondTab dialog

CThirdTab::CThirdTab(CSolver<double>* solver)
    : CTabTemplate(solver)
    , m_graphCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CThirdTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CThirdTab message handlers
BOOL
CThirdTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
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
    m_graphCtrl->setHorizontalName(CString("ітерація"));
    m_graphCtrl->setVerticalName(CString("Помилки"));
    m_graphCtrl->setChartName(CString("Графік помилок"));
    m_graphCtrl->setAbsoluteAxis(true);


    if (m_solver->IsInitialized())
    {

        DispayExperimentalData();
    }


    return TRUE;  // return TRUE  unless you set the focus to a control

}

void CThirdTab::OnDestroy()
{
    m_graphCtrl->DestroyWindow();
    delete m_graphCtrl;

    __super::OnDestroy();
}

void
CThirdTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {


        this->EndPaint(&paintStruct);
    }
}


void
CThirdTab::DispayExperimentalData()
{

    CLinedGraph   dataGraph1(CString("помилка x1"), true);
    CLinedGraph   dataGraph2(CString("помилка x2"), true);

    dataGraph1.SetColor(RGB(0xFF, 0x20, 0x20));
    dataGraph1.SetVisible(true);
    dataGraph1.SetThickness(2);

    dataGraph2.SetColor(RGB(0x20, 0xFF, 0x20));
    dataGraph2.SetVisible(true);
    dataGraph2.SetThickness(2);


    CArray<CGraph::GRAPH_POINT>  Datas1;
    CArray<CGraph::GRAPH_POINT>  Datas2;


    for (unsigned i = 0; i < m_solver->m_Error.GetSize(); i++)
    {
        CGraph::GRAPH_POINT  a;

        a.x = (double)i;
        a.y = (double)m_solver->m_Error[i].x1;

        Datas1.Add(a);
    }

    dataGraph1.UpdateValues(Datas1);

    for (unsigned i = 0; i < m_solver->m_Error.GetSize(); i++)
    {
        CGraph::GRAPH_POINT  a;

        a.x = (double)i;
        a.y = (double)m_solver->m_Error[i].x2;

        Datas2.Add(a);
    }

    dataGraph2.UpdateValues(Datas2);

    m_graphCtrl->InsertGraph(0, dataGraph1);
    m_graphCtrl->InsertGraph(1, dataGraph2);
}

BOOL
CThirdTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_InitializeButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        DispayExperimentalData();

    }

    return __super::OnCommand(wParam, lParam);


}


void
CThirdTab::OnSize(UINT nType, int cx, int cy)
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
