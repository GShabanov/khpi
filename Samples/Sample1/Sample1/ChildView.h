
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
    CChildView();

// Attributes
private:
    CDC                             m_memoryDC;
    CBitmap                         m_memoryBitmap;

    double                          m_planet1;

// Operations
public:

// Overrides
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
    virtual ~CChildView();

    // Generated message map functions
protected:
    void DrawCrosshair(CDC& paintDc);
    void Draw(CDC& cdc);


    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};

