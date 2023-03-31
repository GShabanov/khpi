
//
// CSecondTab.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Lab3.h"
#include <afxdialogex.h>
#include "SecondTab.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CSecondTab::CSecondTab(CSolver<double> *solver)
	: CTabTemplate(solver)
{
}

void CSecondTab::DoDataExchange(CDataExchange* pDX)
{
    CWnd::DoDataExchange(pDX);
    DDX_Control(pDX, 1, m_Teach);
}


BEGIN_MESSAGE_MAP(CSecondTab, CTabTemplate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL
CSecondTab::LoadResource()
{
    BOOL      _return = FALSE;
    HRSRC     hDIBRes;
    HGLOBAL   hDIBMem;
    HGLOBAL   hDIBCopy;
    DWORD     nBmpImageSize = 0;
    IStream* pStream = NULL;

    if ((hDIBRes = ::FindResource(theApp.m_hInstance, MAKEINTRESOURCE(IDR_PERCEPTON), _T("RT_BMP"))) > NULL)
    {

        if ((hDIBMem = ::LoadResource(theApp.m_hInstance, hDIBRes)) > 0)
        {
            if (::LockResource(hDIBMem))
            {

                nBmpImageSize = ::SizeofResource(theApp.m_hInstance, hDIBRes);

                hDIBCopy = ::GlobalAlloc(GMEM_MOVEABLE, nBmpImageSize);

                if (hDIBCopy != NULL)
                {

                    PVOID hgInMem = ::GlobalLock(hDIBCopy);

                    memcpy(hgInMem, hDIBMem, nBmpImageSize);

                    if (nBmpImageSize > 0 && ::CreateStreamOnHGlobal(hDIBCopy, TRUE, &pStream) == S_OK)
                    {

                        HRESULT result = ::OleLoadPicture(
                            pStream,
                            nBmpImageSize,
                            FALSE,
                            IID_IPicture,
                            (LPVOID*)&this->m_pPicture);

                        if (result == S_OK)
                        {

                            _return = TRUE;
                        }

                        pStream->Release();
                    }

                    ::GlobalUnlock(hDIBCopy);

                    ::GlobalFree(hDIBCopy);
                }


                ::FreeResource(hDIBMem);
            }
        }
    }

    return _return;
}

void
CSecondTab::OnTeachClicked()
{
    m_solver->W10(m_W10.GetDouble());
    m_solver->W11(m_W11.GetDouble());
    m_solver->W12(m_W12.GetDouble());

    m_solver->W20(m_W20.GetDouble());
    m_solver->W21(m_W21.GetDouble());
    m_solver->W22(m_W22.GetDouble());

    m_solver->W10_2(m_W10_2.GetDouble());
    m_solver->W11_2(m_W11_2.GetDouble());
    m_solver->W12_2(m_W12_2.GetDouble());

    m_solver->Epoc(m_TeachEpochs.GetInt());
    m_solver->Nu(m_TeachSpeed.GetDouble());

    m_solver->TrainNet();


    m_W10.SetValue(m_solver->W10());
    m_W11.SetValue(m_solver->W11());
    m_W12.SetValue(m_solver->W12());

    m_W20.SetValue(m_solver->W20());
    m_W21.SetValue(m_solver->W21());
    m_W22.SetValue(m_solver->W22());

    m_W10_2.SetValue(m_solver->W10_2());
    m_W11_2.SetValue(m_solver->W11_2());
    m_W12_2.SetValue(m_solver->W12_2());


}

void 
CSecondTab::OnResetClicked()
{
    m_solver->SetDefaults();

    m_W10.SetValue(m_solver->W10());
    m_W11.SetValue(m_solver->W11());
    m_W12.SetValue(m_solver->W12());

    m_W20.SetValue(m_solver->W20());
    m_W21.SetValue(m_solver->W21());
    m_W22.SetValue(m_solver->W22());

    m_W10_2.SetValue(m_solver->W10_2());
    m_W11_2.SetValue(m_solver->W11_2());
    m_W12_2.SetValue(m_solver->W12_2());

    m_TeachEpochs.SetValue(m_solver->Epoc());
    m_TeachSpeed.SetValue(m_solver->Nu());

}

BOOL
CSecondTab::OnCommand(WPARAM wParam, LPARAM lParam)
{
    if ((HWND)lParam == m_Teach.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        OnTeachClicked();
        return TRUE;
    }

    if ((HWND)lParam == m_Reset.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        OnResetClicked();
        return TRUE;
    }

    return __super::OnCommand(wParam, lParam);
}

BOOL
CSecondTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;
    CRect  clRectTemp;
    CString text;

	if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
	{
		return FALSE;
	}

    if (!LoadResource())
        return FALSE;

    // 707 x 323

	this->GetClientRect(&clRect);
    clRectTemp = clRect;

    clRect.bottom = clRect.bottom * 2 / 3 + 50;

    CRect child = clRect;

    child.left = (LONG)(clRect.Width() * 330 / 707);
    child.right = child.left + 45;

    child.top = (LONG)(clRect.bottom * 40 / 323);
    child.bottom = child.top + 20;


    m_W10.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W10.SetFont(&this->m_smallFont, FALSE);
    m_W10.SetLimitText(5);
    m_W10.SetTextColor(RGB(0xFF, 0x20, 0x20));


    child.top = (LONG)(clRect.bottom * 272 / 323);
    child.bottom = child.top + 20;

    m_W20.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W20.SetFont(&this->m_smallFont, FALSE);
    m_W20.SetLimitText(5);
    m_W20.SetTextColor(RGB(0xFF, 0x20, 0x20));

    child.left = (LONG)(clRect.Width() * 208 / 707);
    child.right = child.left + 45;

    child.top = (LONG)(clRect.bottom * 84 / 323);
    child.bottom = child.top + 20;

    m_W11.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W11.SetFont(&this->m_smallFont, FALSE);
    m_W11.SetLimitText(5);
    m_W11.SetTextColor(RGB(0xFF, 0x20, 0x20));

    child.top = (LONG)(clRect.bottom * 256 / 323);
    child.bottom = child.top + 20;

    m_W22.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W22.SetFont(&this->m_smallFont, FALSE);
    m_W22.SetLimitText(5);
    m_W22.SetTextColor(RGB(0xFF, 0x20, 0x20));


    child.left = (LONG)(clRect.Width() * 253 / 707);
    child.right = child.left + 45;

    child.top = (LONG)(clRect.bottom * 144 / 323);
    child.bottom = child.top + 20;

    m_W12.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W12.SetFont(&this->m_smallFont, FALSE);
    m_W12.SetLimitText(5);
    m_W12.SetTextColor(RGB(0xFF, 0x20, 0x20));

    child.top = (LONG)(clRect.bottom * 177 / 323);
    child.bottom = child.top + 20;

    m_W21.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W21.SetFont(&this->m_smallFont, FALSE);
    m_W21.SetLimitText(5);
    m_W21.SetTextColor(RGB(0xFF, 0x20, 0x20));


    //
    // 2
    //
    child.left = (LONG)(clRect.Width() * 466 / 707);
    child.right = child.left + 45;

    child.top = (LONG)(clRect.bottom * 87 / 323);
    child.bottom = child.top + 20;

    m_W11_2.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W11_2.SetFont(&this->m_smallFont, FALSE);
    m_W11_2.SetLimitText(5);
    m_W11_2.SetTextColor(RGB(0xFF, 0x20, 0x20));

    child.top = (LONG)(clRect.bottom * 243 / 323);
    child.bottom = child.top + 20;

    m_W12_2.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W12_2.SetFont(&this->m_smallFont, FALSE);
    m_W12_2.SetLimitText(5);
    m_W12_2.SetTextColor(RGB(0xFF, 0x20, 0x20));


    //
    // 3
    //
    child.left = (LONG)(clRect.Width() * 592 / 707);
    child.right = child.left + 45;

    child.top = (LONG)(clRect.bottom * 81 / 323);
    child.bottom = child.top + 20;

    m_W10_2.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, NULL);
    m_W10_2.SetFont(&this->m_smallFont, FALSE);
    m_W10_2.SetLimitText(5);
    m_W10_2.SetTextColor(RGB(0xFF, 0x20, 0x20));

    clRect = clRectTemp;

    child.top = clRect.bottom * 2 / 3 + 52;
    child.bottom = child.top + 24;
    child.left = clRect.left + 2;
    child.right = child.left + 80;

    m_Teach.Create(_T("Навчання"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_DEFPUSHBUTTON, child, this, NULL);

    CRect  resetRect = child;
    resetRect.top = child.bottom + 2;
    resetRect.bottom = resetRect.top + 24;

    m_Reset.Create(_T("Скинути"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_DEFPUSHBUTTON, resetRect, this, NULL);


    child.left = child.right + 2;
    child.right = child.left + 110;

    child.left = child.right + 2;
    child.right = child.left + 80;


    m_TeachEpochs.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, 1);
    m_TeachEpochs.SetAllowedChars(CEdit2::EDT_NUMS);
    m_TeachEpochs.SetValue(m_solver->Epoc());

    child.top = child.bottom + 2;
    child.bottom = child.top + 24;

    m_TeachSpeed.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, child, this, 1);
    m_TeachSpeed.SetAllowedChars(CEdit2::EDT_PERIOD);
    m_TeachSpeed.SetValue(m_solver->Nu());


    m_W10.SetValue(m_solver->W10());
    m_W11.SetValue(m_solver->W11());
    m_W12.SetValue(m_solver->W12());

    m_W20.SetValue(m_solver->W20());
    m_W21.SetValue(m_solver->W21());
    m_W22.SetValue(m_solver->W22());

    m_W10_2.SetValue(m_solver->W10_2());
    m_W11_2.SetValue(m_solver->W11_2());
    m_W12_2.SetValue(m_solver->W12_2());



	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CSecondTab::OnDestroy()
{
    this->m_pPicture->Release();

	__super::OnDestroy();
}

