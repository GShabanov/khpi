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
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcontrolbars.h>
#include "DynamicPicture.h"

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
BEGIN_MESSAGE_MAP(CDynamicPicture, CWnd)
    //{{AFX_MSG_MAP(CDynamicPicture)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGraphControl construction

CDynamicPicture::CDynamicPicture()
    : CWnd()
{
    m_angle = 0.0;
}

CDynamicPicture::~CDynamicPicture()
{

}

void
CDynamicPicture::OnDestroy()
{
    CWnd::OnDestroy();

    DeleteObject(m_Bitmap);

    m_memoryBitmap.DeleteObject();
    m_memoryDC.DeleteDC();

}


BOOL
CDynamicPicture::Create(
    DWORD dwStyle, const CRect& rect, CWnd* pParentWnd)
{


    if (CWnd::Create(NULL, NULL, dwStyle | WS_CHILD, rect, pParentWnd, NULL, NULL) != TRUE)
    {
        return FALSE;
    }

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


    LoadImageFile(CString("data\\satellite2.gif"));

    this->Invalidate(TRUE);

    return TRUE;
}

BOOL
CDynamicPicture::LoadImageFile(const CString& filename)
{
    IPicture* pPicture;
    IStream* pStream;
    HRESULT   hr;

    //
    // Load any file (jpg, bmp, etc.)
    //
    hr = SHCreateStreamOnFile(
        filename.operator LPCWSTR(),
        STGM_READ,
        &pStream);


    if (FAILED(hr))
        return FALSE;


    hr = ::OleLoadPicture(
        pStream,
        0,
        FALSE,
        IID_IPicture,
        (LPVOID*)&pPicture);


    pStream->Release();

    if (FAILED(hr))
    {
        return FALSE;
    }

    LONG hmWidth = 0;
    LONG hmHeight = 0;
    HBITMAP hsrc;

    pPicture->get_Width(&hmWidth);
    pPicture->get_Height(&hmHeight);

    hr = pPicture->get_Handle((OLE_HANDLE*)&hsrc);

    if (FAILED(hr))
    {
        pPicture->Release();
        return FALSE;
    }

    //
    // get bitmap for this file
    //
    m_Bitmap = (HBITMAP)CopyImage(hsrc, IMAGE_BITMAP, 0, 0, /*LR_CREATEDIBSECTION*/ 0);


    if (m_Bitmap == NULL)
    {
        pPicture->Release();
        return FALSE;
    }

    // Check for valid bitmap format (16, 24 and 32bpp only)
    // Get bitmap data
    //m_pBitmap = CBitmap::FromHandle(hbmp);

    pPicture->Release();

    return TRUE;
}

afx_msg void
CDynamicPicture::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{


    __super::OnWindowPosChanging(lpwndpos);
}


void
CDynamicPicture::OnSize(UINT nType, int cx, int cy)
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

typedef long fixed;												// Our new fixed point type
#define itofx(x) ((x) << 8)										// Integer to fixed point
#define ftofx(x) (long)((x) * 256)								// Float to fixed point
#define dtofx(x) (long)((x) * 256)								// Double to fixed point
#define fxtoi(x) ((x) >> 8)										// Fixed point to integer
#define fxtof(x) ((float) (x) / 256)							// Fixed point to float
#define fxtod(x) ((double)(x) / 256)							// Fixed point to double
#define Mulfx(x,y) (((x) * (y)) >> 8)							// Multiply a fixed by a fixed
#define Divfx(x,y) (((x) << 8) / (y))							// Divide a fixed by a fixed

