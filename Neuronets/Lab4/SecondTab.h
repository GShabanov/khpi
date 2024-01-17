#pragma once

#include "Tabs.h"

#include "CListCtrl2.h"
#include "InfoWindow.h"
#include "HemmingNetwork.h"


// CSecondTab dialog
class CSecondTab : 
    public CTabTemplate, 
    protected CNetworkUpdateCallback<double>,
    public CMFCNotify
{
private:

    class CArrayPicture* m_Picture;

    CButton              m_LearnButton;
    CButton              m_ResetButton;
    CListCtrlMy          m_PictureList;
    CInfoWindow          m_InfoWindow;

    CHemmingNetwork<double>  *m_pNetwork;

    ULONG                m_Learned;

    BOOL                 m_bInPicture;

    CArray<CListCtrlMy::DRAW_CONTEXT *>    m_BitmapTable;


    BOOL   LoadTrainingData(const CString &path);

    BOOL   LoadImageFile(const CString &filename);


    // Construction
public:
    CSecondTab(); // standard constructor
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);


    // Implementation
protected:

    virtual void NetworkUpdateCallback(const CArray<double>& output);

    void OnSelectPicture(CListCtrlMy::DRAW_CONTEXT* pContext);
    void OnLearnButton();

    virtual void ControlCallback(CWnd* from, void* data);

    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    // Generated message map functions
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
