#include "pch.h"
#include "framework.h"
#include "Sprite.h"
#include "Sample5.h"


CSprite::CSprite()
{
    m_Bitmap = NULL;
    m_BitmapData = NULL;

    m_bmHeight = 0;
    m_bmWidth = 0;

    m_InitOk = FALSE;
}

CSprite::~CSprite()
{
    if (m_InitOk)
    {
        DeleteObject(m_Bitmap);
        free(m_BitmapData);
    }
}

CSprite&
CSprite::operator=(const CSprite& other)
{
    m_Bitmap = NULL;
    m_BitmapData = NULL;

    m_bmHeight = 0;
    m_bmWidth = 0;

    m_InitOk = FALSE;

    m_Bitmap = (HBITMAP)CopyImage(other.m_Bitmap, IMAGE_BITMAP, 0, 0, /*LR_CREATEDIBSECTION*/ 0);

    m_BitmapData = GetBitmapData();

    m_InitOk = TRUE;

    return *this;
}

BOOL
CSprite::Init(const CString& filename)
{

    if (m_InitOk)
    {
        DeleteObject(m_Bitmap);
        free(m_BitmapData);
        m_Bitmap = NULL;
        m_BitmapData = NULL;

        m_bmHeight = 0;
        m_bmWidth = 0;

        m_InitOk = FALSE;
    }

    if (LoadImageFile(filename) != TRUE)
    {
        return FALSE;
    }

    m_BitmapData = GetBitmapData();

    m_InitOk = TRUE;
    
    return TRUE;
}

BOOL
CSprite::Init(LPCTSTR resource, LPCTSTR type)
{
    if (m_InitOk)
    {
        DeleteObject(m_Bitmap);
        free(m_BitmapData);
        m_Bitmap = NULL;
        m_BitmapData = NULL;

        m_bmHeight = 0;
        m_bmWidth = 0;

        m_InitOk = FALSE;
    }

    if (LoadFromResources(resource, type) != TRUE)
    {
        return FALSE;
    }


    m_BitmapData = GetBitmapData();

    m_InitOk = TRUE;

    return TRUE;
}

PVOID
CSprite::GetBitmapData()
{
    // Get bitmap info
    BITMAP bmp;
    GetObject(m_Bitmap, sizeof(BITMAP), &bmp);

    WORD iBpp = bmp.bmBitsPixel >> 3;
    LONG iPitch = bmp.bmWidthBytes;
    LONG bmpSize = bmp.bmHeight * iPitch;

    void* pData = (LPBYTE)malloc(bmpSize * sizeof(BYTE));

    if (pData == NULL)
    {
        return NULL;
    }

    m_bmHeight = bmp.bmHeight;
    m_bmWidth = bmp.bmWidth;

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));

    GetBitmapBits(m_Bitmap, (LONG)(bmpSize * sizeof(BYTE)), pData);

    for (DWORD i = 0, j = (DWORD)(m_bmHeight * m_bmWidth); i < j; i++)
    {
        DWORD data = *((DWORD*)pData + i);

        data = _byteswap_ulong(data);

        data = (data >> 8);

        *((DWORD*)pData + i) = data;
    }

    return pData;

}

BOOL
CSprite::LoadImageFile(const CString& filename)
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

BOOL
CSprite::LoadFromResources(LPCTSTR resource, LPCTSTR type)
{
    BOOL      _return = FALSE;
    HRSRC     hDIBRes;
    HGLOBAL   hDIBMem;
    HGLOBAL   hDIBCopy;
    DWORD     nBmpImageSize = 0;
    IStream*  pStream = NULL;

    IPicture* pPicture;


    if ((hDIBRes = ::FindResource(theApp.m_hInstance, resource, type)) > NULL)
    {

        if ((hDIBMem = ::LoadResource(theApp.m_hInstance, hDIBRes)) > 0)
        {

            if (::LockResource(hDIBMem))
            {

                nBmpImageSize = ::SizeofResource(theApp.m_hInstance, hDIBRes);

                hDIBCopy = ::GlobalAlloc(GMEM_MOVEABLE, nBmpImageSize);

                if (hDIBCopy != NULL)
                {

                    PVOID hgInMem = ::GlobalLock(hDIBCopy);

                    memcpy(hgInMem, hDIBMem, nBmpImageSize);

                    if (nBmpImageSize > 0 && ::CreateStreamOnHGlobal(hDIBCopy, TRUE, &pStream) == S_OK)
                    {

                        HRESULT result = ::OleLoadPicture(
                            pStream,
                            nBmpImageSize,
                            FALSE,
                            IID_IPicture,
                            (LPVOID*)&pPicture);

                        if (result == S_OK)
                        {


                            LONG hmWidth = 0;
                            LONG hmHeight = 0;
                            HBITMAP hsrc;
                            HRESULT   hr;

                            pPicture->get_Width(&hmWidth);
                            pPicture->get_Height(&hmHeight);

                            hr = pPicture->get_Handle((OLE_HANDLE*)&hsrc);

                            if (SUCCEEDED(hr))
                            {
                                //
                                // get bitmap for this file
                                //
                                m_Bitmap = (HBITMAP)CopyImage(hsrc, IMAGE_BITMAP, 0, 0, /*LR_CREATEDIBSECTION*/ 0);


                                if (m_Bitmap != NULL)
                                {
                                    _return = TRUE;
                                }

                            }

                            pPicture->Release();
                            
                        }

                        pStream->Release();
                    }

                    ::GlobalUnlock(hDIBCopy);

                    ::GlobalFree(hDIBCopy);
                }


                ::FreeResource(hDIBMem);
            }
        }
    }

    return _return;
}