void
CDynamicPicture::DrawTransmogrified(CDC* dc, CBitmap* bitmap, CPoint& position, long width, long height, double angle)
{
    long bmWidth = 0;
    long bmHeight = 0;

    // Get bitmap info
    BITMAP bmp;
    GetObject(m_Bitmap, sizeof(BITMAP), &bmp);

    WORD iBpp = bmp.bmBitsPixel >> 3;
    LONG iPitch = bmp.bmWidthBytes;
    LONG bmpSize = bmp.bmHeight * iPitch;

    void* pData = (LPBYTE)malloc(bmpSize * sizeof(BYTE));

    if (pData == NULL)
    {
        return;
    }

    bmWidth = bmp.bmWidth;
    bmHeight = bmp.bmHeight;

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));

    GetBitmapBits(m_Bitmap, (LONG)(bmpSize * sizeof(BYTE)), pData);

    if (width != 0 && height != 0)
    {
        // Calculate scaling params
        long _width = max(1, width);
        long _height = max(1, height);
        float dx = (float)bmWidth / (float)_width;
        float dy = (float)bmHeight / (float)_height;
        long _pitch = iBpp * _width;

        while ((_pitch & 3) != 0)
            _pitch++;

        fixed f_dx = ftofx(dx);
        fixed f_dy = ftofx(dy);

        // Create temporary bitmap
        DWORD dwSize = _pitch * _height;
        LPBYTE dataScalled = (LPBYTE)malloc(dwSize * sizeof(BYTE));

        // Scale bitmap
        DWORD dwDstHorizontalOffset;
        DWORD dwDstVerticalOffset = 0;
        DWORD dwDstTotalOffset;
        LPDWORD lpSrcData = (LPDWORD)pData;
        LPDWORD lpDstData = (LPDWORD)dataScalled;

        for (long i = 0; i < _height; i++)
        {
            dwDstHorizontalOffset = 0;

            for (long j = 0; j < _width; j++)
            {
                DWORD dwSrcTotalOffset;

                // Update destination total offset
                dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

                // Update bitmap
                fixed f_i = itofx(i);
                fixed f_j = itofx(j);
                fixed f_a = Mulfx(f_i, f_dy);
                fixed f_b = Mulfx(f_j, f_dx);
                long m = fxtoi(f_a);
                long n = fxtoi(f_b);
                dwSrcTotalOffset = m * iPitch + n * iBpp;
                lpDstData[dwDstTotalOffset >> 2] = lpSrcData[dwSrcTotalOffset >> 2];

                // Update destination horizontal offset
                dwDstHorizontalOffset += iBpp;
            }

            // Update destination vertical offset
            dwDstVerticalOffset += _pitch;
        }

        // Update bitmap info
        iPitch = _pitch;
        bmWidth = _width;
        bmHeight = _height;

        free(pData);

        pData = dataScalled;
    }

    //
    // rotate
    // 
    if (angle != 0.0)
    {
        //float _angle = ((float)-degrees / 180.0f) * _PI;
        //long _width = (long)(abs((float)bmWidth * cos(angle)) + abs((float)bmHeight * sin(angle)) + 0.5f);
        //long _height = (long)(abs((float)bmWidth * sin(angle)) + abs((float)bmHeight * cos(angle)) + 0.5f);

        long _width = bmWidth;
        long _height = bmHeight;


        long _pitch = iBpp * _width;

        while ((_pitch & 3) != 0)
            _pitch++;

        fixed f_0_5 = ftofx(0.5f);
        fixed f_W = itofx(_width / 2);
        fixed f_H = itofx(_height / 2);
        fixed f_cos = ftofx(cos(-angle));
        fixed f_sin = ftofx(sin(-angle));

        // Create temporary bitmap
        DWORD dwSize = _pitch * _height;
        LPBYTE dataRotated = (LPBYTE)malloc(dwSize * sizeof(BYTE));

        // Rotate bitmap
        DWORD dwDstHorizontalOffset;
        DWORD dwDstVerticalOffset = 0;
        DWORD dwDstTotalOffset;
        LPDWORD lpSrcData = (LPDWORD)pData;
        LPDWORD lpDstData = (LPDWORD)dataRotated;

        for (long i = 0; i < _height; i++)
        {
            dwDstHorizontalOffset = 0;

            for (long j = 0; j < _width; j++)
            {
                // Update destination total offset
                dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

                // Update bitmap
                fixed f_i = itofx(i - _height / 2);
                fixed f_j = itofx(j - _width / 2);
                fixed f_m = Mulfx(f_j, f_sin) + Mulfx(f_i, f_cos) + f_0_5 + f_H;
                fixed f_n = Mulfx(f_j, f_cos) - Mulfx(f_i, f_sin) + f_0_5 + f_W;
                long m = fxtoi(f_m);
                long n = fxtoi(f_n);

                if ((m >= 0) && (m < bmHeight) && (n >= 0) && (n < bmWidth))
                {
                    DWORD dwSrcTotalOffset;

                    dwSrcTotalOffset = m * iPitch + n * iBpp;
                    lpDstData[dwDstTotalOffset >> 2] = lpSrcData[dwSrcTotalOffset >> 2];
                }
                else
                    lpDstData[dwDstTotalOffset >> 2] = RGB(0xFF, 0xFF, 0xFF);

                // Update destination horizontal offset
                dwDstHorizontalOffset += iBpp;
            }

            // Update destination vertical offset
            dwDstVerticalOffset += _pitch;
        }

        // Update bitmap info
        iPitch = _pitch;
        bmWidth = _width;
        bmHeight = _height;
        free(pData);

        pData = dataRotated;
    }



    CRect  rect(position.x, position.y, position.x + bmWidth, position.y + bmHeight);

    CopyToDc(dc, bitmap, rect, pData);

    free(pData);

}

