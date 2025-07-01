/***************************************************************************************\
*   File:                                                                               *
*       TeXtext.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       LaTeX renderer implementation                                                   *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    24-Jul-2024                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#pragma once
#ifndef __OPENGL_TEXTEXT_H__
#define __OPENGL_TEXTEXT_H__

#include "GlUtils.h"
#include <xstring>
#include "LaTeX.h"

namespace tex {

    class Font_win32 : public Font {
    private:
        static const Gdiplus::FontFamily* m_serif;
        static const Gdiplus::FontFamily* m_sansserif;

        float                             m_size;

        Font_win32();

    public:
        int                            m_style;
        std::shared_ptr<Gdiplus::Font> m_typeface;
        const Gdiplus::FontFamily*     m_family;

        Font_win32(const std::string& name, int style, float size);

        Font_win32(const std::string& file, float size);

        virtual float getSize() const override;

        virtual std::shared_ptr<Font> deriveFont(int style) const override;

        virtual bool operator==(const Font& f) const override;

        virtual bool operator!=(const Font& f) const override;

        virtual ~Font_win32();

        static int convertStyle(int style);
    };

    /**************************************************************************************************/

    class TextLayout_win32 : public TextLayout
    {
    private:
        std::shared_ptr<Font_win32> m_font;
        std::wstring                m_txt;

    public:
        static const Gdiplus::StringFormat* m_format;
        static Gdiplus::Graphics*           m_g;
        static Gdiplus::Bitmap*             m_img;

        TextLayout_win32(const std::wstring& src, const std::shared_ptr<Font_win32>& font);

        virtual void getBounds(Rect& bounds) override;

        virtual void draw(Graphics2D& g2, float x, float y) override;
    };

};

class CTeXText : public tex::Graphics2D
{
private:

    class CLogCallback* m_log;

    int         m_width;
    int         m_height;

    GLuint               m_textureID;
    TCHAR*               m_text;

    static const Gdiplus::StringFormat* m_format;
    static const tex::Font*             m_defaultFont;

    tex::color           m_color;
    tex::Stroke          m_stroke;
    Gdiplus::Graphics*   m_g;
    Gdiplus::Pen*        m_pen;
    Gdiplus::SolidBrush* m_brush;
    const tex::Font*     m_font;
    tex::TeXRender*      m_render;

    float            m_sx;
    float            m_sy;

    HDC         m_dcMem;
    HBITMAP     m_hBitmap;
    HFONT       m_hFont;
    PVOID       m_buffer;

private:

    //
    // Tex2D graphic callbacks
    //
    virtual void setColor(tex::color c);

    virtual tex::color getColor() const;

    virtual void setStroke(const tex::Stroke& s);

    virtual const tex::Stroke& getStroke() const;

    virtual void setStrokeWidth(float w);

    virtual const tex::Font* getFont() const;

    virtual void setFont(const tex::Font* font);

    virtual void translate(float dx, float dy);

    virtual void scale(float sx, float sy);

    virtual void rotate(float angle);

    virtual void rotate(float angle, float px, float py);

    virtual void reset();

    virtual float sx() const;

    virtual float sy() const;

    virtual void drawChar(wchar_t c, float x, float y);

    virtual void drawText(const std::wstring& c, float x, float y);

    virtual void drawLine(float x1, float y1, float x2, float y2);

    virtual void drawRect(float x, float y, float w, float h);

    virtual void fillRect(float x, float y, float w, float h);

    virtual void drawRoundRect(float x, float y, float w, float h, float rx, float ry);

    virtual void fillRoundRect(float x, float y, float w, float h, float rx, float ry);


    BOOL PrepareGdiCanvas();

public:
    CTeXText(CLogCallback* logcallback);
    virtual ~CTeXText();

    BOOL Setup(COLORREF color = RGB(0xFF, 0xFF, 0xFF), int fontSize = 12);

    void Update(const TCHAR *newText);

    void Draw(const RECT& clientRect, int x, int y);

};

#endif // __OPENGL_TEXTEXT_H__