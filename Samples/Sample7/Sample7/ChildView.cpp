
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Sample7.h"
#include "MainFrm.h"
#include "ChildView.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
    m_rawPixels = NULL;
    m_State = vid_Run;
    m_scaleFactor = 1.0;

}

CChildView::~CChildView()
{
    m_memoryDC.DeleteDC();
    m_memoryBitmap.DeleteObject();

    if (m_rawPixels != NULL)
    {
        free(m_rawPixels);
    }

}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    //ON_WM_COMMAND()
    ON_WM_SIZE()
END_MESSAGE_MAP()


int CChildView::OnCreate(LPCREATESTRUCT cs)
{
    CWnd::OnCreate(cs);

    CSprite  button;
    CSprite  buttonPushed;
    CSprite  sprite;


    m_graphicScene.Init();



    CRect rectDummy;
    rectDummy.SetRectEmpty();

    sprite.Init(MAKEINTRESOURCE(IDR_EDIT_FONT), _T("RT_JPG"));

    m_EarthTime.Create(sprite, WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_LEFT, rectDummy, this, NULL);


    m_EarthTime.SetWindowText(CString("12/12/2024"));
    m_currentDay = 0;
    m_year = 2024;

    button.Init(MAKEINTRESOURCE(IDR_SCALE_UP), _T("RT_JPG"));
    buttonPushed.Init(MAKEINTRESOURCE(IDR_SCALE_UP_P), _T("RT_JPG"));


    m_Scaleup.Create(button, buttonPushed, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        rectDummy, this, NULL);

    button.Init(MAKEINTRESOURCE(IDR_SCALE_DOWN), _T("RT_JPG"));
    buttonPushed.Init(MAKEINTRESOURCE(IDR_SCALE_DOWN_P), _T("RT_JPG"));

    m_Scaledown.Create(button, buttonPushed, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        rectDummy, this, NULL);



    //
    // control buttons
    //
    button.Init(MAKEINTRESOURCE(IDR_BUTTON_PAUSE), _T("RT_JPG"));
    buttonPushed.Init(MAKEINTRESOURCE(IDR_BUTTON_PAUSE_P), _T("RT_JPG"));

    m_Pause.Create(button, buttonPushed, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        rectDummy, this, NULL);

    button.Init(MAKEINTRESOURCE(IDR_BUTTON_RUN), _T("RT_JPG"));
    buttonPushed.Init(MAKEINTRESOURCE(IDR_BUTTON_RUN_P), _T("RT_JPG"));

    m_Run.Create(button, buttonPushed, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        rectDummy, this, NULL);

    m_Run.SetCheck(BST_CHECKED);

    button.Init(MAKEINTRESOURCE(IDR_BUTTON_FASTER), _T("RT_JPG"));
    buttonPushed.Init(MAKEINTRESOURCE(IDR_BUTTON_FASTER_P), _T("RT_JPG"));

    m_RunFast.Create(button, buttonPushed, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        rectDummy, this, NULL);

    button.Init(MAKEINTRESOURCE(IDR_BUTTON_VFAST), _T("RT_JPG"));
    buttonPushed.Init(MAKEINTRESOURCE(IDR_BUTTON_VFAST_P), _T("RT_JPG"));

    m_RunVeryFast.Create(button, buttonPushed, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        rectDummy, this, NULL);


    return 0;
}

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(/*CS_HREDRAW|CS_VREDRAW|*/CS_DBLCLKS, 
        ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);


    return TRUE;
}


BOOL
CChildView::OnCommand(WPARAM wParam, LPARAM lParam)
{
    if ((HWND)lParam == m_Pause.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_Pause.SetCheck(BST_CHECKED);
        m_Run.SetCheck(BST_UNCHECKED);
        m_RunFast.SetCheck(BST_UNCHECKED);
        m_RunVeryFast.SetCheck(BST_UNCHECKED);
        m_State = vid_Pause;
        return TRUE;
    }


    if ((HWND)lParam == m_Run.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_Pause.SetCheck(BST_UNCHECKED);
        m_Run.SetCheck(BST_CHECKED);
        m_RunFast.SetCheck(BST_UNCHECKED);
        m_RunVeryFast.SetCheck(BST_UNCHECKED);
        m_State = vid_Run;
        return TRUE;
    }

    if ((HWND)lParam == m_RunFast.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_Pause.SetCheck(BST_UNCHECKED);
        m_Run.SetCheck(BST_UNCHECKED);
        m_RunFast.SetCheck(BST_CHECKED);
        m_RunVeryFast.SetCheck(BST_UNCHECKED);
        m_State = vid_RunFast;
        return TRUE;
    }

    if ((HWND)lParam == m_RunVeryFast.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_Pause.SetCheck(BST_UNCHECKED);
        m_Run.SetCheck(BST_UNCHECKED);
        m_RunFast.SetCheck(BST_UNCHECKED);
        m_RunVeryFast.SetCheck(BST_CHECKED);
        m_State = vid_RunVeryFast;
        return TRUE;
    }

    if ((HWND)lParam == m_Scaleup.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_scaleFactor += 0.1;
        if (m_scaleFactor > 8.000000)
            m_scaleFactor = 8.0;

        m_graphicScene.SetScaleFactor(m_scaleFactor);
        return TRUE;
    }

    if ((HWND)lParam == m_Scaledown.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        m_scaleFactor -= 0.1;
        if (m_scaleFactor <= 0.1)
            m_scaleFactor = 0.1;

        m_graphicScene.SetScaleFactor(m_scaleFactor);
        return TRUE;
    }

    return CWnd::OnCommand(wParam, lParam);
}

