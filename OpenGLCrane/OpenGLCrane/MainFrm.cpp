﻿
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "OpenGLDemo.h"

#include "MainFrm.h"
#include "ControlsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    //EnableDocking(CBRS_ALIGN_ANY);
    //CDockingManager::SetDockingMode(DT_SMART);


	// create a view to occupy the client area of the frame
	/*if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}*/

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    //m_wndSplitter.
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

void
CMainFrame::OnDestroy()
{
    CWnd* wnd = m_wndSplitter.GetPane(0, 1);
    wnd->DestroyWindow();
    delete wnd;

    wnd = m_wndSplitter.GetPane(0, 0);
    wnd->DestroyWindow();
    delete wnd;

    m_wndSplitter.DestroyWindow();

    CMFCVisualManager::DestroyInstance();
    CFrameWnd::OnDestroy();

}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    CRect rectClient;
    GetClientRect(rectClient);

    m_wndSplitter.CreateStatic(this, 1, 2);
    m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CChildView), CSize(rectClient.Width()  * 4 / 5, 0), pContext);
    m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CControlsWnd), CSize(rectClient.Width() * 1 / 5, 0), pContext);

    CChildView *wnd1 = (CChildView*)m_wndSplitter.GetPane(0, 0);

    if (wnd1 != NULL)
    {
        wnd1->setMediator(&m_mediator);
        m_mediator.subscribe(wnd1);
    }

    CControlsWnd* wnd2 = (CControlsWnd*)m_wndSplitter.GetPane(0, 1);

    if (wnd2 != NULL)
    {
        wnd2->setMediator(&m_mediator);
        m_mediator.subscribe(wnd2);
    }

    return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	/*if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;*/

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

