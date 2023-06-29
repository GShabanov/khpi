
/***************************************************************************************\
*   File:                                                                               *
*       MFCPicture.h                                                                    *
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

#ifndef __MFC_PICTURE_H__
#define __MFC_PICTURE_H__


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcontrolbars.h>
#include <vector>

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

//---------------------------------------------------------------------------
//
// PICTURE WORK IMPLEMENTATION
//
//---------------------------------------------------------------------------

template <class TYPE, class ARG_TYPE = const TYPE&>
class CArrayEx : public CArray<TYPE, ARG_TYPE>
{
    // Construction
public:
    CArrayEx()
    {
    }

    CArrayEx(const CArrayEx& x)
    {
        *this = x;
    }

    // Assigment
public:
    CArrayEx& operator = (const CArrayEx& x)
    {
        if (this != &x)
        {
            Copy(x);
        }

        return *this;
    }
};

class CArrayPicture : public CWnd
{
public:
    CArrayPicture();
    ~CArrayPicture();

private:

    CFont                           m_axisFont;
    CDC                             m_memoryDC;
    CBitmap                         m_memoryBitmap;
    LONG                            m_granularity;

    HCURSOR                         m_hDrawCursor;
    HCURSOR                         m_hPrevios;

    BOOL                            m_LButtonPressed;

    CArrayEx<CArrayEx<COLORREF>>    m_InputArray;

    int                             m_cx;
    int                             m_cy;

    void Draw(CDC* cdc);

public:
    virtual BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd);

    int GetWidth() {
        return m_cx;
    }

    int GetHeight() {
        return m_cy;
    }

    void Clear();

    void SetGranularity(LONG  gran = 4) {

        m_granularity = gran;
    }

    void SetInputArray(const CArrayEx<CArrayEx<COLORREF>>& input)
    {
        m_cy = (int)input.GetSize();
        m_cx = (int)input[0].GetSize();

        for (INT_PTR i = 0; i < m_InputArray.GetSize(); i++)
        {
            m_InputArray[i].RemoveAll();
        }

        m_InputArray.RemoveAll();

        m_InputArray.Copy(input);

        this->Invalidate(TRUE);
    }

    void  SetDrawCursor(HCURSOR cursor)
    {
        m_hDrawCursor = cursor;
    }

    CArrayEx<COLORREF>  Serialize();
    


protected:

    //
    // message handlers
    //
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    DECLARE_MESSAGE_MAP()
};

#endif // __MFC_GRAPH_H__