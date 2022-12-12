
//
// CSixTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "SixTab.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSixTab dialog

CSixTab::CSixTab(const CSolver<double> *solver)
    : CTabTemplate(solver)
    , m_graphCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CSixTab, CTabTemplate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CSixTab message handlers
BOOL
CSixTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;
    CString text;

	if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
	{
		return FALSE;
	}

	this->GetClientRect(&clRect);

    clRect.top += 70;
    clRect.bottom -= 20;

    m_graphCtrl = new CGraphControl();

    m_graphCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        clRect, this);


    const CMatrix<double> &inputMatrix = m_solver->getInput();
    const CMatrix<double> &dataMatrix = m_solver->getCoefficientsMatrix();


    //
    // experimental data filling
    //
    m_graphCtrl->setChartName(CString("графік лінії регресії і експериментальних даних"));

    CGraph   experimentalDataGraph(CGraph::Type::DottedGraph, CString("експериментальні дані"), true);

    experimentalDataGraph.SetColor(RGB(0xFF, 0x20, 0x20));
    experimentalDataGraph.SetVisible(true);

    CArray<CGraph::GRAPH_POINT>  experimentData;

    for (unsigned i = 0; i < inputMatrix.rows(); i++)
    {
        CGraph::GRAPH_POINT  a = { inputMatrix(i, 0),  inputMatrix(i, 1) };

        experimentData.Add(a);
    }

    experimentalDataGraph.UpdateValues(experimentData);

    //
    // regression data filling
    //

    CGraph   regressionDataGraph(CGraph::Type::PixelGraph, CString("данні регресії"), false);

    regressionDataGraph.SetColor(RGB(0xFF, 0xFF, 0x20));
    regressionDataGraph.SetVisible(true);

    CArray<CGraph::GRAPH_POINT>  regressionData;


    //
    // our delta X
    //
    double  deltaX = inputMatrix.columnMax(0) - inputMatrix.columnMin(0);
    double  deltaStep = deltaX / ((double)inputMatrix.rows() * 20 + 20);

    //
    // offset data for begin
    //
    double xi = inputMatrix.columnMin(0) - deltaStep * 3;
    double xmax = inputMatrix.columnMax(0) + deltaStep * 3;

    while (xi < xmax)
    {
        double yi = xi * m_solver->getB1yx() + m_solver->getB0yx();

        CGraph::GRAPH_POINT  a = { xi,  yi };

        regressionData.Add(a);

        xi += deltaStep;
    }

    regressionDataGraph.UpdateValues(regressionData);

    m_graphCtrl->setAbsoluteAxis(true);
    m_graphCtrl->UpdateChart(0, experimentalDataGraph);
    m_graphCtrl->UpdateChart(1, regressionDataGraph);


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSixTab::OnDestroy()
{
    m_graphCtrl->DestroyWindow();
	delete m_graphCtrl;

	__super::OnDestroy();
}

void
CSixTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CString         text1 = _T("4. Побудувати графік лінії регресії і");
        CString         text2 = _T("   позначити розташування експериментальних даних.");
        CRect  clRect;
        LONG   topSave;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);
        topSave = clRect.top;

        clRect.top = 10;
        clRect.bottom = clRect.top + 30;
        clRect.left = 10;
        clRect.right = clRect.left + 660;

        dc->DrawText(text1, clRect, 0);

        clRect.top += 30;
        clRect.bottom = clRect.top + 30;

        dc->DrawText(text2, clRect, 0);

        topSave = clRect.bottom;

        dc->SelectObject(m_smallFont);


        CRect  graphRect;
        m_graphCtrl->GetClientRect(graphRect);


        clRect.top = topSave + graphRect.Height() + 3;
        clRect.bottom = clRect.top + 16;
        clRect.left = 10;
        clRect.right = clRect.left + 320;

        CString  text;
        text.Format(L"r = Qxy / sqrt(Qx*Qy)  =  S̄xy / (S̄x*S̄y)  =  sqrt(b1yx * b1xy)");

        dc->DrawText(text, clRect, 0);




        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}

void
CSixTab::OnSize(UINT nType, int cx, int cy)
{
	CRect  clRect;

	__super::OnSize(nType, cx, cy);

	if (m_graphCtrl == NULL)
		return; // Return if window is not created yet.

	GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 70;
    clRect.bottom -= 20;



    m_graphCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
