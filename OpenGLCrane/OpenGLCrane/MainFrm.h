
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"
#include "controlState.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame() noexcept;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

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

protected:  // control bar embedded members
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	//CChildView        m_wndView;
    CSplitterWndEx    m_wndSplitter;
    CStateMediator    m_mediator;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

};


