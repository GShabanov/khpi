
// OpenGLDemo.h : main header file for the OpenGLDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLDemoApp:
// See OpenGLDemo.cpp for the implementation of this class
//

class COpenGLDemoApp : public CWinApp
{
public:
	COpenGLDemoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLDemoApp theApp;