void
CDynamicPicture::CopyToDc(CDC* dc, CBitmap* bitmap, CRect& rect, void* data)
{

    CRect  clrect;
    struct
    {
        BITMAPINFO      bi;
        RGBQUAD         zerroColor[2];
    } bmpInfo;


    GetClientRect(&clrect);
    //
    // get 
    //

    memset(&bmpInfo, 0, sizeof(bmpInfo));

    bmpInfo.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bi.bmiHeader.biCompression = BI_RGB;

    if (GetDIBits(dc->GetSafeHdc(), (HBITMAP)bitmap->GetSafeHandle(), 0, 0, NULL, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {
        return;
    }

    void* oldData = malloc(bmpInfo.bi.bmiHeader.biSizeImage);

    if (oldData == NULL)
    {
        return;
    }


    bmpInfo.bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bi.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bi.bmiHeader.biHeight = -bmpInfo.bi.bmiHeader.biHeight;


    if (GetDIBits(dc->GetSafeHdc(), (HBITMAP)bitmap->GetSafeHandle(), 0, bmpInfo.bi.bmiHeader.biHeight, oldData, &bmpInfo.bi, DIB_RGB_COLORS) == 0)
    {

        free(oldData);
        return;
    }

    for (long y = 0, ymax = rect.Height(); y < ymax; y++)
    {
        for (long x = 0, xmax = rect.Width(); x < xmax; x++)
        {
            DWORD color = *((DWORD*)data + y * rect.Width() + x);

            if (color != 0x0FFFFFF && color != 0)
            {
                *((DWORD*)oldData + (y + rect.top) * clrect.Width() + x + rect.left) = color;
            }
        }
    }


    SetDIBitsToDevice(
        dc->GetSafeHdc(),       // handle to DC
        0,					    // x-coord of destination upper-left corner
        0,					    // y-coord of destination upper-left corner 
        clrect.Width(),         // source rectangle width
        clrect.Height(),        // source rectangle height
        0,                      // x-coord of source lower-left corner
        0,                      // y-coord of source lower-left corner
        0,                      // first scan line in array
        clrect.Height(),        // number of scan lines
        ((DWORD*)oldData),      // array of DIB bits
        &bmpInfo.bi,		    // bitmap information
        DIB_RGB_COLORS);        // RGB or palette indexes


    free(oldData);

}


void
CDynamicPicture::Draw(CDC* cdc, CBitmap* bitmap)
/*
    draw control
*/
{
    CPen* pOldPen;

    CRect  workRect;
    GetClientRect(&workRect);


    CPoint centralPoint(workRect.Width() / 2, workRect.Height() / 2);

    //::GetStockObject(DEFAULT_GUI_FONT);

    CBrush brush;
    brush.CreateSysColorBrush(COLOR_BTNFACE);

    //CBrush *brush = (CBrush*)dc->GetGetCurrentBrush();

    cdc->FillRect(workRect, &brush);

    CPen  axisPen(PS_SOLID, 1, RGB(0xFF, 0, 0));

    pOldPen = (CPen*)cdc->SelectObject(&axisPen);

    cdc->MoveTo(centralPoint);
    cdc->LineTo(centralPoint.x, workRect.top);
    cdc->MoveTo(centralPoint);
    cdc->LineTo(workRect.right, centralPoint.y);

    cdc->TextOutW(centralPoint.x + 2, workRect.top + 2, L"0");

    cdc->SelectObject(pOldPen);
    cdc->SelectObject(brush);


    CPoint offsetPoint = centralPoint;

    offsetPoint.x -= 50;
    offsetPoint.y -= 60;

    DrawTransmogrified(cdc, bitmap, offsetPoint, workRect.Width() / 2, workRect.Height() / 2, m_angle);

    CPen  pointerPen(PS_SOLID, 1, RGB(0x0, 0xFF, 0));

    pOldPen = (CPen*)cdc->SelectObject(&pointerPen);

    cdc->MoveTo(centralPoint);
    cdc->LineTo(centralPoint.x + (LONG)(workRect.Width() * cos(m_angle - 1.5707) / 2),
        centralPoint.y + (LONG)(workRect.Height() * sin(m_angle - 1.5707) / 2));

    cdc->SelectObject(pOldPen);
}

void
CDynamicPicture::OnPaint()
/*
    WM_PAINT dispatcher
*/
{
    PAINTSTRUCT     paintStruct = { 0, };
    CRect           windowRect;

    CWnd::OnPaint();

    CClientDC dc(this);

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
    Draw(&m_memoryDC, &m_memoryBitmap);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &m_memoryDC, 0, 0, SRCCOPY);

}
