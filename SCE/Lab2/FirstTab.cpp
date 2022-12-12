
// CFirstTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include <afxdialogex.h>
#include "FirstTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CFirstTab::CFirstTab(const CSolver<double>* solver)
	: CTabTemplate(solver)
	, m_listCtrl(NULL)
{
    m_InputMatrix = &solver->getInput();
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

    m_listCtrl = new CListCtrl();

    m_listCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
        clRect, this, NULL);

    m_listCtrl->InsertColumn(0, _T("i"), 0, 100);
    m_listCtrl->InsertColumn(1, _T("X"), 0, 100);
    m_listCtrl->InsertColumn(2, _T("Y"), 0, 100);

    for (unsigned i = 0; i < m_InputMatrix->rows(); i++)
    {
        CString num;
        num.Format(_T("%d"), i + 1);

        m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, i, num, 0, LVIS_SELECTED, 0, 0);

        num.Format(_T("%4.4f"), m_InputMatrix->operator()(i, 0));
        m_listCtrl->SetItemText(i, 1, num);

        num.Format(_T("%4.4f"), m_InputMatrix->operator()(i, 1));
        m_listCtrl->SetItemText(i, 2, num);

    }


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CFirstTab::OnDestroy()
{
    m_listCtrl->DestroyWindow();
	delete m_listCtrl;

	__super::OnDestroy();
}

void
CFirstTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;
    CString         text = _T("Завдання");


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        clRect.top = 20;
        clRect.bottom = 70;
        clRect.left = clRect.Width() / 2 - 90;
        clRect.right = clRect.left + 170;

        dc->DrawText(text, clRect, 0);

        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}

void
CFirstTab::OnSize(UINT nType, int cx, int cy)
{
	CRect  clRect;

	__super::OnSize(nType, cx, cy);

	if (m_listCtrl == NULL)
		return; // Return if window is not created yet.

	GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    clRect.top += 100;


    m_listCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
