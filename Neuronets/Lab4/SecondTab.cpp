
// CSecondTab.cpp : implementation file
//

#include "pch.h"
#include <afxdialogex.h>
#include "..\..\Shared\MFCPicture.h"
#include "SecondTab.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstTab dialog

CSecondTab::CSecondTab()
    : CTabTemplate()
    , m_Picture(NULL)
    , m_bInPicture(FALSE)
{
}


BEGIN_MESSAGE_MAP(CSecondTab, CTabTemplate)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CFirstTab message handlers
BOOL
CSecondTab::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
    CRect  clRect;
    CRect  workRect;

    if (__super::Create(dwStyle | WS_CHILD, rect, pParentWnd) != TRUE)
    {
        return FALSE;
    }

    this->GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    workRect = clRect;

    workRect.top += 70;
    workRect.right = workRect.left + 5 * 64 + 2;
    workRect.bottom = workRect.top + 5 * 64 + 2;

    m_Picture = new CArrayPicture();

    m_Picture->Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
        workRect, this);

    m_Picture->SetDrawCursor(LoadCursor((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1)));

    workRect.left = 5 * 64 + 2 + 10;
    workRect.right += 100;
    workRect.bottom = workRect.top + 30;

    m_LearnButton.Create(
        _T("Завчити"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_DISABLED, workRect, this, NULL);


    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_ResetButton.Create(
        _T("Скинути"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, workRect, this, NULL);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_RestoreButton.Create(
        _T("Відновити"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED, workRect, this, NULL);

    workRect.top = workRect.bottom + 10;
    workRect.bottom = clRect.bottom - 10;
    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 110;

    m_PictureList.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED,
        workRect,
        this, NULL);



    LoadTrainingData(CString(_T(".\\data\\")));



    return TRUE;  // return TRUE  unless you set the focus to a control

}

BOOL
CSecondTab::LoadImageFile(const CString &filename)
{
    IPicture* pPicture;
    IStream*  pStream;
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
    HBITMAP hbmp = (HBITMAP)CopyImage(hsrc, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);


    if (hbmp == NULL)
    {
        pPicture->Release();
        return FALSE;
    }

    CBitmap   *bitmap = CBitmap::FromHandle(hbmp);

    CListCtrlMy::DRAW_CONTEXT  *drawContext = new CListCtrlMy::DRAW_CONTEXT;

    if (drawContext != NULL)
    {

        drawContext->bitmap = bitmap;
        drawContext->Learned = FALSE;

        m_BitmapTable.Add(drawContext);

        m_PictureList.InsertItem(LVIF_IMAGE | LVIF_STATE | LVIF_PARAM, (int)(m_BitmapTable.GetSize() - 1), _T(""), 0, 0, 0, (LPARAM)drawContext);
    }

    pPicture->Release();

    return TRUE;
}

BOOL
CSecondTab::LoadTrainingData(const CString &path)
{
    CString  searchPath;
    WIN32_FIND_DATA  findData;

    HANDLE           hFindHandle;

    searchPath = path;
    searchPath += _T("*.jpg");

    hFindHandle = FindFirstFile(
        searchPath,
        &findData);

    while (hFindHandle != NULL)
    {

        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
        {
            CString  fileName = path;
            fileName += findData.cFileName;


            LoadImageFile(fileName);


        }


        if (!FindNextFile(hFindHandle, &findData))
            break;
    }


    FindClose(hFindHandle);

    return TRUE;
}

void
CSecondTab::OnDestroy()
{

    m_Picture->DestroyWindow();
    delete m_Picture;

    m_LearnButton.DestroyWindow();
    m_PictureList.DestroyWindow();

    while (m_BitmapTable.GetSize() > 0)
    {
        m_BitmapTable[0]->bitmap->DeleteObject();
        delete m_BitmapTable[0];

        m_BitmapTable.RemoveAt(0, 1);
    }

    __super::OnDestroy();
}

void
CSecondTab::OnPaint()
{
    PAINTSTRUCT     paintStruct = { 0, };
    CDC             *dc;


    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {

        CRect  clRect;

        HGDIOBJ oldFont = dc->SelectObject(m_bigFont);

        GetClientRect(clRect);

        clRect.top = 20;
        clRect.bottom = 70;
        clRect.left = clRect.left = clRect.Width() / 2 - (16 * sizeof("Лабораторна робота 4, Шабанов Г.В. ІКМ-М222К") / 3);
        clRect.right = clRect.left + 500;

        dc->DrawText(CString(_T("Лабораторна робота 4, Шабанов Г.В. ІКМ-М222К")), clRect, 0);

        dc->SelectObject(oldFont);

        this->EndPaint(&paintStruct);
    }
}


afx_msg void 
CSecondTab::OnMouseMove(UINT nFlags, CPoint point)
{
    //::ShowCursor(TRUE);
    /*RECT inwindow;

    m_Picture->GetClientRect(&inwindow);

    this->MapWindowPoints(m_Picture, &inwindow);
    //this->ClientToScreen(&point);

    if (PtInRect(&inwindow, point))
    {
        if (m_bInPicture == FALSE)
        {
            m_bInPicture = TRUE;

            ::ShowCursor(FALSE);
        }
    }
    else
    {

        if (m_bInPicture == TRUE)
        {
            ::ShowCursor(TRUE);
        }
        
        m_bInPicture = FALSE;
    }*/
    
}

void
CSecondTab::OnSelectPicture(CListCtrlMy::DRAW_CONTEXT* pContext)
{
    CBitmap* Bitmap = pContext->bitmap;

    BITMAP  bitmap;

    if (!Bitmap->GetBitmap(&bitmap))
        return;



    CArrayEx<CArrayEx<COLORREF>>  newArray;

    BYTE* dwBits = (BYTE*)bitmap.bmBits;

    DWORD  dwMask = (DWORD)(bitmap.bmBitsPixel == 32 ? -1 : (1 << bitmap.bmBitsPixel) - 1);

    for (LONG y = 0; y < bitmap.bmHeight; y++)
    {
        newArray.Add(CArrayEx<COLORREF>());

        for (LONG x = 0; x < bitmap.bmWidth; x++)
        {
            COLORREF  color = RGB(0x80, 0x20, 0x20);

            newArray[y].Add(color);
        }
    }

    for (LONG y = 0; y < bitmap.bmHeight; y++)
    {

        for (LONG x = 0; x < bitmap.bmWidth; x++)
        {
            BYTE Blue = *(dwBits + 0);
            BYTE Green = *(dwBits + 1);
            BYTE Red = *(dwBits + 2);

            COLORREF  color = RGB(Red, Green, Blue);

            newArray[bitmap.bmHeight - 1 - y][x] = color;

            dwBits = dwBits + (bitmap.bmBitsPixel >> 3);
        }


    }

    m_Picture->SetInputArray(newArray);

    if (pContext->Learned == TRUE)
    {
        m_LearnButton.EnableWindow(FALSE);
    }
    else
    {
        m_LearnButton.EnableWindow(TRUE);
    }

}

BOOL
CSecondTab::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* header = (NMHDR*)lParam;

    if (header->hwndFrom == m_PictureList.m_hWnd &&
        header->code == LVN_ITEMCHANGING)
    {

        NMLISTVIEW * pNMListView = (LPNMLISTVIEW)lParam;

        if (pNMListView->uNewState & LVIS_SELECTED)
        {
            pNMListView = pNMListView;

            OnSelectPicture((CListCtrlMy::DRAW_CONTEXT*)pNMListView->lParam);

        }

    }

    return __super::OnNotify(wParam, lParam, pResult);
}

