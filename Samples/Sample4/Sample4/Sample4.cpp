
// Sample4.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Sample4.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSample4App

BEGIN_MESSAGE_MAP(CSample4App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSample4App::OnAppAbout)
END_MESSAGE_MAP()


// CSample4App construction

CSample4App::CSample4App() noexcept
{

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

	SetAppID(_T("Sample4.AppID.NoVersion"));

    m_dwTicksCount = GetTickCount();
}


CSample4App theApp;


BOOL
CSample4App::OnIdle(LONG lCount)
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

// CSample4App initialization

BOOL CSample4App::InitInstance()
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

int CSample4App::ExitInstance()
{
	return CWinApp::ExitInstance();
}

// CSample1App message handlers


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
void CSample4App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSample4App message handlers



