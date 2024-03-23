
// Task1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTask1App:
// See Lab1.cpp for the implementation of this class
//

class CTask1App : public CWinApp
{
public:
    CTask1App();

// Overrides
public:
    virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

extern CTask1App theApp;
