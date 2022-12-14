
//
// CSecondTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "SixTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSixTab dialog

CSixTab::CSixTab(const CSolver<double> *solver)
    : CTabTemplate(solver)
    , m_listCtrl(NULL)
{
}


BEGIN_MESSAGE_MAP(CSixTab, CTabTemplate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CFourthTab message handlers
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

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;

    m_listCtrl = new CListCtrl();

    m_listCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
        clRect, this, NULL);

    m_listCtrl->SetExtendedStyle(LVS_EX_GRIDLINES);


    const CMatrix<double> &checkMatrix = m_solver->getCheckMatrix2();


    m_listCtrl->InsertColumn(0, _T("i"), 0, 30);
    m_listCtrl->InsertColumn(1, _T("X"), 0, 80);
    m_listCtrl->InsertColumn(2, _T("Y"), 0, 80);
    m_listCtrl->InsertColumn(3, _T("X²"), 0, 80);
    m_listCtrl->InsertColumn(4, _T("ŷ"), 0, 80);
    m_listCtrl->InsertColumn(5, _T("|y-ŷ|"), 0, 80);
    m_listCtrl->InsertColumn(6, _T("(y-ŷ)²"), 0, 80);
    m_listCtrl->InsertColumn(7, _T("y-ȳ"), 0, 80);
    m_listCtrl->InsertColumn(8, _T("(y-ȳ)²"), 0, 80);

    for (unsigned i = 0; i < checkMatrix.rows(); i++)
    {
        CString num;
        num.Format(_T("%d"), i + 1);

        m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, i, num, 0, LVIS_SELECTED, 0, 0);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 0));
        m_listCtrl->SetItemText(i, 1, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 1));
        m_listCtrl->SetItemText(i, 2, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 2));
        m_listCtrl->SetItemText(i, 3, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 3));
        m_listCtrl->SetItemText(i, 4, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 4));
        m_listCtrl->SetItemText(i, 5, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 5));
        m_listCtrl->SetItemText(i, 6, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 6));
        m_listCtrl->SetItemText(i, 7, num);

        num.Format(_T("%4.4f"), checkMatrix.operator()(i, 7));
        m_listCtrl->SetItemText(i, 8, num);

    }


    //
    // one empty
    //
    int row = checkMatrix.rows();

    m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, row, L"", 0, LVIS_SELECTED, 0, 0);

    row++;

    //
    // Sums
    //
    text = L"Σ";

    m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, row, text, 0, LVIS_SELECTED, 0, 0);

    text.Format(_T("%4.4f"), checkMatrix.columnSumm(4));
    m_listCtrl->SetItemText(row, 5, text);

    text.Format(_T("%4.4f"), checkMatrix.columnSumm(5));
    m_listCtrl->SetItemText(row, 6, text);

    text.Format(_T("%4.4f"), checkMatrix.columnSumm(6));
    m_listCtrl->SetItemText(row, 7, text);

    text.Format(_T("%4.4f"), checkMatrix.columnSumm(7));
    m_listCtrl->SetItemText(row, 8, text);


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSixTab::OnDestroy()
{
    m_listCtrl->DestroyWindow();
	delete m_listCtrl;

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

        CString         text1 = _T("Перевірити значимість рівняння в цілому, використовуючи ");
        CString         text2 = _T("F-критерій Фішера і обчислюючи залишкову дисперсію.");
        CRect  clRect;
        LONG   topSave;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        clRect.top = 10;
        clRect.bottom = clRect.top + 30;
        clRect.left = 10;
        clRect.right = clRect.left + 660;

        dc->DrawText(text1, clRect, 0);

        clRect.top += 30;
        clRect.bottom = clRect.top + 30;

        dc->DrawText(text2, clRect, 0);


        //-------------------------------------------------------------------------
        //    Data
        dc->SelectObject(m_smallFont);
        //
        // firts
        //
        clRect.top = 320;
        topSave = clRect.top;
        clRect.bottom = clRect.top + 16;
        clRect.left = 10;
        clRect.right = clRect.left + 320;

        const CMatrix<double>& checkMatrix = m_solver->getCheckMatrix2();

        unsigned n = checkMatrix.rows();


        double S_mid2_rest = checkMatrix.columnSumm(5) / (n - 2);
        double S_mid2_y = checkMatrix.columnSumm(7) / (n - 1);
        double F = S_mid2_y / S_mid2_rest;

        CString  text;
        text.Format(L"S̄² зал = %f, S̄² _y = %f, F = %f", S_mid2_rest, S_mid2_y, F);

        dc->DrawText(text, clRect, 0);

        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        if (F > 5.9106)
        {
            text.Format(L"F = %f >> F(9, 8,1%%) = 5.9106", F);
        }
        else
        {
            text.Format(L"F = %f < F(9, 8,1%%) = 5.9106", F);
        }

        dc->DrawText(text, clRect, 0);

        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        text.Format(L"коефіцієнт кореляції");
        dc->DrawText(text, clRect, 0);

        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        double l1 = checkMatrix.columnSumm(5);
        double l2 = checkMatrix.columnSumm(7);
        double l = l1 / l2;

        double R = sqrt(1.0 - (checkMatrix.columnSumm(5) / checkMatrix.columnSumm(7)));
        text.Format(L"R = %f", R);
        dc->DrawText(text, clRect, 0);

        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        if (R > 0.9800)
        {
            text.Format(L"R > r(крит, 2, 1%%) => %f > 0.9800", R);
        }
        else
        {
            text.Format(L"R < r(крит, 2, 1%%) => %f < 0.9800", R);
        }

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

	if (m_listCtrl == NULL)
		return; // Return if window is not created yet.

	GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;



    m_listCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
