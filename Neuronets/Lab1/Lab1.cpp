
// Lab1.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Lab1.h"
#include "Lab1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab1App

BEGIN_MESSAGE_MAP(CLab1App, CWinApp)
   ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLab1App construction

CLab1App::CLab1App()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CLab3App object

CLab1App theApp;


// CLab1App initialization

BOOL CLab1App::InitInstance()
{
    CWinApp::InitInstance();


    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager *pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


    CLab1Dlg dlg;
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

