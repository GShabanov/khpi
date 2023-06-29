/***************************************************************************************\
*   File:                                                                               *
*       MFCPicture.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 18-Apr-2023                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#include "pch.h"
#include "MFCPicture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  CHART_BACKGROUND_COLOR     RGB(0x20, 0x20, 0x20)
#define  CHART_AXIS_COLOR           RGB(0xFF, 0xFF, 0x20)
#define  CHART_GRID_COLOR           RGB(0x50, 0x50, 0x50)
#define  CHART_NAMES_COLOR          RGB(0x50, 0xFF, 0x50)
#define  CHART_GRAPH_COLOR          RGB(0x20, 0x20, 0xFF)
#define  CHART_CROSSPOINT_OFFSET    16
#define  CHART_ARROW_LENGTH         12


//---------------------------------------------------------------------------
//
// CHART CONTROL IMPLEMENTAION
//
//---------------------------------------------------------------------------

// CChartControlApp
BEGIN_MESSAGE_MAP(CArrayPicture, CWnd)
    //{{AFX_MSG_MAP(CGraphControl)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGraphControl construction

CArrayPicture::CArrayPicture()
    : CWnd()
{
    //m_graphs.SetSize(0)
    m_LButtonPressed = FALSE;
    m_granularity = 4;
    m_cx = 64;
    m_cy = 64;

    for (int i = 0; i < m_cy; i++)
    {
        m_InputArray.Add(CArrayEx<COLORREF>());

        for (int j = 0; j < m_cx; j++)
        {

            m_InputArray[i].Add(RGB(0xFF, 0xFF, 0xFF));
        }
    }
}

void CArrayPicture::Clear()
{

    for (int i = 0; i < m_cy; i++)
    {
        m_InputArray.Add(CArrayEx<COLORREF>());

        for (int j = 0; j < m_cx; j++)
        {

            m_InputArray[i].Add(RGB(0xFF, 0xFF, 0xFF));
        }
    }
    this->Invalidate(TRUE);
}

CArrayPicture::~CArrayPicture()
{

}

void
CArrayPicture::OnDestroy()
{
    CWnd::OnDestroy();

    m_memoryBitmap.DeleteObject();
    m_memoryDC.DeleteDC();


    //_ballon.DestroyWindow();
    m_axisFont.DeleteObject();
}

BOOL
CArrayPicture::Create(
    DWORD dwStyle, const CRect& rect, CWnd* pParentWnd)
{


    if (CWnd::Create(NULL, NULL, dwStyle | WS_CHILD, rect, pParentWnd, NULL, NULL) != TRUE)
    {
        return FALSE;
    }

    m_hDrawCursor = GetCursor();  

    m_hPrevios = GetCursor();

    //CRect   ballonRect = rect;

    //ballonRect.InflateRect(ballonRect.Width() / 8, ballonRect.Height() / 8);
    //_ballon.Create(NULL, this);
    //_ballon.ShowWindow(SW_SHOW);

    SetOwner(pParentWnd);

    this->UpdateData(FALSE);

    HGDIOBJ  hDefaultGuiFont = reinterpret_cast<HFONT>(
        ::GetStockObject(DEFAULT_GUI_FONT));

    LOGFONT logfont;
    GetObject(hDefaultGuiFont, sizeof(logfont), &logfont);

    m_axisFont.CreateFontW(-12, 5, 0, 0, FW_THIN, 0, 0, 0, 0, 0, 0, 0,
        DEFAULT_PITCH | FF_DONTCARE, logfont.lfFaceName);

    this->Invalidate(TRUE);

    return TRUE;
}

afx_msg void
CArrayPicture::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    
    if (lpwndpos->cx < (m_cx * (m_granularity + 1)) )
    {
        lpwndpos->cx = lpwndpos->x + (m_cx * (m_granularity + 1));
    }

    if (lpwndpos->cy < (m_cy * (m_granularity + 1)))
    {
        lpwndpos->cy = lpwndpos->y + (m_cy * (m_granularity + 1));
    }

    __super::OnWindowPosChanging(lpwndpos);
}

afx_msg void
CArrayPicture::OnLButtonDown(UINT nFlags, CPoint point)
{

    LONG  dx = point.x / (m_granularity + 1);
    LONG  dy = point.y / (m_granularity + 1);

    if (dy < m_InputArray.GetSize() &&
        m_InputArray[dy].GetSize())
    {

        m_InputArray[dy][dx] = RGB(0x00, 0x00, 0x00);

        m_LButtonPressed = TRUE;

        ::SetCursor(m_hDrawCursor);

        this->Invalidate(FALSE);
    }

}

CArrayEx<COLORREF>
CArrayPicture::Serialize()
{

    CArrayEx<COLORREF> output;

    SIZE_T maxy = m_InputArray.GetSize();
    SIZE_T maxx = m_InputArray[0].GetSize();

    for (size_t y = 0; y < maxy; y++)
    {
        for (size_t x = 0; x < maxx; x++)
        {
            output.Add(m_InputArray[y][x]);
        }
    }

    return output;
}

afx_msg void
CArrayPicture::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_LButtonPressed = FALSE;
    ::SetCursor(m_hDrawCursor);
}

afx_msg void
CArrayPicture::OnMouseMove(UINT nFlags, CPoint point)
{
    TRACKMOUSEEVENT  tr;

    tr.cbSize = sizeof(TRACKMOUSEEVENT);

    tr.hwndTrack = this->m_hWnd;
    tr.dwFlags = TME_LEAVE;
    tr.dwHoverTime = HOVER_DEFAULT;

    TrackMouseEvent(&tr);


    ::SetCursor(m_hDrawCursor);

    if (m_LButtonPressed == TRUE)
    {
        LONG  dx = point.x / (m_granularity + 1);
        LONG  dy = point.y / (m_granularity + 1);

        if (dy < m_InputArray.GetSize() &&
            m_InputArray[dy].GetSize())
        {
            m_InputArray[dy][dx] = RGB(0x00, 0x00, 0x00);

            this->Invalidate(FALSE);

        }
    }

}

afx_msg void
CArrayPicture::OnMouseLeave()
{
    m_LButtonPressed = FALSE;
    ::SetCursor(m_hPrevios);
}

void
CArrayPicture::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (m_hWnd != NULL)
    {

        m_memoryDC.DeleteDC();
        m_memoryBitmap.DeleteObject();

        m_memoryDC.CreateCompatibleDC(this->GetDC());

        m_memoryBitmap.CreateCompatibleBitmap(this->GetDC(), cx, cy);

        CBitmap* oldBitmap = m_memoryDC.SelectObject(&m_memoryBitmap);

        oldBitmap->DeleteObject();
    }

    CRect   wndRect;
    this->ClientToScreen(wndRect);

}


void
CArrayPicture::Draw(CDC* cdc)
/*
    draw control
*/
{
    LONG position = 0;
    CRect   worksArrea;

    //
    // copy chart array to local buffer, so _chartValues can be freely updates 
    //



    GetClientRect(&worksArrea);

    //CFont* oldFont = cdc->SelectObject(&m_axisFont);
    //
    // fill background
    //
    cdc->FillSolidRect(&worksArrea, CHART_BACKGROUND_COLOR);


    CPen  gridPen(PS_SOLID, 1, CHART_GRID_COLOR);

    CPen  *oldPen = cdc->SelectObject(&gridPen);

    LONG   maxx = m_cx * (m_granularity + 1);
    LONG   maxy = m_cy * (m_granularity + 1);

    for (int y = 0; y < m_cy; y++)
    {
        for (int x = 0; x < m_cx; x++)
        {
            cdc->MoveTo(x * (m_granularity + 1), 0);

            cdc->LineTo(x * (m_granularity + 1), maxy);

            COLORREF  incolor = m_InputArray[y][x];

            CRect  colorRect(
                x * (m_granularity + 1) + 1,
                y * (m_granularity + 1) + 1,
                x * (m_granularity + 1) + m_granularity + 1,
                y * (m_granularity + 1) + m_granularity + 1);

            cdc->FillSolidRect(colorRect, incolor);
        }

        cdc->MoveTo(0, y * (m_granularity + 1));

        cdc->LineTo(maxx, y * (m_granularity + 1));
    }


    cdc->SelectObject(oldPen);
    //cdc->SelectObject(oldFont);
}

void
CArrayPicture::OnPaint()
/*
    WM_PAINT dispatcher
*/
{
    CWnd::OnPaint();

    CClientDC dc(this);

    CRect windowRect;
    this->GetWindowRect(&windowRect);

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
    Draw(&m_memoryDC);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &m_memoryDC, 0, 0, SRCCOPY);
}
