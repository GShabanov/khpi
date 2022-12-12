
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
	, m_listCtrl(NULL)
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

    m_listCtrl = new CListCtrl();

    m_listCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
        clRect, this, NULL);

    m_listCtrl->SetExtendedStyle(LVS_EX_GRIDLINES);


    const CMatrix<double> &dataMatrix = m_solver->getCoefficientsMatrix();


    m_listCtrl->InsertColumn(0, _T("i"), 0, 30);
    m_listCtrl->InsertColumn(1, _T("X*X"), 0, 80);
    m_listCtrl->InsertColumn(2, _T("Y*Y"), 0, 80);
    m_listCtrl->InsertColumn(3, _T("X*Y"), 0, 80);
    m_listCtrl->InsertColumn(4, _T("X+Y"), 0, 80);
    m_listCtrl->InsertColumn(5, _T("(X+Y)²"), 0, 80);

    for (unsigned i = 0; i < dataMatrix.rows(); i++)
    {
        CString num;
        num.Format(_T("%d"), i + 1);

        m_listCtrl->InsertItem(LVIF_TEXT | LVIF_STATE, i, num, 0, LVIS_SELECTED, 0, 0);

        num.Format(_T("%4.4f"), dataMatrix.operator()(i, 0));
        m_listCtrl->SetItemText(i, 1, num);

        num.Format(_T("%4.4f"), dataMatrix.operator()(i, 1));
        m_listCtrl->SetItemText(i, 2, num);

        num.Format(_T("%4.4f"), dataMatrix.operator()(i, 2));
        m_listCtrl->SetItemText(i, 3, num);

        num.Format(_T("%4.4f"), dataMatrix.operator()(i, 3));
        m_listCtrl->SetItemText(i, 4, num);

        num.Format(_T("%4.4f"), dataMatrix.operator()(i, 4));
        m_listCtrl->SetItemText(i, 5, num);
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

    text.Format(_T("%4.4f"), m_solver->getSumXX());
    m_listCtrl->SetItemText(row, 1, text);

    text.Format(_T("%4.4f"), m_solver->getSumYY());
    m_listCtrl->SetItemText(row, 2, text);

    text.Format(_T("%4.4f"), m_solver->getSumXY());
    m_listCtrl->SetItemText(row, 3, text);

    text.Format(_T("   "));
    m_listCtrl->SetItemText(row, 4, text);

    text.Format(_T("%4.4f"), m_solver->getSumSqXY());
    m_listCtrl->SetItemText(row, 5, text);


	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CThirdTab::OnDestroy()
{
    m_listCtrl->DestroyWindow();
	delete m_listCtrl;

	__super::OnDestroy();
}

void
CThirdTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;
    CString         text = _T("Перевірка обчислень");


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
        // firts
        //
        clRect.top += 300;
        topSave = clRect.top;
        clRect.bottom = clRect.top + 16;
        clRect.left = 10;
        clRect.right = clRect.left + 320;

        text.Format(L"r = Qxy / sqrt(Qx*Qy) = S̄xy/(S̄x*S̄y) = sqrt(b1yx*b1xy)");

        dc->DrawText(text, clRect, 0);


        //
        // first
        //
        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        // print(f'r={r}={Qxy/sqrt(Qx*Qy)}={s_mid_xy/(s_mid_x*s_mid_y)}={math.sqrt(b1_yx*b1_xy)}')
        text.Format(L"r = %f = %f = %f = %f",
            m_solver->getCorr(), 
            m_solver->getQxy() / sqrt(m_solver->getQx() * m_solver->getQy()),
            m_solver->getSxy_med() / (m_solver->getSx_med() * m_solver->getSy_med()),
            sqrt(m_solver->getY_hat_2() * m_solver->getX_hat_2()));

        dc->DrawText(text, clRect, 0);

        clRect.top += 36;
        clRect.bottom = clRect.top + 16;

        // S̄b0(yx)/S̄b1(yx)=sqrt( (Σx^2)/n
        text.Format(L"S̄b0(yx) / S̄b1(yx) = sqrt( (Σx²)/n )");

        dc->DrawText(text, clRect, 0);

        clRect.top += 18;
        clRect.bottom = clRect.top + 16;

        // S̄b0(yx)/S̄b1(yx)=sqrt( (Σx^2)/n
        text.Format(L"%f = %f", 
            m_solver->getSb0_med() / m_solver->getSb1_med(), 
            sqrt(m_solver->getSumXX() / m_solver->getCoefficientsMatrix().rows()));

        dc->DrawText(text, clRect, 0);


        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}

void
CThirdTab::OnSize(UINT nType, int cx, int cy)
{
	CRect  clRect;

	__super::OnSize(nType, cx, cy);

	if (m_listCtrl == NULL)
		return; // Return if window is not created yet.

	GetClientRect(&clRect);

	//m_selection.SetWindowPos(this, 0, 0, cx, cy - 30, SWP_NOZORDER);
    clRect.DeflateRect(1, 1);

    clRect.top += 100;
    clRect.bottom = clRect.top + 216;



    m_listCtrl->SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}
