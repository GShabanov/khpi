/***************************************************************************************\
*   File:                                                                               *
*       TeXtext.cpp                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       LaTeX renderer implementation for OpenGL                                        *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    24-Jul-2024                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "TeXtext.h"
#include <xstring>
#include <memory>

using namespace tex;

/**************************************************************************************************/

int tex::Font_win32::convertStyle(int style) {
    int s;
    switch (style) {
    case PLAIN:
        s = Gdiplus::FontStyleRegular;
        break;
    case BOLD:
        s = Gdiplus::FontStyleBold;
        break;
    case ITALIC:
        s = Gdiplus::FontStyleItalic;
        break;
    case BOLDITALIC:
        s = Gdiplus::FontStyleBoldItalic;
        break;
    default:
        s = -1;
        break;
    }
    return s;
}

const Gdiplus::FontFamily* tex::Font_win32::m_serif;
const Gdiplus::FontFamily* tex::Font_win32::m_sansserif;

tex::Font_win32::~Font_win32()
{
    if (m_family == m_serif || m_family == m_sansserif) {
        return;
    }

    delete m_family;
}

tex::Font_win32::Font_win32() 
    : m_size(0) 
    , m_family(nullptr)
    , m_style(0)
{}

tex::Font_win32::Font_win32(const std::string& name, int style, float size) 
    : m_size(size)
{
    if (m_serif == nullptr) {
        m_serif = Gdiplus::FontFamily::GenericSerif();
        m_sansserif = Gdiplus::FontFamily::GenericSansSerif();
    }

    const Gdiplus::FontFamily* f = nullptr;

    if (name == "Serif" || name == "SansSerif") {
        if (name == "Serif") {
            f = m_serif;
        }
        else {
            f = m_sansserif;
        }
    }
    else
    {
        std::wstring wname = utf82wide(name.c_str());
        f = new Gdiplus::FontFamily(wname.c_str());
    }

    int s = convertStyle(style);

    if (!f->IsStyleAvailable(s)) {
        throw ex_invalid_state("specified font style not available!");
    }

    Gdiplus::Font* tf = new Gdiplus::Font(f, m_size, s, Gdiplus::UnitPixel);

    m_style = s;
    m_family = f;
    m_typeface = sptr<Gdiplus::Font>(tf);
}

tex::Font_win32::Font_win32(const std::string& file, float size)
{

    // add font to font collection
    // some like fontconfig
    Gdiplus::PrivateFontCollection c;
    std::wstring wfile = utf82wide(file.c_str());

    c.AddFontFile(wfile.c_str());

    Gdiplus::FontFamily* ff = new Gdiplus::FontFamily();

    int num = 0;
    c.GetFamilies(1, ff, &num);
    if (num <= 0) {
        throw ex_invalid_state("cannot load font file " + file);
    }

    // search order :
    // regular -> bold -> italic -> bold-italic
    m_size = size;
    m_family = ff;
    Gdiplus::Font* f = nullptr;

    if (ff->IsStyleAvailable(Gdiplus::FontStyleRegular)) {

        f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        m_style = Gdiplus::FontStyleRegular;
    }
    else if (ff->IsStyleAvailable(Gdiplus::FontStyleBold)) {

        f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
        m_style = Gdiplus::FontStyleBold;
    }
    else if (ff->IsStyleAvailable(Gdiplus::FontStyleItalic)) {

        f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);
        m_style = Gdiplus::FontStyleItalic;
    }
    else if (ff->IsStyleAvailable(Gdiplus::FontStyleBoldItalic)) {

        f = new Gdiplus::Font(ff, size, Gdiplus::FontStyleBoldItalic, Gdiplus::UnitPixel);
        m_style = Gdiplus::FontStyleBoldItalic;
    }
    else {
        throw ex_invalid_state("no available font in file " + file);
    }

    m_typeface = sptr<Gdiplus::Font>(f);
}

float
tex::Font_win32::getSize() const
{
    return m_size;
}

std::shared_ptr<tex::Font>
tex::Font_win32::deriveFont(int style) const
{
    int s = convertStyle(style);
    if (!m_family->IsStyleAvailable(s)) {
        throw ex_invalid_state("specified font style not available!");
    }

    tex::Font_win32* f = new tex::Font_win32();
    f->m_family = m_family;
    f->m_style = s;
    f->m_size = m_size;
    Gdiplus::Font* ff = new Gdiplus::Font(&(*m_family), m_size, s, Gdiplus::UnitPixel);
    f->m_typeface = sptr<Gdiplus::Font>(ff);
    return sptr<tex::Font>(f);
}

