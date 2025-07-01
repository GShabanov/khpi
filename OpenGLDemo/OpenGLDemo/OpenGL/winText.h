/***************************************************************************************\
*   File:                                                                               *
*       winText.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       windows font for OpenGL renderer implementation                                 *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    24-Jul-2024                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#pragma once
#ifndef __OPENGL_WINTEXT_H__
#define __OPENGL_WINTEXT_H__

#include "GlUtils.h"

class CWinText
{
private:

    class CLogCallback* m_log;

    int         m_width;
    int         m_height;

    GLuint      m_textureID;
    TCHAR       *m_text;
    COLORREF    m_color;

    HBITMAP     m_hBitmap;
    HFONT       m_hFont;
    PVOID       m_buffer;

    void GenerateTextureFromGDI();

public:
    CWinText(CLogCallback* logcallback);
    virtual ~CWinText();

    void Setup(COLORREF color = RGB(0xFF, 0xFF, 0xFF), int fontSize = 12);

    void Update(const TCHAR *newText);

    void Draw(const RECT& clientRect, int x, int y);

};

#endif // __OPENGL_WINTEXT_H__