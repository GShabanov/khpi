#pragma once

#include "Tabs.h"

// SecondTab.h : header file
//
#include "CEdit2.h"

// CSecondTab dialog
class CSecondTab : public CTabTemplate
{
private:
    struct IPicture* m_pPicture;

    //
    // layer 1
    //
    CEdit2            m_W10;
    CEdit2            m_W11;
    CEdit2            m_W12;
    CEdit2            m_W20;
    CEdit2            m_W21;
    CEdit2            m_W22;

    //
    // layer 2
    //
    CEdit2            m_W10_2;
    CEdit2            m_W11_2;
    CEdit2            m_W12_2;

    CButton           m_Teach;
    CButton           m_Reset;
    //CEdit2            m_TeachEpochsText;
    CEdit2            m_TeachEpochs;
    CEdit2            m_TeachSpeed;

    // Construction
    BOOL LoadResource();

public:
    CSecondTab(CSolver<double> *solver);	// standard constructor
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


	// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTeachClicked();   
    afx_msg  void OnResetClicked();


	// Generated message map functions
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
