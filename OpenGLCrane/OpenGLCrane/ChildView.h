
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "LogWindow.h"
#include "OpenGl\Renderer.h"
#include "OpenGl\MathModel.h"
#include "controlState.h"


// CChildView window

class CChildView : public CWnd, public CStateComponent, public CLogCallback
{
protected:
    DECLARE_DYNCREATE(CChildView)

    CMathModel m_mathModel;
    CRenderer  m_renderer;

    CString* LogFormatMessage(const TCHAR* format, va_list vl);

    virtual void LogMessage(LOG_TYPE logType, const TCHAR* format, va_list va);
    virtual void LogMessage(const TCHAR* format, ...);
    virtual void LogMessage(LOG_TYPE logType, const TCHAR* format, ...);

    virtual void  OnSetAngle(int num, float angle);
    // Construction
public:
	CChildView();


// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

