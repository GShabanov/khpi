#pragma once

#include "Tabs.h"
#include "..\..\Shared\MFCNotify.h"


// CFourthTab dialog
class CFourthTab : public CTabTemplate, public CMFCNotify
{
private:


    class CGraphControl* m_graphCtrl;

    CButton              m_ResetButton;

    CArray<CGraph::GRAPH_POINT>  m_TestPoints;


    void DispayTestData();
    // Construction
public:
    CFourthTab(CSolver<double>* solver);    // standard constructor
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


    // Implementation
protected:

    virtual void ControlCallback(CWnd* from, void* data);

    // Generated message map functions
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
};
