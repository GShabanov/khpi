
// Sample7.h : main header file for the Sample1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSample7App:
// See Sample7.cpp for the implementation of this class
//

class CSample7App : public CWinApp
{
public:
    CSample7App() noexcept;

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

extern CSample7App theApp;