void
CSprite::Draw(
    DWORD* canvas,
    CRect& canvasDimensions,
    CRect& drawRect,
    BOOL  drawBackground
    )
{

    //
    // scale factors calculation
    //
    double x_scale = (double)m_bmWidth / (double)drawRect.Width();
    double y_scale = (double)m_bmHeight / (double)drawRect.Height();


    DWORD  offset = drawRect.top * canvasDimensions.Width() + drawRect.left;

    if (drawRect.top < canvasDimensions.top)
        return;

    if (drawRect.left < canvasDimensions.left)
        return;

    if (drawRect.right > canvasDimensions.right)
        return;

    if (drawRect.bottom > canvasDimensions.bottom)
        return;

    //
    // copy image in buffer
    //

    for (long y = 0, ymax = drawRect.Height(); y < ymax; y++)
    {

        //
        // texture Y calculation
        //
        DWORD  textureY = (DWORD)(y * y_scale);

        //
        // liner offset calculation
        //
        DWORD  yOffset = (DWORD)(textureY * m_bmWidth);

        //
        // X loop
        //
        for (long x = 0, xmax = drawRect.Width(); x < xmax; x++)
        {

            DWORD  textureX = (DWORD)(x * x_scale);

            DWORD color = *((DWORD*)m_BitmapData + yOffset + textureX);



            if (drawBackground)
            {
                *(canvas + offset + y * canvasDimensions.Width() + x) = color;
            }
            else
            {

                if (color != 0)
                {
                    DWORD Rvalue = 2126 * (DWORD)GetRValue(color);
                    DWORD Gvalue = 7152 * (DWORD)GetGValue(color);
                    DWORD Bvalue = 722 * (DWORD)GetBValue(color);

                    DWORD Clinear = (Rvalue + Gvalue + Bvalue) / 10000;

                    if (Clinear > 5)
                    {
                        //*((DWORD*)oldData + offset + y * bmpInfo.bi.bmiHeader.biWidth + x) = color;
                        *(canvas + offset + y * canvasDimensions.Width() + x) = color;
                    }
                }
            }
        }
    }

}

void
CSprite::DrawPartial(
    DWORD* canvas,
    CRect& canvasDimensions,
    CRect& drawRect,
    CRect& spriteRect,
    BOOL  drawBackground
    )
{
    CRect  spriteRectLocal;
    spriteRectLocal = spriteRect;

    if (spriteRectLocal.left > m_bmWidth)
        return;

    if (spriteRectLocal.top > m_bmHeight)
        return;

    if (spriteRectLocal.right > m_bmWidth) {

        spriteRectLocal.right = m_bmWidth;
    }

    if (spriteRectLocal.bottom > m_bmHeight) {

        spriteRectLocal.bottom = m_bmHeight;
    }

    //
    // scale factors calculation
    //
    double x_scale = (double)spriteRectLocal.Width() / (double)drawRect.Width();
    double y_scale = (double)spriteRectLocal.Height() / (double)drawRect.Height();


    DWORD  offset = drawRect.top * canvasDimensions.Width() + drawRect.left;

    if (drawRect.top < canvasDimensions.top)
        return;

    if (drawRect.left < canvasDimensions.left)
        return;

    if (drawRect.right > canvasDimensions.right)
        return;

    if (drawRect.bottom > canvasDimensions.bottom)
        return;

    //
    // copy image in buffer
    //

    for (long y = 0, ymax = drawRect.Height(); y < ymax; y++)
    {

        //
        // texture Y calculation
        //
        DWORD  textureY = (DWORD)(y * y_scale + spriteRectLocal.top);

        //
        // liner offset calculation
        //
        DWORD  yOffset = (DWORD)(textureY * m_bmWidth);

        //
        // X loop
        //
        for (long x = 0, xmax = drawRect.Width(); x < xmax; x++)
        {

            DWORD  textureX = (DWORD)(x * x_scale + spriteRectLocal.left);

            DWORD color = *((DWORD*)m_BitmapData + yOffset + textureX);



            if (drawBackground)
            {
                *(canvas + offset + y * canvasDimensions.Width() + x) = color;
            }
            else
            {

                if (color != 0)
                {
                    DWORD Rvalue = 2126 * (DWORD)GetRValue(color);
                    DWORD Gvalue = 7152 * (DWORD)GetGValue(color);
                    DWORD Bvalue = 722 * (DWORD)GetBValue(color);

                    DWORD Clinear = (Rvalue + Gvalue + Bvalue) / 10000;

                    if (Clinear > 5)
                    {
                        //*((DWORD*)oldData + offset + y * bmpInfo.bi.bmiHeader.biWidth + x) = color;
                        *(canvas + offset + y * canvasDimensions.Width() + x) = color;
                    }
                }
            }
        }
    }
}