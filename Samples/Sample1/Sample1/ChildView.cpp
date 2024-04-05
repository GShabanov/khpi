
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Sample1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
    m_planet1 = 0.0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
        ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);



    return TRUE;
}

void
CChildView::OnSize(UINT nType, int cx, int cy)
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

void CChildView::DrawCrosshair(CDC& paintDc)
{
    CRect  clientRect;

    GetWindowRect(&clientRect);

    CPoint  start, end;

    start.x = clientRect.Width() / 2;
    start.y = 0;

    end = start;
    end.y = clientRect.Height();



    paintDc.MoveTo(start);
    paintDc.LineTo(end);


    start.x = 0;
    start.y = clientRect.Height() / 2;

    end = start;
    end.x = clientRect.Width();

    paintDc.MoveTo(start);
    paintDc.LineTo(end);

}

void CChildView::Draw(CDC& dc)
{
    CRect   worksArrea;

    GetClientRect(&worksArrea);

    //
    // fill background
    //
    dc.FillSolidRect(&worksArrea, RGB(0xFF, 0xFF, 0xFF));

    DrawCrosshair(dc);

    CPen    drawPenGraph(PS_SOLID, 1, RGB(0xFF, 0xFF, 0x00));
    CBrush  drawBrush;
    drawBrush.CreateSolidBrush(RGB(0xFF, 0xFF, 0x00));

    //CHusBrush  drawBrush(BS_SOLID, graph.m_chartColor);
    CPen* oldPen = dc.SelectObject(&drawPenGraph);
    CBrush* oldBrush = dc.SelectObject(&drawBrush);

    CPoint   center;

    center.x = worksArrea.Width() / 2;
    center.y = worksArrea.Height() / 2;

    CRect   solarRect;

    solarRect.left = center.x - 50;
    solarRect.right = center.x + 50;
    solarRect.top = center.y - 50;
    solarRect.bottom = center.y + 50;

    dc.Ellipse(&solarRect);

    dc.SelectObject(oldBrush);
    dc.SelectObject(oldPen);



    double   fi = 0.0;

    while (fi < 6.28)
    {

        LONG X = (LONG)(center.x + 200 * cos(fi));
        LONG Y = (LONG)(center.y + 200 * sin(fi));

        dc.SetPixel(X, Y, RGB(0x00, 0xFF, 0xFF));

        fi += 0.01;
    }



    //
    // MARS
    //

    LONG Xplanet = (LONG)(center.x + 200 * cos(m_planet1));
    LONG Yplanet = (LONG)(center.y + 200 * sin(m_planet1));



    CPen    drawPenGraph1(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));
    CBrush  drawBrush1;
    drawBrush1.CreateSolidBrush(RGB(0xFF, 0x00, 0x00));

    //CHusBrush  drawBrush(BS_SOLID, graph.m_chartColor);
    oldPen = dc.SelectObject(&drawPenGraph1);
    oldBrush = dc.SelectObject(&drawBrush1);

    center.x = worksArrea.Width() / 2;
    center.y = worksArrea.Height() / 2;

    CRect   marsRect;

    marsRect.left = Xplanet - 20;
    marsRect.right = Xplanet + 20;
    marsRect.top = Yplanet - 20;
    marsRect.bottom = Yplanet + 20;

    dc.Ellipse(&marsRect);

    dc.SelectObject(oldBrush);
    dc.SelectObject(oldPen);


}

void CChildView::OnPaint() 
{
    //

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
    Draw(m_memoryDC);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &m_memoryDC, 0, 0, SRCCOPY);

    
}

