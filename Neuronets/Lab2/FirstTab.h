#pragma once

#include "Tabs.h"


#include "DynamicPicture.h"
#include "Lab2impl.h"

#include "..\..\Shared\MFCGraph.h"
#include "..\..\Shared\CEdit2.h"
#include "..\..\Shared\MFCNotify.h"


// CFirstTab dialog
class CFirstTab : public CTabTemplate, public CSolver<double>::CUpdateCallback, public CMFCNotify
{
private:

    CButton              m_LearnButton;
    CButton              m_ResetButton;
    CButton              m_RestoreButton;

    BOOL                 m_bInPicture;

    CDynamicPicture      m_Satellite;
    CGraphControl        m_Graph;
    CButton              m_Start;
    CButton              m_Reset;
    CEdit2               m_Angle;
    CEdit2               m_Velocity;
    CEdit2               m_Time;

    volatile HANDLE      m_hThread;
    BOOL                 m_bTerminate;


    CArray<CGraph::GRAPH_POINT>  m_angleData;
    CArray<CGraph::GRAPH_POINT>  m_velocityData;
    CArray<CGraph::GRAPH_POINT>  m_momentData;

    CSolver<double>      m_solver;

    //IPicture*            m_pPicture;
    //BITMAPINFO          *m_bmi;
    //void*                m_bmpData;

    void OnSolveClick();

    void OnResetClick();

    static DWORD  WINAPI SolveThread(CFirstTab* lpThis) {

        lpThis->SolveThreadRoutine();
        return 0;
    }

    void  SolveThreadRoutine();


protected:
    virtual bool UpdateCallback(double t, double phi, double omega, double mu);
    virtual void ControlCallback(CWnd* from, void* data);


    // Construction
public:
    CFirstTab(); // standard constructor
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);




    // Implementation
protected:
    // Generated message map functions
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
