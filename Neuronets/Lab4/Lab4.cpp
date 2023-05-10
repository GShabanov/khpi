
// Lab2.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Lab4.h"
#include "Lab4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab4App

BEGIN_MESSAGE_MAP(CLab4App, CWinApp)
   ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLab3App construction

CLab4App::CLab4App()
{
}


// The one and only CLab3App object

CLab4App theApp;


// CLab4App initialization

BOOL CLab4App::InitInstance()
{
    CWinApp::InitInstance();


    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager *pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


    CLab4Dlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();


    // Delete the shell manager created above.
    if (pShellManager != nullptr)
    {
        delete pShellManager;
    }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
    ControlBarCleanUp();
#endif

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