bool
tex::Font_win32::operator==(const Font& ft) const
{
    const tex::Font_win32& f = static_cast<const tex::Font_win32&>(ft);
    return m_typeface.get() == f.m_typeface.get() && m_size == f.m_size;
}

bool
tex::Font_win32::operator != (const Font& f) const
{
    return !(*this == f);
}

tex::Font*
tex::Font::create(const std::string& file, float s) {
    return new Font_win32(file, s);
}

std::shared_ptr<tex::Font>
tex::Font::_create(const std::string& name, int style, float size)
{
    return sptrOf<Font_win32>(name, style, size);
}

/**************************************************************************************************/

Gdiplus::Graphics* tex::TextLayout_win32::m_g = nullptr;
Gdiplus::Bitmap* tex::TextLayout_win32::m_img = nullptr;
const Gdiplus::StringFormat* tex::TextLayout_win32::m_format = nullptr;

tex::TextLayout_win32::TextLayout_win32(const std::wstring& src, const sptr<Font_win32>& font)
    : m_txt(src)
    , m_font(font)
{
    if (m_img == nullptr) {
        m_img = new Gdiplus::Bitmap(1, 1, PixelFormat32bppARGB);
        m_g = Gdiplus::Graphics::FromImage(m_img);
        m_format = Gdiplus::StringFormat::GenericTypographic();
    }
}

void
tex::TextLayout_win32::getBounds(Rect& r)
{
    int em = m_font->m_family->GetEmHeight(m_font->m_style);
    int ascent = m_font->m_family->GetCellAscent(m_font->m_style);
    float ap = m_font->getSize() * ascent / em;

    Gdiplus::RectF r1;
    m_g->MeasureString(
        m_txt.c_str(), (INT)wcslen(m_txt.c_str()), &(*m_font->m_typeface), Gdiplus::PointF(0, 0), m_format, &r1);

    r.x = 0;
    r.y = -ap;
    r.w = r1.Width;
    r.h = r1.Height;
}

void tex::TextLayout_win32::draw(Graphics2D& g2, float x, float y)
{
    const tex::Font* prev = g2.getFont();
    g2.setFont(m_font.get());
    g2.drawText(m_txt, x, y);
    g2.setFont(prev);
}

std::shared_ptr<tex::TextLayout>
tex::TextLayout::create(const std::wstring& src, const std::shared_ptr<tex::Font>& font)
{

    std::shared_ptr<tex::Font_win32> f = std::static_pointer_cast<tex::Font_win32>(font);

    return sptrOf<tex::TextLayout_win32>(src, f);
}

const Gdiplus::StringFormat* CTeXText::m_format = nullptr;
const tex::Font* CTeXText::m_defaultFont = nullptr;


CTeXText::CTeXText(CLogCallback* logcallback)
    : m_log(logcallback)
    , m_width(800)
    , m_height(600)
    , m_textureID(0)
    , m_text(NULL)
    , m_buffer(NULL)
    , m_color(RGB(0xFF, 0xFF, 0xFF))
    , m_hFont(NULL)
    , m_hBitmap(NULL)
    , m_g(NULL)
    , m_brush(NULL)
    , m_pen(NULL)
{
}

CTeXText::~CTeXText()
{
    if (m_text != NULL)
        free(m_text);

    if (m_hBitmap)
        DeleteObject(m_hBitmap);

    if (m_hFont)
        DeleteObject(m_hFont);

    glDeleteTextures(1, &m_textureID);

    if (!m_brush)
    {
        delete m_brush;
    }

    if (!m_pen)
    {
        delete m_pen;
    }

    if (!m_g)
    {
        delete m_g;
    }

}

BOOL
CTeXText::Setup(COLORREF color, int fontSize)
{
    m_color = color;

    tex::LaTeX::init();

    PrepareGdiCanvas();

    m_sx = m_sy = 1.f;
    m_color = tex::black;

    if (m_format == nullptr) {
        m_format = Gdiplus::StringFormat::GenericTypographic();
    }

    if (m_defaultFont == nullptr) {
        m_defaultFont = new tex::Font_win32("Arial", tex::PLAIN, 72.f);
    }


    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return TRUE;
}

