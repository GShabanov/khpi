/***************************************************************************************\
*   File:                                                                               *
*       InfoWindow.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       Informational window                                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 18-Apr-2023                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#pragma once

#ifndef __MFC_INFOWINDOW_H__
#define __MFC_INFOWINDOW_H__

class CInfoWindow : public CWnd
{
public:
    CInfoWindow();
    ~CInfoWindow();

private:
    CFont     m_BigFont;
    CFont     m_MedFont;

    INT       m_Number;

public:
    virtual BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd);

    void SetNumber(INT number)
    {
        m_Number = number;
        this->UpdateWindow();
    }

protected:



    //
    // message handlers
    //
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
};

#endif // __MFC_INFOWINDOW_H__
