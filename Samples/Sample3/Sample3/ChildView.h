
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "Object.h"
#include "Planet.h"
#include "GraphicScene.h"
#include <list>


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
    CGraphicScene                   m_graphicScene;


// Operations
public:

// Overrides
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
    virtual ~CChildView();

    void ClockTick()
    {


        m_graphicScene.Update(1);
    }

    // Generated message map functions
protected:
    void Draw(CDC& cdc);


    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};