void
CTeXText::Update(const TCHAR *newText)
{
    if (!newText)
        return;


    TCHAR *newtext = _tcsdup(newText);

    if (!newtext)
        return;

    if (m_text) {
        free(m_text);
    }

    if (m_g)
        delete m_g;

    if (m_g)
        delete m_g;

    PrepareGdiCanvas();

    m_g = new Gdiplus::Graphics(m_dcMem);

    if (!m_g) {
        return;
    }

    m_pen = new Gdiplus::Pen(Gdiplus::Color(m_color));

    if (!m_pen)
        return;

    m_brush = new Gdiplus::SolidBrush(Gdiplus::Color(m_color));

    if (!m_brush)
    {
        delete m_pen;
        return;
    }

    m_sx = m_sy = 1.f;
    m_color = tex::black;
    m_font = m_defaultFont;

    m_g->ResetTransform();
    m_g->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    m_g->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    m_text = newtext;

    m_render = tex::LaTeX::parse(m_text, m_width, 26, 26 / 3.f, 0xff424242);

    m_render->draw(*this, 10, 10);

    //
    // transparency update
    //
    BYTE* pixels = (BYTE*)m_buffer;

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

void
CTeXText::Draw(const RECT& clientRect, int x, int y)
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


BOOL
CTeXText::PrepareGdiCanvas()
{

    HDC     dcMem;
    HBITMAP hBitmap;

    HDC hdcScreen = GetDC(NULL);

    if (!hdcScreen)
        return FALSE;

    dcMem = CreateCompatibleDC(hdcScreen);

    ReleaseDC(NULL, hdcScreen);

    if (!dcMem)
        return FALSE;


    // retrive pixels
    BITMAPINFO bmpInfo = { 0 };
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = m_width;
    bmpInfo.bmiHeader.biHeight = -m_height;  // inverted
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;


    hBitmap = CreateDIBSection(dcMem, &bmpInfo, DIB_RGB_COLORS, &m_buffer, NULL, 0);

    if (hBitmap == NULL)
    {
        DeleteDC(dcMem);
        dcMem = NULL;
        return FALSE;
    }

    HBITMAP hBitmapOld = (HBITMAP)SelectObject(dcMem, hBitmap);

    if (hBitmapOld) {
        DeleteObject(hBitmapOld);
    }

    //
    // update
    //
    if (m_dcMem != NULL) {
        DeleteDC(m_dcMem);
    }

    if (m_hBitmap != NULL) {
        DeleteObject(m_hBitmap);
    }

    m_dcMem = dcMem;
    m_hBitmap = hBitmap;

    return TRUE;
}


void
CTeXText::setColor(tex::color color)
/**
 * Set the color of the context
 *
 * @param c required color
 */
{
    m_color = color;
    m_pen->SetColor(Gdiplus::Color(color));
    m_brush->SetColor(Gdiplus::Color(color));
}

tex::color
CTeXText::getColor() const
/** Get the color of the context */
{
    return m_color;
}

void 
CTeXText::setStroke(const tex::Stroke& s)
/** Set the stroke of the context */
{
    m_stroke = s;
    m_pen->SetWidth(s.lineWidth);
    Gdiplus::LineCap c;
    switch (s.cap) {
    case tex::CAP_BUTT:
        c = Gdiplus::LineCapFlat;
        break;
    case tex::CAP_ROUND:
        c = Gdiplus::LineCapRound;
        break;
    case tex::CAP_SQUARE:
        c = Gdiplus::LineCapSquare;
        break;
    }

    m_pen->SetLineCap(c, c, Gdiplus::DashCapRound);

    Gdiplus::LineJoin j;

    switch (s.join) {
    case tex::JOIN_BEVEL:
        j = Gdiplus::LineJoinBevel;
        break;

    case tex::JOIN_ROUND:
        j = Gdiplus::LineJoinRound;
        break;

    case tex::JOIN_MITER:
        j = Gdiplus::LineJoinMiter;
        break;
    }

    m_pen->SetLineJoin(j);
    m_pen->SetMiterLimit(s.miterLimit);

}

const
tex::Stroke&
CTeXText::getStroke() const
/** Get the stroke of the context */
{
    return m_stroke;
}

void
CTeXText::setStrokeWidth(float w)
/** Set the stroke width of the context */
{
    m_stroke.lineWidth = w;
    m_pen->SetWidth(w);

}

const tex::Font*
CTeXText::getFont() const
/** Get the current font */
{
    return m_font;
}

void
CTeXText::setFont(const tex::Font* font)
/** Set the font of the context */
{
    m_font = font;
}

void
CTeXText::translate(float dx, float dy)
/**
 * Translate the context with distance dx, dy
 *
 * @param dx distance in x-direction to translate
 * @param dy distance in y-direction to translate
 */
{
    m_g->TranslateTransform(dx, dy);
}

void
CTeXText::scale(float sx, float sy)
/**
 * Scale the context with sx, sy
 *
 * @param sx scale ratio in x-direction
 * @param sy scale ratio in y-direction
 */
{
    m_sx *= sx;
    m_sy *= sy;
    m_g->ScaleTransform(sx, sy);
}

void
CTeXText::rotate(float angle)
/**
 * Rotate the context with the given angle (in radian), with pivot (0, 0).
 *
 * @param angle angle (in radian) amount to rotate
 */
{
    m_g->RotateTransform(angle / 3.14159f * 180);
}

void
CTeXText::rotate(float angle, float px, float py)
/**
 * Rotate the context with the given angle (in radian), with pivot (px, py).
 *
 * @param angle angle (in radian) amount to rotate
 * @param px pivot in x-direction
 * @param py pivot in y-direction
 */
{
    m_g->TranslateTransform(px, py);
    m_g->RotateTransform(angle / 3.14159f * 180);
    m_g->TranslateTransform(-px, -py);
}

void
CTeXText::reset()
/** Reset transformations */
{
    m_g->ResetTransform();
    m_sx = m_sy = 1.f;
}

float
CTeXText::sx() const
/**
 * Get the scale of the context in x-direction
 *
 * @return the scale in x-direction
 */
{
    return m_sx;
}

float
CTeXText::sy() const
/**
 * Get the scale of the context in y-direction
 *
 * @return the scale in y-direction
 */
{
    return m_sy;
}

void
CTeXText::drawChar(wchar_t c, float x, float y)
/**
 * Draw character, is baseline aligned
 *
 * @param c specified character
 * @param x x-coordinate
 * @param y y-coordinate, is baseline aligned
 */
{
    wchar_t str[]{ c, L'\0' };
    drawText(str, x, y);
}

void
CTeXText::drawText(const std::wstring& c, float x, float y)
/**
 * Draw text, is baseline aligned
 *
 * @param c specified text
 * @param x x-coordinate
 * @param y y-coordinate, is baseline aligned
 */
{
    const wchar_t* str = c.c_str();
    const tex::Font_win32* f = (const tex::Font_win32*)m_font;
    int em = f->m_family->GetEmHeight(f->m_style);

    int ascent = f->m_family->GetCellAscent(f->m_style);

    float ap = f->getSize() * ascent / em;
    int len = wcslen(str);
    Gdiplus::RectF r1, r2;
    m_g->MeasureString(str, len, &(*f->m_typeface), Gdiplus::PointF(0, 0), &r1);
    m_g->MeasureString(str, len, &(*f->m_typeface), Gdiplus::PointF(0, 0), m_format, &r2);
    float off = (r1.Width - r2.Width) / 2.f;
    m_g->DrawString(str, len, &(*f->m_typeface), Gdiplus::PointF(x - off, y - ap), m_brush);
}

void
CTeXText::drawLine(float x1, float y1, float x2, float y2)
/**
 * Draw line
 *
 * @param x1 start point in x-direction
 * @param y1 start point in y-direction
 * @param x2 end point in x-direction
 * @param y2 end point in y-direction
 */
{
    m_g->DrawLine(m_pen, x1, y1, x2, y2);
}

void
CTeXText::drawRect(float x, float y, float w, float h)
/**
 * Draw rectangle
 *
 * @param x left position
 * @param y top position
 * @param w width
 * @param h height
 */
{
    m_g->DrawRectangle(m_pen, x, y, w, h);
}

void
CTeXText::fillRect(float x, float y, float w, float h)
/**
 * Fill rectangle
 *
 * @param x left position
 * @param y top position
 * @param w width
 * @param h height
 */
{
    m_g->FillRectangle(m_brush, x, y, w, h);
}

void
CTeXText::drawRoundRect(float x, float y, float w, float h, float rx, float ry)
/**
 * Draw round rectangle
 *
 * @param x left position
 * @param y top position
 * @param w width
 * @param h height
 * @param rx radius in x-direction
 * @param ry radius in y-direction
 */
{
    drawRect(x, y, w, h);
}

void
CTeXText::fillRoundRect(float x, float y, float w, float h, float rx, float ry)
/**
 * Fill round rectangle
 *
 * @param x left position
 * @param y top position
 * @param w width
 * @param h height
 * @param rx radius in x-direction
 * @param ry radius in y-direction
 */
{
    fillRect(x, y, w, h);
}