void
CSecondTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;

    CRect           rect;

    this->GetClientRect(&rect);

    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        if (this->m_pPicture != NULL)
        {

            long hmHeight;
            long hmWidth;
            this->m_pPicture->get_Height(&hmHeight);
            this->m_pPicture->get_Width(&hmWidth);

            this->m_pPicture->Render(
                dc->m_hDC,
                rect.left, rect.top,
                rect.Width(), rect.Height() * 2 / 3 + 50,
                0, hmHeight,
                hmWidth, -hmHeight,
                NULL);

        }

        CRect child;

        child.top = rect.bottom * 2 / 3 + 56;
        child.bottom = child.top + 24;
        child.left = rect.left + 10 + 80;
        child.right = child.left + 110;


        dc->DrawText(CString(_T("Кількість епох")), child, 0);

        child.top = child.bottom + 1;
        child.bottom = child.top + 24;

        dc->DrawText(CString(_T("Швидкість")), child, 0);

        __super::OnPaint();

        this->EndPaint(&paintStruct);
    }


}

void
CSecondTab::OnSize(UINT nType, int cx, int cy)
{
	CRect  clRect;
    CRect  clRectTemp;

    this->GetClientRect(&clRect);

    clRectTemp = clRect;

    clRect.bottom = clRect.bottom * 2 / 3 + 50;

    CRect child = clRect;

    child.left = (LONG)(clRect.Width() * 330 / 707);
    child.right = child.left + 45;

    if (m_W10.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 40 / 323);
        child.bottom = child.top + 20;

        m_W10.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }

    if (m_W20.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 272 / 323);
        child.bottom = child.top + 20;

        m_W20.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }


    child.left = (LONG)(clRect.Width() * 208 / 707);
    child.right = child.left + 45;

    if (m_W11.m_hWnd != NULL)
    {

        child.top = (LONG)(clRect.bottom * 84 / 323);
        child.bottom = child.top + 20;

        m_W11.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);

    }

    if (m_W22.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 256 / 323);
        child.bottom = child.top + 20;

        m_W22.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }


    child.left = (LONG)(clRect.Width() * 253 / 707);
    child.right = child.left + 45;

    if (m_W12.m_hWnd != NULL)
    {

        child.top = (LONG)(clRect.bottom * 144 / 323);
        child.bottom = child.top + 20;

        m_W12.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);

    }

    if (m_W21.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 177 / 323);
        child.bottom = child.top + 20;

        m_W21.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }



    //
    // 2
    //
    child.left = (LONG)(clRect.Width() * 466 / 707);
    child.right = child.left + 45;

    if (m_W11_2.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 87 / 323);
        child.bottom = child.top + 20;

        m_W11_2.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }

    if (m_W12_2.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 243 / 323);
        child.bottom = child.top + 20;

        m_W12_2.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }



    child.left = (LONG)(clRect.Width() * 592 / 707);
    child.right = child.left + 45;

    if (m_W10_2.m_hWnd != NULL)
    {
        child.top = (LONG)(clRect.bottom * 81 / 323);
        child.bottom = child.top + 20;

        m_W10_2.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }


    clRect = clRectTemp;

    child.top = clRect.bottom * 2 / 3 + 52;
    child.bottom = child.top + 24;
    child.left = clRect.left + 2;
    child.right = child.left + 80;

    if (m_Teach.m_hWnd != NULL)
    {
        m_Teach.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }

    CRect  resetRect = child;
    resetRect.top = child.bottom + 2;
    resetRect.bottom = resetRect.top + 24;

    if (m_Reset.m_hWnd != NULL)
    {
        m_Reset.SetWindowPos(this, resetRect.left, resetRect.top, resetRect.Width(), resetRect.Height(), SWP_NOZORDER);
    }


    child.left = child.right + 2;
    child.right = child.left + 110;

    child.left = child.right + 2;
    child.right = child.left + 80;

    if (m_TeachEpochs.m_hWnd != NULL)
    {
        m_TeachEpochs.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }

    child.top = child.bottom + 2;
    child.bottom = child.top + 24;

    if (m_TeachSpeed.m_hWnd != NULL)
    {
        m_TeachSpeed.SetWindowPos(this, child.left, child.top, child.Width(), child.Height(), SWP_NOZORDER);
    }


	__super::OnSize(nType, cx, cy);

}
