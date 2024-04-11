
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Sample3.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
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


    m_graphicScene.Init();


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

void CChildView::Draw(CDC& dc)
{
    CRect   worksArrea;

    GetClientRect(&worksArrea);

    //
    // fill background
    //
    dc.FillSolidRect(&worksArrea, RGB(0xFF, 0xFF, 0xFF));


    m_graphicScene.Draw(dc, worksArrea, worksArrea);



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

