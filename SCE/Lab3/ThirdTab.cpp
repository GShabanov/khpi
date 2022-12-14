
//
// CSecondTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "ThirdTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CThirdTab::CThirdTab(const CSolver<double> *solver)
	: CTabTemplate(solver)
	, m_listCtrlA(NULL)
    , m_listCtrlB(NULL)
    , m_listCtrlX(NULL)
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
    CString text;

	if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
	{
		return FALSE;
	}

	this->GetClientRect(&clRect);

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;

    LONG  width3 = clRect.Width() / 3 - 20;

    clRect.right = width3;


    m_listCtrlA = new CListCtrl();

    m_listCtrlA->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
        clRect, this, NULL);

    m_listCtrlA->SetExtendedStyle(LVS_EX_GRIDLINES);

    clRect.left += width3 + 10;
    clRect.right += width3 + 10;

    m_listCtrlB = new CListCtrl();

    m_listCtrlB->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
        clRect, this, NULL);

    m_listCtrlB->SetExtendedStyle(LVS_EX_GRIDLINES);

    clRect.left += width3 + 10;
    clRect.right += width3 + 10;

    m_listCtrlX = new CListCtrl();

    m_listCtrlX->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
        clRect, this, NULL);

    m_listCtrlX->SetExtendedStyle(LVS_EX_GRIDLINES);


    //
    // A matrix
    //

    const CMatrix<double> &aMatrix = m_solver->getAMatrix();

    for (unsigned col = 0; col < 3; col++)
    {
        m_listCtrlA->InsertColumn(col, CString(" "), 0, 70);
    }

    for (unsigned col = 0; col < 3; col++)
    {
        text.Format(_T("%4.4f"), aMatrix(0, col));
        m_listCtrlA->InsertItem(LVIF_TEXT | LVIF_STATE, col, text, 0, LVIS_SELECTED, 0, 0);
    
        for (unsigned row = 1; row < 3; row++)
        {
            text.Format(_T("%4.4f"), aMatrix(row, col));
            m_listCtrlA->SetItemText(col, row, text);
        }
    }

    //
    // B matrix
    //
    const CMatrix<double>& bMatrix = m_solver->getBMatrix();

    m_listCtrlB->InsertColumn(0, CString(" "), 0, 70);

    for (unsigned row = 0; row < 3; row++)
    {
        text.Format(_T("%4.4f"), bMatrix(row, 0));
        m_listCtrlB->InsertItem(LVIF_TEXT | LVIF_STATE, row, text, 0, LVIS_SELECTED, 0, 0);

    }

    //
    // X matrix
    //
    const CMatrix<double>& xMatrix = m_solver->getXMatrix();

    for (unsigned col = 0; col < 3; col++)
    {
        m_listCtrlX->InsertColumn(col, CString(" "), 0, 70);

    }

    for (unsigned row = 0; row < 3; row++)
    {
        text.Format(_T("%4.4f"), xMatrix(row, 0));
        m_listCtrlX->InsertItem(LVIF_TEXT | LVIF_STATE, row, text, 0, LVIS_SELECTED, 0, 0);

    }


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CThirdTab::OnDestroy()
{
    m_listCtrlA->DestroyWindow();
	delete m_listCtrlA;

    m_listCtrlB->DestroyWindow();
    delete m_listCtrlB;

    m_listCtrlX->DestroyWindow();
    delete m_listCtrlX;

	__super::OnDestroy();
}

void
CThirdTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;
    CString         text = _T("Матриці обчислень для квадратного");


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;
        LONG   topSave;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        LONG  width3 = clRect.Width() / 3 - 20;

        clRect.top = 20;
        clRect.bottom = 50;
        clRect.left = width3;
        clRect.right = clRect.left + 630;

        dc->DrawText(text, clRect, 0);


        clRect.top += 50;
        clRect.bottom = 100;
        clRect.left = width3 / 2;
        dc->DrawText(CString("A"), clRect, 0);

        clRect.left += width3;
        dc->DrawText(CString("B"), clRect, 0);

        clRect.left += width3;
        dc->DrawText(CString("X"), clRect, 0);

        dc->SelectObject(m_smallFont);
        //
        // firts
        //
        clRect.top += 250;
        topSave = clRect.top;
        clRect.bottom = clRect.top + 16;
        clRect.left = 10;
        clRect.right = width3 * 2;

        text.Format(L"Квадратичне рівняння ŷ = a + b*x + c*x²");

        dc->DrawText(text, clRect, 0);

        clRect.top += 30;
        clRect.bottom = clRect.top + 30;

        const CMatrix<double>& xMatrix = m_solver->getXMatrix();

        text.Format(L"Загальний вигляд: ŷ = %f + %f * x + %f * x²", xMatrix(0, 0), xMatrix(1, 0), xMatrix(2, 0));

        dc->DrawText(text, clRect, 0);
        //
        // first
        //
        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}

void
CThirdTab::OnSize(UINT nType, int cx, int cy)
{
	CRect  clRect;

	__super::OnSize(nType, cx, cy);

	if (m_listCtrlA == NULL)
		return; // Return if window is not created yet.

	GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;

    LONG  width3 = clRect.Width() / 3 - 20;

    m_listCtrlA->SetWindowPos(this, clRect.left, clRect.top, width3, clRect.Height(), SWP_NOZORDER);

    m_listCtrlB->SetWindowPos(this, clRect.left + width3 + 10, clRect.top, width3, clRect.Height(), SWP_NOZORDER);

    m_listCtrlX->SetWindowPos(this, clRect.left + width3 + width3 + 20, clRect.top, width3, clRect.Height(), SWP_NOZORDER);

}
