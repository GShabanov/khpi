#pragma once

#include "Tabs.h"

// SixTab.h : header file
//


// CFifthTab dialog
class CSixTab : public CTabTemplate
{
private:
    

	class CGraphControl* m_graphCtrl;

	// Construction
public:
    CSixTab(const CSolver<double> *solver);	// standard constructor
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


	// Implementation
protected:

	// Generated message map functions
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
