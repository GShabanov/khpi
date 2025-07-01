
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "OpenGLDemo.h"
#include "ChildView.h"
#include <comdef.h>
#include <gdiplus.h>
#include <objidl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
    : m_renderer(this)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_CREATE()
	ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL
CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

int
CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    m_renderer.Init(this);

    this->Invalidate(TRUE);

    return CWnd::OnCreate(lpCreateStruct);
}

void
CChildView::LogMessage(LOG_TYPE logType, const TCHAR* format, va_list va)
{
    //m_mainFrame->LogMessage(logType, format, va);
}

void
CChildView::LogMessage(const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);

    LogMessage(CLogCallback::LOG_GL, format, args);

    va_end(args);
}

void CChildView::LogMessage(LOG_TYPE logType, const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);

    LogMessage(logType, format, args);

    va_end(args);
}


void
CChildView::OnPaint()  // WM_PAINT
{
	CPaintDC dc(this); // device context for painting

    m_renderer.Draw();
	
}

void
CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    m_renderer.SetSize(cx, cy);

    this->Invalidate(FALSE);
}

