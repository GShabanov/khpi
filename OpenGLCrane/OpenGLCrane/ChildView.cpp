
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "OpenGLDemo.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

IMPLEMENT_DYNCREATE(CChildView, CWnd)

CChildView::CChildView()
    : m_renderer(this)
{
}

CChildView::~CChildView()
{
    this->m_pMediator->unsubscribe(this);
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

    m_renderer.Init(this->GetSafeHwnd(), &m_mathModel);

    this->Invalidate(FALSE);

    return CWnd::OnCreate(lpCreateStruct);
}

void
CChildView::OnSetAngle(int num, float angle)
{
    if (num == 0) {
        m_mathModel.setArrowAngle(angle);
    }

    if (num == 1) {
        m_mathModel.setCrankAngle(angle);
    }

    this->Invalidate(FALSE);
}

CString*
CChildView::LogFormatMessage(const TCHAR* format, va_list vl)
{
    TCHAR  stackBuffer[200];
    TCHAR* logBuffer = stackBuffer;

    int chars = _vsctprintf(format, vl);

    if (chars > (sizeof(stackBuffer) / sizeof(TCHAR) + 2))
    {
        logBuffer = (TCHAR*)malloc((chars + 2) * sizeof(TCHAR));
    }

    _vsntprintf(logBuffer, chars, format, vl);

    if (chars < 1)
    {
        if (logBuffer != stackBuffer)
            free(logBuffer);

        return NULL;
    }

    logBuffer[chars] = 0;

    CString* pStr = new CString(logBuffer);

#ifdef _DEBUG
    logBuffer[chars] = _T('\n');
    logBuffer[chars + 1] = 0;
    OutputDebugString(logBuffer);
#endif // _DEBUG


    if (logBuffer != stackBuffer)
        free(logBuffer);

    return pStr;
}

void
CChildView::LogMessage(LOG_TYPE logType, const TCHAR* format, va_list va)
{
    //m_mainFrame->LogMessage(logType, format, va);

    CString  *log = LogFormatMessage(format, va);

    //OutputDebugString(log->GetString());

    delete log;
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
CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    m_renderer.Draw();
	
}

void
CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    m_renderer.SetSize(cx, cy);
}

