
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Sample4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
    m_rawPixels = NULL;
}

CChildView::~CChildView()
{
    if (m_rawPixels != NULL)
    {
        m_memoryDC.DeleteDC();
        m_memoryBitmap.DeleteObject();
        free(m_rawPixels);
    }

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
    struct
    {
        BITMAPINFO      bi;
        RGBQUAD         zeroColor[2];
    } bmpInfo;

    CWnd::OnSize(nType, cx, cy);

    if (m_hWnd != NULL)
    {

        m_memoryDC.DeleteDC();
        m_memoryBitmap.DeleteObject();

        m_memoryDC.CreateCompatibleDC(this->GetDC());

        m_memoryBitmap.CreateCompatibleBitmap(this->GetDC(), cx, cy);

        CBitmap* oldBitmap = m_memoryDC.SelectObject(&m_memoryBitmap);

        oldBitmap->DeleteObject();


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

    }

    CRect   wndRect;
    this->ClientToScreen(wndRect);

}

void CChildView::Draw(CDC& dc, CBitmap& bitmap)
{
    CRect   worksArrea;

    GetClientRect(&worksArrea);

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

    if (GetDIBits(dc.GetSafeHdc(), (HBITMAP)bitmap.GetSafeHandle(), 0, 0, NULL, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {
        return;
    }


    bmpInfo.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bi.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bi.bmiHeader.biHeight = -bmpInfo.bi.bmiHeader.biHeight;

    //
    // copy background bits to buffer oldData
    //

    if (GetDIBits(dc.GetSafeHdc(), (HBITMAP)bitmap.GetSafeHandle(), 0, bmpInfo.bi.bmiHeader.biHeight, m_rawPixels, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {
        return;
    }



    m_graphicScene.Draw((DWORD *)m_rawPixels, worksArrea, worksArrea);


    //
    // copy image buffer to DC
    //
    SetDIBitsToDevice(
        dc.GetSafeHdc(),        // handle to DC
        0,                      // x-coord of destination upper-left corner
        0,                      // y-coord of destination upper-left corner 
        bmpInfo.bi.bmiHeader.biWidth,       // source rectangle width
        -bmpInfo.bi.bmiHeader.biHeight,      // source rectangle height
        0,                      // x-coord of source lower-left corner
        0,                      // y-coord of source lower-left corner
        0,                      // first scan line in array
        -bmpInfo.bi.bmiHeader.biHeight,      // number of scan lines
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
    Draw(m_memoryDC, m_memoryBitmap);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &m_memoryDC, 0, 0, SRCCOPY);

    
}

