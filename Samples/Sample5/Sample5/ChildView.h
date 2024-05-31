
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "Object.h"
#include "Planet.h"
#include "Sprite.h"
#include "GraphicScene.h"
#include "CGraphicButton.h"
#include "CGraphicEdit.h"
#include <list>


// CChildView window

class CChildView : public CWnd, public CGraphicObject
{
// Construction
public:
    CChildView();

// Attributes
private:
    CGraphicScene                   m_graphicScene;

    CGraphicEdit                    m_EarthTime;
    LONG                            m_currentDay;
    LONG                            m_year;

    CGraphicButton                  m_Up;
    CGraphicButton                  m_Down;
    CGraphicButton                  m_Left;
    CGraphicButton                  m_Right;


// Operations
public:

// Overrides
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    void DayNumberToDate(int date, int& day, int& month, int& year);


// Implementation
public:
    virtual ~CChildView();
    void ClockTick();

    // Generated message map functions
protected:
    void Draw(CDC& cdc);

    afx_msg int OnCreate(LPCREATESTRUCT cs);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};

