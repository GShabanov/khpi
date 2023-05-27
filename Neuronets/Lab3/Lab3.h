
// Lab3.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLab3App:
// See Lab3.cpp for the implementation of this class
//

class CLab3App : public CWinApp
{
public:
    CLab3App();

// Overrides
public:
    virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

extern CLab3App theApp;
