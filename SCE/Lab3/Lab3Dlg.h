
// Lab3Dlg.h : header file
//

#pragma once

#include "Lab3impl.h"

// CLab3Dlg dialog
class CLab3Dlg : public CDialogEx
{
// Construction
public:
    CLab3Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_LAB2_DIALOG };
#endif

private:

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    CTabCtrl     m_selection;

    class CTabTemplate*		m_CurrentTab;

    //
    // solver class
    //
    CSolver<double>         m_solver;


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};
