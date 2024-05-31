
// Sample4.h : main header file for the Sample1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSample3App:
// See Sample3.cpp for the implementation of this class
//

class CSample5App : public CWinApp
{
public:
    CSample5App() noexcept;

protected:

    DWORD     m_dwTicksCount;

// Overrides
protected:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
    virtual BOOL OnIdle(LONG lCount); // return TRUE if more idle processing


// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSample5App theApp;
