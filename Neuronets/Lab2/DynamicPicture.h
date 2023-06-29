/***************************************************************************************\
*   File:                                                                               *
*       DynamicPicture.h                                                                *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 18-Apr-2023                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#pragma once

#ifndef __DYNAMICPICTURE_H__
#define __DYNAMICPICTURE_H__

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

//---------------------------------------------------------------------------
//
// PICTURE WORK IMPLEMENTATION
//
//---------------------------------------------------------------------------

class CDynamicPicture : public CWnd
{
public:
    CDynamicPicture();
    ~CDynamicPicture();

private:

    CDC                             m_memoryDC;
    CBitmap                         m_memoryBitmap;
    LONG                            m_granularity;

    double                          m_angle;

    HBITMAP                         m_Bitmap;


    int                             m_cx;
    int                             m_cy;

    void Draw(CDC* cdc, CBitmap *bitmap);


    BOOL   LoadImageFile(const CString& filename);

    void DrawTransmogrified(CDC* dc, CBitmap* bitmap, CPoint& position, long width, long height, double angle);
    void CopyToDc(CDC* dc, CBitmap* bitmap, CRect& rect, void* data);

public:
    virtual BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd);

    void  SetAngle(double angle) {

        m_angle = angle;

        this->Invalidate(FALSE);
    }


protected:

    //
    // message handlers
    //
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    DECLARE_MESSAGE_MAP()
};

#endif // __DYNAMICPICTURE_H__