
// Sample1.h : main header file for the Sample1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSample1App:
// See Sample1.cpp for the implementation of this class
//

class CSample1App : public CWinApp
{
public:
	CSample1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSample1App theApp;
