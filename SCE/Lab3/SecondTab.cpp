
//
// CSecondTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "SecondTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CSecondTab::CSecondTab(const CSolver<double> *solver)
	: CTabTemplate(solver)
	, m_listCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CSecondTab, CTabTemplate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CFirstTab message handlers
BOOL
CSecondTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;
    CString text;

	if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
	{
		return FALSE;
	}

	this->GetClientRect(&clRect);

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;

    m_listCtrl = new CListCtrl();

    m_listCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
        clRect, this, NULL);

    m_listCtrl->SetExtendedStyle(LVS_EX_GRIDLINES);


    const CMatrix<double> &dataMatrix = m_solver->getCoefficientsMatrix();


    m_listCtrl->InsertColumn(0, _T("i"), 0, 30);
    m_listCtrl->InsertColumn(1, _T("X"), 0, 70);
    m_listCtrl->InsertColumn(2, _T("Y"), 0, 70);
    m_listCtrl->InsertColumn(3, _T("X*Y"), 0, 70);
    m_listCtrl->InsertColumn(4, _T("X²"),  0, 70);
    m_listCtrl->InsertColumn(5, _T("Y*X²"), 0, 70);
    m_listCtrl->InsertColumn(6, _T("X³"),   0, 70);
    m_listCtrl->InsertColumn(7, _T("X⁴"),   0, 70);
    m_listCtrl->InsertColumn(8, _T("X⁵"), 0, 70);
    m_listCtrl->InsertColumn(9, _T("X⁶"), 0, 70);

    for (unsigned i = 0; i < dataMatrix.rows(); i++)
    {
        CString num;
        num.Format(_T("%d"), i + 1);

        m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, i, num, 0, LVIS_SELECTED, 0, 0);

        for (unsigned j = 0; j < 9; j++)
        {
            num.Format(_T("%4.4f"), dataMatrix.operator()(i, j));
            m_listCtrl->SetItemText(i, j + 1, num);

        }
    }


    //
    // one empty
    //
    int row = dataMatrix.rows();

    m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, row, L"", 0, LVIS_SELECTED, 0, 0);

    row++;

    //
    // Sums
    //
    text = L"Σ";

    m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, row, text, 0, LVIS_SELECTED, 0, 0);

    for (unsigned j = 0; j < 9; j++)
    {

        text.Format(_T("%4.4f"), dataMatrix.columnSumm(j));
        m_listCtrl->SetItemText(row, j + 1, text);

    }


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSecondTab::OnDestroy()
{
    m_listCtrl->DestroyWindow();
	delete m_listCtrl;

	__super::OnDestroy();
}

void
CSecondTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;
    CString         text = _T("необхідні коєфіцієнти");


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;
        LONG   topSave;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        clRect.top = 20;
        clRect.bottom = 70;
        clRect.left = 20;
        clRect.right = clRect.left + 630;

        dc->DrawText(text, clRect, 0);

        dc->SelectObject(m_smallFont);
        //
        // Y hat
        //
        clRect.top += 300;
        topSave = clRect.top;
        clRect.bottom = clRect.top + 16;
        clRect.left = 10;
        clRect.right = clRect.left + 160;




        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}

void
CSecondTab::OnSize(UINT nType, int cx, int cy)
{
	CRect  clRect;

	__super::OnSize(nType, cx, cy);

	if (m_listCtrl == NULL)
		return; // Return if window is not created yet.

	GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;



    m_listCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
