/***************************************************************************************\
*   File:                                                                               *
*       CEdit2.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#pragma once

#ifndef __CEDIT2_H__
#define __CEDIT2_H__

class CEdit2 : public CEdit
{
public:
    enum ALLOWED_CHARS {

        EDT_NUMS = 1,
        EDT_SIGN = 2,
        EDT_PERIOD = 4
    };


    CString             m_lastText;
    int                 m_allowedChars;

public:

    CEdit2()
        : CEdit()
    {
        m_allowedChars = EDT_NUMS | EDT_SIGN | EDT_PERIOD;
    }

    void SetAllowedChars(int chars = EDT_NUMS | EDT_SIGN | EDT_PERIOD) {

        m_allowedChars = chars;
    }

    double GetDouble();
    void SetValue(double value);

    unsigned int GetInt();
    void SetValue(unsigned int value);

protected:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

};

#endif // __CEDIT2_H__
