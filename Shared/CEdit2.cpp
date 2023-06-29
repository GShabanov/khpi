/***************************************************************************************\
*   File:                                                                               *
*       CEdit2.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#include "pch.h"
#include "CEdit2.h"

//#include "Winuser.h"

BEGIN_MESSAGE_MAP(CEdit2, CEdit)
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_PAINT()
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


void
CEdit2::OnSetFocus(CWnd* pOldWnd)
{
    this->GetWindowText(m_lastText);

    __super::OnSetFocus(pOldWnd);
}

void 
CEdit2::OnKillFocus(CWnd* pNewWnd)
{
    CString   text;
    this->GetWindowText(text);

    if (text.IsEmpty() ||
        text.Right(1) == _T("."))
    {
        this->SetWindowText(m_lastText);
    }

    __super::OnKillFocus(pNewWnd);
}

void
CEdit2::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    UINT allowedChars[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, VK_BACK };

    for (int i = 0; i < ARRAYSIZE(allowedChars); i++)    
    {
        if (nChar == allowedChars[i])
        {
            __super::OnChar(nChar, nRepCnt, nFlags);

            if (m_parentNotify != NULL)
                m_parentNotify->ControlCallback(this, (void *)0);
            break;
        }
    }

    if (m_allowedChars & EDT_SIGN)
    {
        if (nChar == '-')
        {
            __super::OnChar(nChar, nRepCnt, nFlags);

            if (m_parentNotify != NULL)
                m_parentNotify->ControlCallback(this, (void*)0);

            return;
        }
    }

    if (m_allowedChars & EDT_PERIOD)
    {
        if (nChar == '.')
        {
            __super::OnChar(nChar, nRepCnt, nFlags);

            if (m_parentNotify != NULL)
                m_parentNotify->ControlCallback(this, (void*)0);

            return;
        }
    }

}

double
CEdit2::GetDouble()
{
    CString   text;
    this->GetWindowText(text);

    double f = _tstof(text);

    return f;
}

void
CEdit2::SetValue(double value)
{

    CString text;

    text.Format(_T("%2.2f"), value);


    this->SetWindowText(text);
}

unsigned int
CEdit2::GetInt()
{
    CString   text;
    this->GetWindowText(text);


    return _tstol(text);
}

void
CEdit2::SetValue(unsigned int value)
{

    CString text;

    text.Format(_T("%d"), value);


    this->SetWindowText(text);
}

/*HBRUSH
CEdit2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH _return = __super::OnCtlColor(pDC, pWnd, nCtlColor);

    pDC->SetTextColor(RGB(0xFF, 0x20, 0x20));

    return _return;
}*/

HBRUSH
CEdit2::CtlColor(CDC* pDC, UINT nCtlColor)
{

    HBRUSH _return = (HBRUSH)GetStockObject(DC_BRUSH);

    pDC->SetTextColor(m_TextColor);

    return _return;
}

void
CEdit2::OnPaint()
{
    __super::OnPaint();

    /*PAINTSTRUCT     paintStruct = { 0, };
    CDC* dc;


    dc = this->GetDC();

    if (dc != NULL)
    {

        CPen  numberPen(PS_DOT, 1, RGB(0xFF, 0x20, 0x20));

        CPen *oldPen = dc->SelectObject(&numberPen);
        COLORREF oldColor = dc->SetTextColor(RGB(0xFF, 0x20, 0x20));


        dc->SetTextColor(oldColor);


        //dc->SetBkColor(RGB(0x20, 0xFF, 0xFF));

        dc->SelectObject(oldPen);


        this->ReleaseDC(dc);
    }*/


}
