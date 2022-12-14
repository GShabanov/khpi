
//
// CSevenTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "SevenTab.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSevenTab dialog

CSevenTab::CSevenTab(const CSolver<double> *solver)
    : CTabTemplate(solver)
    , m_graphCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CSevenTab, CTabTemplate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CSixTab message handlers
BOOL
CSevenTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
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

    CGraph   quadraticDataGraph(CGraph::Type::PixelGraph, CString("квадратична регресія"), false);

    quadraticDataGraph.SetColor(RGB(0xFF, 0xFF, 0x20));
    quadraticDataGraph.SetVisible(true);

    CGraph   cubicDataGraph(CGraph::Type::PixelGraph, CString("кубічна регресія"), false);

    cubicDataGraph.SetColor(RGB(0x20, 0xFF, 0xFF));
    cubicDataGraph.SetVisible(true);

    CArray<CGraph::GRAPH_POINT>  quadratic;
    CArray<CGraph::GRAPH_POINT>  cubic;

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

    const CMatrix<double>& xMatrix1 = m_solver->getXMatrix();
    const CMatrix<double>& xMatrix2 = m_solver->getXMatrix2();

    while (xi < xmax)
    {

        //text.Format(L"Загальний вигляд: ŷ = %f + %f * x + %f * x² + %f * x³", xMatrix(0, 0), xMatrix(1, 0), xMatrix(2, 0), xMatrix(3, 0));

        double yi = xMatrix1(0,0) + xMatrix1(1, 0) * xi + xMatrix1(2, 0) * xi * xi;

        CGraph::GRAPH_POINT  a = { xi,  yi };

        quadratic.Add(a);

        double yi2 = xMatrix2(0, 0) + xMatrix2(1, 0) * xi + xMatrix2(2, 0) * xi * xi + xMatrix2(3, 0) * xi * xi * xi;

        a = { xi,  yi2 };

        cubic.Add(a);

        xi += deltaStep;
    }

    quadraticDataGraph.UpdateValues(quadratic);
    cubicDataGraph.UpdateValues(cubic);

    m_graphCtrl->setAbsoluteAxis(true);
    m_graphCtrl->UpdateChart(0, experimentalDataGraph);
    m_graphCtrl->UpdateChart(1, quadraticDataGraph);
    m_graphCtrl->UpdateChart(2, cubicDataGraph);


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSevenTab::OnDestroy()
{
    m_graphCtrl->DestroyWindow();
	delete m_graphCtrl;

	__super::OnDestroy();
}

void
CSevenTab::OnPaint()
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

        //CString  text;
        //text.Format(L"r = Qxy / sqrt(Qx*Qy)  =  S̄xy / (S̄x*S̄y)  =  sqrt(b1yx * b1xy)");

        //dc->DrawText(text, clRect, 0);




        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}

void
CSevenTab::OnSize(UINT nType, int cx, int cy)
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