void
CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    struct
    {
        BITMAPINFO      bi;
        RGBQUAD         zeroColor[2];
    } bmpInfo;

    CRect  clRect;

    if (m_hWnd == NULL)
    {
        return;
    }

    CDC* dc = this->GetDC();

    if (dc == NULL)
        return;


    m_memoryDC.DeleteDC();
    m_memoryBitmap.DeleteObject();

    m_memoryDC.CreateCompatibleDC(dc);

    m_memoryBitmap.CreateCompatibleBitmap(dc, cx, cy);

    CBitmap* oldBitmap = m_memoryDC.SelectObject(&m_memoryBitmap);

    oldBitmap->DeleteObject();

    ReleaseDC(dc);


    //
    // get background bitmap data (current screen color bits)
    //

    memset(&bmpInfo, 0, sizeof(bmpInfo));

    bmpInfo.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bi.bmiHeader.biCompression = BI_RGB;

    if (GetDIBits(m_memoryDC.GetSafeHdc(), (HBITMAP)m_memoryBitmap.GetSafeHandle(), 0, 0, NULL, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {
        return;
    }

    if (m_rawPixels != NULL)
    {
        free(m_rawPixels);
    }

    m_rawPixels = malloc(bmpInfo.bi.bmiHeader.biSizeImage);


    if (this->m_hWnd == NULL)
        return;

    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);


    //
    // center for a move buttons
    //
    CPoint buttons;

    buttons.x = clRect.right - 90;
    buttons.y = clRect.top + 190;



    //
    // rect for buttons
    //
    CRect button;

    button.top = buttons.y - 30;
    button.bottom = button.top + 30;
    button.left = buttons.x;
    button.right = button.left + 30;


    m_Scaleup.SetWindowPos(this, button.left, button.top, button.Width(), button.Height(), SWP_NOZORDER);

    button.top = buttons.y + 30;
    button.bottom = button.top + 30;
    button.left = buttons.x;
    button.right = button.left + 30;

    m_Scaledown.SetWindowPos(this, button.left, button.top, button.Width(), button.Height(), SWP_NOZORDER);


    //
    // center for a buttons
    //
    CPoint edits;

    edits.x = clRect.right - 390;
    edits.y = clRect.top + 30;

    //
    // rect for buttons
    //
    CRect edit;

    edit.top = edits.y;
    edit.bottom = edit.top + 60;
    edit.left = edits.x;
    edit.right = edit.left + 360;

    m_EarthTime.SetWindowPos(this, edit.left, edit.top, edit.Width(), edit.Height(), SWP_NOZORDER);


    //
    // center for a control buttons
    //

    buttons.x = clRect.right - 290;
    buttons.y = clRect.top + 100;

    button.top = buttons.y;
    button.bottom = button.top + 40;
    button.left = buttons.x + 30;
    button.right = button.left + 40;

    m_Pause.SetWindowPos(this, button.left, button.top, button.Width(), button.Height(), SWP_NOZORDER);

    button.left = buttons.x + 70;
    button.right = button.left + 40;

    m_Run.SetWindowPos(this, button.left, button.top, button.Width(), button.Height(), SWP_NOZORDER);

    button.left = buttons.x + 110;
    button.right = button.left + 40;

    m_RunFast.SetWindowPos(this, button.left, button.top, button.Width(), button.Height(), SWP_NOZORDER);

    button.left = buttons.x + 150;
    button.right = button.left + 40;

    m_RunVeryFast.SetWindowPos(this, button.left, button.top, button.Width(), button.Height(), SWP_NOZORDER);


}


