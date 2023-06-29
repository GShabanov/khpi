
// CFirstTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCGraph.h"
#include "ThirdTab.h"
#include "Lab3impl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThirdTab dialog

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


// CFirstTab message handlers
BOOL
CThirdTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;

    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    this->GetClientRect(&clRect);

    clRect.top += 40;
    clRect.right -= 10;

    m_graphCtrl = new CGraphControl();

    m_graphCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        clRect, this);


    //
    // experimental data filling
    //
    m_graphCtrl->setChartName(CString("Похибка навчання"));

    if (m_solver->IsTrained())
    {


        DispayErrors();
    }

    m_graphCtrl->setAbsoluteAxis(true);
    //m_graphCtrl->UpdateChart(1, quadraticDataGraph);
    //m_graphCtrl->UpdateChart(2, cubicDataGraph);


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

        if (!m_solver->IsTrained())
        {

            CRect  clRect;

            HGDIOBJ oldFont = dc->SelectObject(m_bigFont);


            GetClientRect(clRect);

            clRect.top = 5;
            clRect.bottom = 30;
            clRect.left = clRect.Width() / 2 - (17 * sizeof("Мережа не навчена") / 3);
            clRect.right = clRect.left + 500;

            dc->DrawText(CString(_T("Мережа не навчена")), clRect, 0);



            dc->SelectObject(oldFont);
        }

        this->EndPaint(&paintStruct);
    }
}


void
CThirdTab::DispayErrors()
{

    CLinedGraph   errorDataGraph(CString("похибка"), true);
    CLinedGraph   checkErrorDataGraph(CString("похибка в кінці епохи"), true);


    errorDataGraph.SetColor(RGB(0xFF, 0x20, 0x20));
    errorDataGraph.SetVisible(true);

    checkErrorDataGraph.SetColor(RGB(0x22, 0xFF, 0x20));
    checkErrorDataGraph.SetVisible(true);

    CArray<CGraph::GRAPH_POINT>  ErrorData;
    CArray<CGraph::GRAPH_POINT>  checkErrorData;

    for (unsigned int i = 0; i < m_solver->Epoc(); i++)
    {
        CGraph::GRAPH_POINT  a;

        a.x = (double)i;
        a.y = (double)m_solver->m_Error[i];

        ErrorData.Add(a);

        a.x = (double)i;
        a.y = (double)m_solver->m_TestError[i];

        checkErrorData.Add(a);


    }

    errorDataGraph.UpdateValues(ErrorData);
    checkErrorDataGraph.UpdateValues(checkErrorData);

    m_graphCtrl->InsertGraph(0, errorDataGraph);
    m_graphCtrl->InsertGraph(1, checkErrorDataGraph);

}

BOOL
CThirdTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if (m_solver->IsTrained())
    {

        DispayErrors();
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

    clRect.top += 40;
    clRect.right -= 10;


    m_graphCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
