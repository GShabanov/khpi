﻿
// Lab4Dlg.cpp : implementation file
//

#include "pch.h"
#include <afxdialogex.h>
#include "Lab4.h"

#include "..\..\Shared\MFCGraph.h"
#include "FirstTab.h"
#include "SecondTab.h"
#include "Lab4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab3Dlg dialog

CLab4Dlg::CLab4Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_LAB4_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab4Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB1, m_selection);
}

BEGIN_MESSAGE_MAP(CLab4Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CLab4Dlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CLab4Dlg message handlers

BOOL CLab4Dlg::OnInitDialog()
{
    TC_ITEM ti;
    CRect    rect;

    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);           // Set small icon

    ti.pszText = _T("Мережа Хопфілда");
    ti.mask = TCIF_TEXT;
    m_selection.InsertItem(0, &ti);

    ti.pszText = _T("Мережа Хеммінга");
    ti.mask = TCIF_TEXT;
    m_selection.InsertItem(1, &ti);


    m_selection.GetClientRect(&rect);

    rect.DeflateRect(2, 2);


    //
    // create first tab
    //
    m_CurrentTab = new CFirstTab();
    m_CurrentTab->Create(WS_BORDER, rect, &m_selection);
    m_CurrentTab->ShowWindow(SW_SHOW);



    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLab4Dlg::OnDestroy()
{
    m_CurrentTab->DestroyWindow();
    delete m_CurrentTab;

    CDialogEx::OnDestroy();
}

void CLab4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLab4Dlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

void
CLab4Dlg::OnSize(UINT nType, int cx, int cy)
{
    CRect  itemRect;
    CRect  clRect;

    CDialogEx::OnSize(nType, cx, cy);

    if (m_selection.m_hWnd == NULL)
        return; // Return if window is not created yet.

    GetClientRect(&clRect);

    //m_selection.SetWindowPos(this, 0, 0, cx, cy - 30, SWP_NOZORDER);

    m_selection.AdjustRect(FALSE, &clRect);
    m_selection.MoveWindow(&clRect, TRUE);
    //m_selection.SetWindowPos(&m_selection, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);


    m_selection.GetItemRect(TabCtrl_GetCurSel(m_selection.m_hWnd), &itemRect);


    //
    // adject for selection
    //
    m_selection.GetClientRect(&clRect);

    clRect.DeflateRect(2, 2);

    clRect.top += itemRect.Height();

    m_CurrentTab->SetWindowPos(&m_selection, clRect.left, clRect.top, clRect.Width(), clRect.Height(), SWP_NOZORDER);

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR
CLab4Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void
CLab4Dlg::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
    RECT rect;
    RECT itemRect;

    if (pNMHDR->hwndFrom == m_selection.m_hWnd)
    {

        m_selection.GetClientRect(&rect);

        int selected = TabCtrl_GetCurSel(m_selection.m_hWnd);

        if (m_selection.GetItemRect(selected, &itemRect) == FALSE)
            return;

        CRect  newRect(1, itemRect.bottom + 1, rect.right - 1, rect.bottom - 3);


        if (m_CurrentTab != NULL)
        {
            m_CurrentTab->ShowWindow(SW_HIDE);
            m_CurrentTab->DestroyWindow();
            delete m_CurrentTab;
        }

        if (selected == 0)
        {

            //
            // create first tab
            //
            m_CurrentTab = new CFirstTab();

        }
        else if (selected == 1)
        {
            //
            // create second tab
            //
            m_CurrentTab = new CSecondTab();


        }
        else
        {
            m_CurrentTab = NULL;
        }

        if (m_CurrentTab != NULL)
        {
            m_CurrentTab->Create(WS_BORDER, newRect, &m_selection);
            m_CurrentTab->ShowWindow(SW_SHOW);
        }

    }

    // TODO: Add your control notification handler code here
    *pResult = 0;
}