void
CChildView::DayNumberToDate(int date, int& day, int& month, int& year)
{
    long y = (10000 * date + 14780) / 3652425;
    long ddd = date - (365 * y + y / 4 - y / 100 + y / 400);
    if (ddd < 0)
    {
        y = y - 1;
        ddd = date - (365 * y + y / 4 - y / 100 + y / 400);
    }

    long mi = (100 * ddd + 52) / 3060;
    long mm = (mi + 2) % 12 + 1;
    y = y + (mi + 2) / 12;
    long dd = ddd - (mi * 306 + 5) / 10 + 1;

    day = dd;
    month = mm;
    year = y;
}

void
CChildView::ClockTick()
{

    // float radToDay = 6.28 / 365; // 0,01720547945205479452054794520548

    if (m_State == vid_Pause)
        return;

    int count = 0;

    if (m_State == vid_Run)
    {
        count = 1;
    }

    if (m_State == vid_RunFast)
    {
        count = 4;
    }

    if (m_State == vid_RunVeryFast)
    {
        count = 8;
    }

    for (int i = 0; i < count; i++)
    {

        m_currentDay++;

        if (m_currentDay > 365)
        {
            m_currentDay = 0;
            m_year++;
        }

        int day;
        int month;
        int year;

        //
        // simple calculation for a day 
        //
        // Calculating months
        //
        month = (m_currentDay) / 30;

        // Calculating days
        day = (m_currentDay - month * 30);

        month += 1;
        day += 1;

        year = m_year;

        CString  text;

        text.Format(_T("%02d/%02d/%04d"), day, month, year);

        m_EarthTime.SetWindowText(text);

        m_graphicScene.Update(0.1);
    }
}


void
CChildView::Draw(CDC& dc)
{
    CRect   worksArrea;

    GetClientRect(&worksArrea);

    CBitmap *pBitmap = dc.GetCurrentBitmap();

    struct
    {
        BITMAPINFO      bi;
        RGBQUAD         zerroColor[2];
    } bmpInfo;


    //
    // get background bitmap data (current screen color bits)
    //

    memset(&bmpInfo, 0, sizeof(bmpInfo));

    bmpInfo.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bi.bmiHeader.biCompression = BI_RGB;

    if (GetDIBits(dc.GetSafeHdc(), (HBITMAP)pBitmap->GetSafeHandle(), 0, 0, NULL, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {
        return;
    }

    LONG  height = bmpInfo.bi.bmiHeader.biHeight;
    LONG  width = bmpInfo.bi.bmiHeader.biWidth;

    bmpInfo.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bi.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bi.bmiHeader.biHeight = -bmpInfo.bi.bmiHeader.biHeight;


    //
    // copy background bits to buffer oldData
    //

    if (GetDIBits(dc.GetSafeHdc(), (HBITMAP)pBitmap->GetSafeHandle(), 0, height, m_rawPixels, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {
        return;
    }

    m_graphicScene.Draw((DWORD *)m_rawPixels, worksArrea, worksArrea);


    m_Scaleup.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);
    m_Scaledown.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);

    m_EarthTime.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);

    m_Pause.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);
    m_Run.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);
    m_RunFast.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);
    m_RunVeryFast.Draw((DWORD*)m_rawPixels, worksArrea, worksArrea);


    for (DWORD i = 0, j = (DWORD)(height * width); i < j; i++)
    {
        DWORD data = *((DWORD*)m_rawPixels + i);

        data = _byteswap_ulong(data);

        data = (data >> 8);

        *((DWORD*)m_rawPixels + i) = data;
    }

    //
    // copy image buffer to DC
    //
    SetDIBitsToDevice(
        dc.GetSafeHdc(),        // handle to DC
        0,                      // x-coord of destination upper-left corner
        0,                      // y-coord of destination upper-left corner 
        width,                  // source rectangle width
        height,                 // source rectangle height
        0,                      // x-coord of source lower-left corner
        0,                      // y-coord of source lower-left corner
        0,                      // first scan line in array
        height,                 // number of scan lines
        ((DWORD*)m_rawPixels),  // array of DIB bits
        &bmpInfo.bi,            // bitmap information
        DIB_RGB_COLORS);        // RGB or palette indexes


}

void CChildView::OnPaint() 
{
    //

    CWnd::OnPaint();

    CClientDC dc(this);

    CRect windowRect;
    this->GetClientRect(&windowRect);

    int  width = windowRect.Width();
    int  height = windowRect.Height();

    //
    // just not try at all to draw something
    //
    if ((width < 10) || (height < 10))
    {
        return;
    }

    //
    // draw in memory
    //
    Draw(m_memoryDC);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &m_memoryDC, 0, 0, SRCCOPY);

    
}

