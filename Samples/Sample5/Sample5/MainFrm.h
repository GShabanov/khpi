
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame() noexcept;
    DEFINE_LN();

protected:
	DECLARE_DYNAMIC(CMainFrame)


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    CWnd* GetChild() {
        return &m_wndView;
    }

protected:  // control bar embedded members
    class CFont      m_smallFont;

    CString           m_note;
	CToolBar          m_wndToolBar;
	CChildView        m_wndView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

};


