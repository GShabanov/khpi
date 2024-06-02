
// Sample7.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Sample7.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSample7App

BEGIN_MESSAGE_MAP(CSample7App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSample7App::OnAppAbout)
END_MESSAGE_MAP()


// CSample7App construction

CSample7App::CSample7App() noexcept
{

    // support Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

    SetAppID(_T("Sample4.AppID.NoVersion"));

    m_dwTicksCount = GetTickCount();
}


CSample7App theApp;


BOOL
CSample7App::OnIdle(LONG lCount)
{


    DWORD    dwCurrentTicks = GetTickCount();
    DWORD    dwDelta;

    if (dwCurrentTicks < m_dwTicksCount)
    {
        dwDelta = m_dwTicksCount - dwCurrentTicks;
    }
    else
    {
        dwDelta = dwCurrentTicks - m_dwTicksCount;
    }

    //
    // one second
    //
    if (dwDelta > 100)
    {
        m_dwTicksCount = dwCurrentTicks;
        //////////////////

        CChildView* pChildView = (CChildView *)((CMainFrame*)m_pMainWnd)->GetChild();

        pChildView->ClockTick();
        pChildView->Invalidate(FALSE);
    }


    return TRUE;
}

// CSample7App initialization

BOOL CSample7App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(277);


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;


	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);


	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CSample7App::ExitInstance()
{
    delete m_pMainWnd;

	return CWinApp::ExitInstance();
}

// CSample7App message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CSample7App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSample7App message handlers



