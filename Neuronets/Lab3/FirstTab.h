#pragma once

#include "Tabs.h"


// CFirstTab dialog
class CFirstTab : public CTabTemplate
{
private:


    class CGraphControl* m_graphCtrl;

    CButton              m_InitializeButton;


    void DispayExperimentalData();
	// Construction
public:
	CFirstTab(CSolver<double>* solver);	// standard constructor
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


	// Implementation
protected:

	// Generated message map functions
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
