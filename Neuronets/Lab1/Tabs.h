#pragma once

// CTabTemplate.h : header file
//

#include "Lab1impl.h"


// CTabTemplate default
class CTabTemplate : public CWnd
{

    CTabTemplate();
    // Construction
protected:
    class CFont      m_bigFont;
    class CFont      m_smallFont;
    CSolver<double>* m_solver;

    CTabTemplate(CSolver<double>* solver);  // standard constructor


public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


    // Implementation
protected:

    // Generated message map functions
    DECLARE_MESSAGE_MAP()
};
