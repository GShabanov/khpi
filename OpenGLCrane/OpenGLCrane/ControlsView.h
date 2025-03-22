
#pragma once
#ifndef __CONTROLSWND_H__
#define __CONTROLSWND_H__
#include "controlState.h"

class CMySliderCtrl : public CSliderCtrl
{
private:
    CWnd* m_callbackWnd;

protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
    CMySliderCtrl(CWnd *callbackWnd)
        : CSliderCtrl()
        , m_callbackWnd(callbackWnd)
    {
    }

};

class CControlsWnd : public CWnd, public CStateComponent
{
// Construction
protected:
    DECLARE_DYNCREATE(CControlsWnd)

public:
    CControlsWnd() noexcept;

    void AdjustLayout();

// Attributes
public:
    void SetVSDotNetLook(BOOL bSet)
    {
    }

protected:
    CFont           m_fntPropList;
    CStatic         m_FirstAngle;
    CStatic         m_SecondAngle;
    CMySliderCtrl   m_FirstChain;
    CMySliderCtrl   m_SecondChain;

    virtual void OnSetAngle(int num, float angle);
// Implementation
public:
    virtual ~CControlsWnd();

protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg void OnParentNotify(UINT message, LPARAM lParam);
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()

    void InitControlsWnd();
    void SetPropListFont();

    int m_nComboHeight;
};

#endif // __CONTROLSWND_H__