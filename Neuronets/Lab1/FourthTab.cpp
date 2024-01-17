
// CFirstTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCGraph.h"
#include "FourthTab.h"
#include "Lab1impl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CFourthTab::CFourthTab(CSolver<double>* solver)
    : CTabTemplate(solver)
    , m_graphCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CFourthTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CFirstTab message handlers
BOOL
CFourthTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;

    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    this->GetClientRect(&clRect);

    clRect.top += 40;
    clRect.right -= 110;

    m_graphCtrl = new CGraphControl(this);

    m_graphCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        clRect, this);

    clRect.left = clRect.right + 5;
    clRect.right += 100;
    clRect.bottom = clRect.top + 30;



    m_ResetButton.Create(
        _T("Очистити"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, clRect, this, NULL);

    //
    // experimental data filling
    //
    m_graphCtrl->setHorizontalName(CString("x1"));
    m_graphCtrl->setVerticalName(CString("x2"));
    m_graphCtrl->setChartName(CString("тестові данні"));

    DispayTestData();

    m_graphCtrl->setAbsoluteAxis(true);


    return TRUE;  // return TRUE  unless you set the focus to a control

}

void CFourthTab::OnDestroy()
{
    m_graphCtrl->DestroyWindow();
    delete m_graphCtrl;

    __super::OnDestroy();
}

void
CFourthTab::OnPaint()
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
CFourthTab::DispayTestData()
{


    CArray<CGraph::GRAPH_POINT>  lineData1;
    CArray<CGraph::GRAPH_POINT>  lineData2;

    //lineData1.Add({ -1, -0.5 });
    //lineData1.Add({ 0.5, 1 });
    lineData1.Add({ 0, 0.5 });
    lineData1.Add({ 0.5, 1 });

    CLinedGraph   Line1(CString(""), true);
    CLinedGraph   Line2(CString(""), true);

    Line1.UpdateValues(lineData1);

    //lineData2.Add({ -0.5, -1 });
    //lineData2.Add({ 1, 0.5 });

    lineData2.Add({ 0.5, 0 });
    lineData2.Add({ 1, 0.5 });

    Line2.UpdateValues(lineData2);

    Line1.SetColor(RGB(0x20, 0xFF, 0xFF));
    Line1.SetVisible(true);
    Line2.SetColor(RGB(0x20, 0xFF, 0xFF));
    Line2.SetVisible(true);

    m_graphCtrl->InsertGraph(0, Line1);
    m_graphCtrl->InsertGraph(1, Line2);


    if (m_TestPoints.GetSize() > 0)
    {
        CColorDottedGraph   testDataGraph(CString("тестові дані"), true);

        testDataGraph.SetColor(RGB(0xFF, 0x20, 0x20));
        testDataGraph.SetVisible(true);
        testDataGraph.SetDotSize(2);

        CArray<CGraph::GRAPH_POINT2>  testData;

        for (INT_PTR i = 0; i < m_TestPoints.GetSize(); i++)
        {
            CGraph::GRAPH_POINT2  a;

            a.x = (double)m_TestPoints[i].x;
            a.y = (double)m_TestPoints[i].y;

            //
            // get value using neuronet !!
            //

            bool result = m_solver->CheckValue(a.x, a.y);


            if (result)
            {
                a.color = RGB(0xFF, 0, 0);
            }
            else
            {
                a.color = RGB(0, 0xFF, 0);
            }

            testData.Add(a);
        }

        testDataGraph.UpdateValues(testData);

        m_graphCtrl->InsertGraph(2, testDataGraph);
    }
    else
    {
        m_graphCtrl->DeleteGraph(2);
    }
}

void
CFourthTab::ControlCallback(CWnd* from, void* data)
{
    if (from == m_graphCtrl)
    {
        CGraph::GRAPH_POINT* a = (CGraph::GRAPH_POINT*)data;

        m_TestPoints.Add({ a->x, a->y });

        DispayTestData();
    }
}

BOOL
CFourthTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_ResetButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        m_TestPoints.RemoveAll();
        DispayTestData();

    }

    return __super::OnCommand(wParam, lParam);


}

void
CFourthTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;

    __super::OnSize(nType, cx, cy);

    if (m_graphCtrl == NULL)
        return; // Return if window is not created yet.

    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 40;
    clRect.right -= 110;


    m_graphCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

    clRect.left = clRect.right + 5;
    clRect.right += 100;
    clRect.bottom = clRect.top + 30;

    m_ResetButton.SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
