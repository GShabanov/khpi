
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
    double                          m_planet2;
    double                          m_planet3;
    double                          m_orbit2Angle;
    double                          m_orbit3Angle;

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
        m_planet1 += 0.05;

        if (m_planet1 >= 6.28)
        {
            m_planet1 = 0.0;
        }

        m_planet2 += 0.08;

        if (m_planet2 >= 6.28)
        {
            m_planet2 = 0.0;
        }

        m_orbit2Angle += 0.01;

        if (m_orbit2Angle >= 6.28)
        {
            m_orbit2Angle = 0.0;
        }

    }

    // Generated message map functions
protected:
    void DrawCrosshair(CDC& paintDc);
    void Draw(CDC& cdc);


    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};