BOOL
CSecondTab::OnCommand(
    WPARAM wParam,
    LPARAM lParam
    )
{
    if ((HWND)lParam == m_PictureList.m_hWnd)
    {
        m_PictureList.m_hWnd = m_PictureList.m_hWnd;
    }

    if ((HWND)lParam == m_LearnButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {

        POSITION pos = m_PictureList.GetFirstSelectedItemPosition();

        if (pos != NULL)
        {
            int nItem = m_PictureList.GetNextSelectedItem(pos);

            CListCtrlMy::DRAW_CONTEXT *m_pItemData = (CListCtrlMy::DRAW_CONTEXT*)m_PictureList.GetItemData(nItem);

            m_pItemData->Learned = TRUE;

            m_LearnButton.EnableWindow(FALSE);
            m_PictureList.Invalidate(TRUE);
        }


    }

    if ((HWND)lParam == m_ResetButton.m_hWnd &&
        HIWORD(wParam) == BN_CLICKED)
    {
        for (INT_PTR i = 0, j = m_BitmapTable.GetSize(); i < j; i++)
        {
            m_BitmapTable[i]->Learned = FALSE;

            if (m_PictureList.GetFirstSelectedItemPosition() != NULL)
            {
                m_LearnButton.EnableWindow(TRUE);
            }
            else
            {
                m_LearnButton.EnableWindow(FALSE);
            }

            m_PictureList.Invalidate(TRUE);

        }
    }

    return __super::OnCommand(wParam, lParam);


}


void
CSecondTab::OnSize(UINT nType, int cx, int cy)
{
    CRect  clRect;
    CRect  workRect;

    __super::OnSize(nType, cx, cy);

    if (m_Picture == NULL)
        return; // Return if window is not created yet.


    GetClientRect(&clRect);

    clRect.DeflateRect(1, 1);

    workRect = clRect;

    workRect.top += 70;
    workRect.right -= 200;


    m_Picture->SetWindowPos(this, workRect.left, workRect.top, 5 * 64 + 2, 5 * 64 + 2, SWP_NOZORDER);

    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 100;
    workRect.bottom = workRect.top + 30;

    m_LearnButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_ResetButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.left = workRect.right + 5;
    workRect.right += 100;

    m_RestoreButton.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

    workRect.top = workRect.bottom + 10;
    workRect.bottom = clRect.bottom - 10;
    workRect.left = 5 * 64 + 2 + 10;
    workRect.right = workRect.left + 110;

    m_PictureList.SetWindowPos(this, workRect.left, workRect.top, workRect.Width(), workRect.Height(), SWP_NOZORDER);

}
