
#include "pch.h"
#include "framework.h"

#include "ControlsView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LRESULT
CMySliderCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    DWORD dwPos;    // current position of slider 

    if (message == WM_LBUTTONUP)
    {
        LRESULT result = CSliderCtrl::WindowProc(message, wParam, lParam);

        dwPos = GetPos();

        dwPos = dwPos;

        return result;

    }

    /*if (message != WM_GETDLGCODE &&
        message != 0x363 &&
        message != WM_NCHITTEST &&
        message != WM_SETCURSOR &&
        message != WM_MOUSEFIRST &&
        message != WM_MOUSELEAVE &&
        message != WM_PAINT &&
        message != TBM_GETPOS)
    {
        TRACE(_T("Message: 0x%X\n"), message);
    }*/

    return CSliderCtrl::WindowProc(message, wParam, lParam);
}


/*BOOL
CMySliderCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    DWORD dwPos;    // current position of slider 

    switch (LOWORD(wParam))
    {

    case TB_ENDTRACK:
        dwPos = GetPos();
        break;
    };

    return CSliderCtrl::OnNotify(wParam, lParam, pResult);
}*/


IMPLEMENT_DYNCREATE(CControlsWnd, CWnd)

CControlsWnd::CControlsWnd() noexcept
    : m_FirstChain(this)
    , m_SecondChain(this)
{
    m_nComboHeight = 0;
}

CControlsWnd::~CControlsWnd()
{
    this->m_pMediator->unsubscribe(this);
}

BEGIN_MESSAGE_MAP(CControlsWnd, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CControlsWnd::AdjustLayout()
{
    if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);


    m_FirstAngle.SetWindowPos(nullptr, rectClient.left + 10, rectClient.top + 5, 25, 20, SWP_NOACTIVATE | SWP_NOZORDER);
    m_SecondAngle.SetWindowPos(nullptr, rectClient.left + 40, rectClient.top + 5, 25, 20, SWP_NOACTIVATE | SWP_NOZORDER);


    m_FirstChain.SetWindowPos(nullptr, rectClient.left + 10, rectClient.top + 25, 25, rectClient.Height() - 70, SWP_NOACTIVATE | SWP_NOZORDER);
    m_SecondChain.SetWindowPos(nullptr, rectClient.left + 40, rectClient.top + 25, 25, rectClient.Height() - 70, SWP_NOACTIVATE | SWP_NOZORDER);
}

int
CControlsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create combo:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    InitControlsWnd();


    //AdjustLayout();
    return 0;
}

void
CControlsWnd::OnDestroy()
{
    CWnd::OnDestroy();
}

void
CControlsWnd::OnSetAngle(int num, float angle)
{
    CString  text;
    int degree = (int)glm::degrees(angle);

    text.Format(_T("%d"), degree);

    switch (num)
    {
    case 0:
        m_FirstAngle.SetWindowText(text);
        m_FirstChain.SetPos(degree);
        break;

    case 1:
        m_SecondAngle.SetWindowText(text);
        m_SecondChain.SetPos(degree);
        break;


    default:;
    }

}


void
CControlsWnd::InitControlsWnd()
{
    SetPropListFont();

    CRect rectDummy;
    rectDummy.SetRectEmpty();


    m_FirstAngle.Create(_T("90"), WS_CHILD | WS_VISIBLE | WS_BORDER, rectDummy, this, NULL);
    m_SecondAngle.Create(_T("90"), WS_CHILD | WS_VISIBLE | WS_BORDER, rectDummy, this, NULL);


    m_FirstChain.Create(WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_ENABLESELRANGE | TBS_NOTIFYBEFOREMOVE | TBS_AUTOTICKS, rectDummy, this, NULL);
    m_FirstChain.SetRange(0, 180);
    m_FirstChain.SetTicFreq(1);
    m_FirstChain.SetPos(90);

    m_SecondChain.Create(WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_ENABLESELRANGE | TBS_NOTIFYBEFOREMOVE | TBS_AUTOTICKS, rectDummy, this, NULL);
    m_SecondChain.SetRange(0, 180);
    m_SecondChain.SetTicFreq(1);
    m_SecondChain.SetPos(90);
    //m_SecondChain.SetSelection(0, 80);


}

BOOL
CControlsWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    /*if ((HWND)lParam == m_ExecuteScript.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

        if (pMain == NULL)
            return TRUE;

        if (m_Executing == TRUE)
        {

            return TRUE;

        }
        else
        {
            return TRUE;
        }
    }*/


    return CWnd::OnCommand(wParam, lParam);
}

BOOL
CControlsWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    CString  text;

    CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

    if (pMain == NULL)
        return TRUE;


    NMHDR* hdr = (NMHDR*)lParam;

    CWnd* sender = FromHandle(hdr->hwndFrom);

    if (sender == NULL ||
        sender->GetParent() != this)
        return CWnd::OnNotify(wParam, lParam, pResult);


    if (hdr->hwndFrom == m_FirstChain.GetSafeHwnd() &&
        hdr->code == ((UINT)TRBN_THUMBPOSCHANGING))
    {
        int pos = m_FirstChain.GetPos();
        float radians = glm::radians((float)pos);

        m_pMediator->UpdateAngle(this, 0, radians);

        text.Format(_T("%d"), pos);
        m_FirstAngle.SetWindowText(text);

    }

    if (hdr->hwndFrom == m_SecondChain.GetSafeHwnd() &&
        hdr->code == ((UINT)TRBN_THUMBPOSCHANGING))
    {
        int pos = m_SecondChain.GetPos();
        float radians = glm::radians((float)pos);

        m_pMediator->UpdateAngle(this, 1, radians);

        text.Format(_T("%d"), pos);
        m_SecondAngle.SetWindowText(text);


    }


    return CWnd::OnNotify(wParam, lParam, pResult);
}

void
CControlsWnd::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC* dc;

    CRect  clRect;
    GetClientRect(clRect);

    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CBrush brush;
        brush.CreateSysColorBrush(COLOR_WINDOW);

        dc->FillRect(clRect, &brush);

        this->EndPaint(&paintStruct);
    }

}

void
CControlsWnd::OnParentNotify(UINT message, LPARAM lParam)
{

    /*TCHAR  code[100];

    _stprintf(code, _T("OnParentNotify: Message 0x%X\n"), message);

    OutputDebugString(code);*/

}

void CControlsWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    AdjustLayout();

}

void CControlsWnd::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);
}

void CControlsWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CWnd::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

void CControlsWnd::SetPropListFont()
{
    ::DeleteObject(m_fntPropList.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    m_fntPropList.CreateFontIndirect(&lf);

}
