/***************************************************************************************\
*   File:                                                                               *
*       winText.cpp                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       windows font for OpenGL renderer implementation                                 *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    24-Jul-2024                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "winText.h"


CWinText::CWinText(CLogCallback* logcallback)
    : m_log(logcallback)
    , m_width(20)
    , m_height(20)
    , m_textureID(0)
    , m_text(NULL)
    , m_buffer(NULL)
    , m_color(RGB(0xFF, 0xFF, 0xFF))
    , m_hFont(NULL)
    , m_hBitmap(NULL)
{
}

CWinText::~CWinText()
{
    if (m_text != NULL)
        free(m_text);

    if (m_hBitmap)
        DeleteObject(m_hBitmap);

    if (m_hFont)
        DeleteObject(m_hFont);

    glDeleteTextures(1, &m_textureID);
}

void
CWinText::Setup(COLORREF color, int fontSize)
{
    m_width = 10;
    m_height = 10;
    m_color = color;

    // create font
    m_hFont = CreateFont(
        -(fontSize), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Times New Roman"));

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void
CWinText::Update(const TCHAR *newText)
{
    if (!newText)
        return;


    TCHAR *newtext = _tcsdup(newText);

    if (!newtext)
        return;

    if (m_text) {
        free(m_text);
    }

    m_text = newtext;


    GenerateTextureFromGDI();
}

void
CWinText::Draw(const RECT& clientRect, int x, int y)
{

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Преобразование координат
    int windowWidth = clientRect.right - clientRect.left;
    int windowHeight = clientRect.bottom - clientRect.top;

    float left = (float(x) / windowWidth) * 2.0f - 1.0f;
    float right = (float(x + m_width) / windowWidth) * 2.0f - 1.0f;

    float top = 1.0f - (float(y) / windowHeight) * 2.0f;
    float bottom = 1.0f - (float(y + m_height) / windowHeight) * 2.0f;

    glBegin(GL_TRIANGLES);

    // first triangle (lower)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(left, bottom, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(right, bottom, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(right, top, 0.0f);

    // second triangle (upper)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(left, bottom, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(right, top, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(left, top, 0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}


void
CWinText::GenerateTextureFromGDI()
{
    size_t  len = _tcslen(m_text);


    HDC hdcScreen = GetDC(NULL);

    if (!hdcScreen)
        return;

    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    ReleaseDC(NULL, hdcScreen);

    if (!hdcMem)
        return;


    //
    // get actual box size for this text
    //
    SelectObject(hdcMem, m_hFont);

    SIZE  boundBoxSize;
    GetTextExtentPoint32(hdcMem, m_text, (int)len, &boundBoxSize);

    m_width = boundBoxSize.cx + 2;
    m_height = boundBoxSize.cy + 2;

    // retrive pixels
    BITMAPINFO bmpInfo = { 0 };
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = m_width;
    bmpInfo.bmiHeader.biHeight = -m_height;  // inverted
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;


    m_hBitmap = CreateDIBSection(hdcMem, &bmpInfo, DIB_RGB_COLORS, &m_buffer, NULL, 0);

    if (m_hBitmap == NULL)
    {
        DeleteDC(hdcMem);
        return;
    }

    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, m_hBitmap);

    if (hBitmapOld) {
        DeleteObject(hBitmapOld);
    }

    //
    // draw strings
    // 
    RECT rect = { 0, 0, m_width, m_height };

    // 
    // black background
    //


    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdcMem, &rect, hBrush);
    DeleteObject(hBrush);

    SetBkMode(hdcMem, TRANSPARENT);
    SetTextColor(hdcMem, m_color);
    DrawText(hdcMem, m_text, (int)len, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    DeleteDC(hdcMem);

    //
    // transparency update
    //
    BYTE* pixels = (BYTE *)m_buffer;

    for (int y = 0; y < m_height; y++)
    {

        for (int x = 0; x < m_width; x++)
        {
            int index = (y * m_width + x) * 4;

            if (pixels[index + 0] == 0 &&
                pixels[index + 1] == 0 &&
                pixels[index + 2] == 0)
            {
                pixels[index + 3] = 0; // transparent
            }
            else
            {
                pixels[index + 3] = 255; // non transparent
            }
        }
    }


    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

}
