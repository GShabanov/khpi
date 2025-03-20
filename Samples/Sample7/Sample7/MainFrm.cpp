
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "Sample7.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()


CMainFrame::~CMainFrame()
{
    m_smallFont.DeleteObject();
}

int
CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_smallFont.CreateFont(
        14,                       // nHeight
        0,                        // nWidth
        0,                        // nEscapement
        0,                        // nOrientation
        FW_SEMIBOLD,                // nWeight
        FALSE,                    // bItalic
        FALSE,                    // bUnderline
        0,                        // cStrikeOut
        ANSI_CHARSET,             // nCharSet
        OUT_DEFAULT_PRECIS,       // nOutPrecision
        CLIP_DEFAULT_PRECIS,      // nClipPrecision
        DEFAULT_QUALITY,          // nQuality
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
        _T("Arial"));

    CRect  clRect;

    //theApp.LoadAppLangResourceDLL

    GetWindowRect(&clRect);

    clRect.top += 50;

    // create a view to occupy the client area of the frame
    if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, clRect, this, AFX_IDW_PANE_FIRST, nullptr))
    {
        TRACE0("Failed to create view window\n");
	return -1;
    }

    return 0;
}

BOOL
CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(0);
    return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void
CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void
CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void
CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
    // forward focus to the view window
    m_wndView.SetFocus();
}

void
CMainFrame::OnPaint()
{
    //CWnd::OnPaint();

    PAINTSTRUCT     paintStruct = { 0, };
    CDC* dc;

    CRect  clRect;
    GetClientRect(clRect);

    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {
        
        CBrush brush;
        brush.CreateSysColorBrush(COLOR_WINDOW);


        clRect.bottom = clRect.top + 50;

        dc->FillRect(clRect, &brush);


        HGDIOBJ oldFont = dc->SelectObject(m_smallFont);

        /*clRect.top = 10;
        clRect.bottom = 40;
        clRect.left = clRect.Width() / 2 - (11 * m_note.GetLength() / 3);
        clRect.right = clRect.left + 11 * m_note.GetLength();


        dc->DrawText(m_note, clRect, 0);*/

        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }

}

void
CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (m_hWnd == NULL)
    {
        return;
    }

    CRect  clRect;

    GetClientRect(&clRect);

    CRect  clInvalidate;

    clInvalidate = clRect;
    clInvalidate.bottom = clRect.top + 50;

    InvalidateRect(&clInvalidate, TRUE);


    clRect.top += 50;

    m_wndView.SetWindowPos(this, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);
}

BOOL
CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // let the view have first crack at the command
    if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;


    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
}
