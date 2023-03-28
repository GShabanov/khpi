
// Lab2.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Lab3.h"
#include "Lab3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab3App

BEGIN_MESSAGE_MAP(CLab3App, CWinApp)
   ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLab3App construction

CLab3App::CLab3App()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CLab3App object

CLab3App theApp;


// CLab3App initialization

BOOL CLab3App::InitInstance()
{
    CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


	CLab3Dlg dlg;
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

