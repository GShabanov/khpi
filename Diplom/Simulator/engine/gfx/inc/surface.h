/***************************************************************************************\
*   File:                                                                               *
*       surface.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       surface access members and interfaces                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 19-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Id:  $
#ifndef __SURFACE_H__
#define __SURFACE_H__

typedef unsigned int Units;

class ISurface : public IObject
{

protected:

    Units m_width;
    Units m_height;
    Units m_bits;

    ISurface(Units width, Units height, int bits) noexcept;

public:
    virtual BOOL Init(HDC hDC) = 0;

    Units Width() {
        return this->m_width;
    }

    Units Height() {
        return this->m_height;
    }

    virtual void SetPixel(Units x, Units y, COLORREF color) = 0;

    virtual void BlendPixel(Units x, Units y, COLORREF color, COLORREF blend) = 0;

    virtual COLORREF GetPixel(Units x, Units y) = 0;

    virtual void SetPixels(Units x, Units y, COLORREF* colors, Units count) = 0;

    virtual Units GetPixels(Units x, Units y, COLORREF* colors, Units count) = 0;

    virtual void Cleanup(COLORREF color) = 0;

    virtual void Blt(HDC hDC, RECT* lpRect) = 0;

    virtual void Line(POINT &from, POINT &to, COLORREF color) = 0;

    virtual void Circle(POINT& position, Units radius, COLORREF color) = 0;

};

class CSurfaceAPI : public ISurface
{
private:
    HDC		m_BackBufferDc;


public:

    CSurfaceAPI(unsigned int width, unsigned int height, int bits) noexcept
        : ISurface(width, height, bits)
        , m_BackBufferDc(NULL)
    {
    }

    virtual BOOL Init(HDC hDC);
    virtual void Free();
    virtual void SetPixel(Units x, Units y, COLORREF color);
    virtual void BlendPixel(Units x, Units y, COLORREF color, COLORREF blend);
    virtual COLORREF GetPixel(unsigned int x, unsigned int y);
    virtual void Cleanup(COLORREF color);
    virtual void Blt(HDC hDC, RECT* lpRect);

    virtual void Line(POINT& from, POINT& to, COLORREF color);
    virtual void Circle(POINT& position, Units radius, COLORREF color);
};

class CSurfaceRAW : public ISurface
{
private:


    void			   *m_buffer;
    HBITMAP				m_hbmp;
    BITMAPINFO          m_bmpInfo;


    //
    // 640x480x32
    //
    void SetPixel640xNx32(unsigned int x, unsigned int y, COLORREF color);
    void BlendPixel640xNx32(unsigned int x, unsigned int y, COLORREF color, COLORREF blend);
    COLORREF GetPixel640xNx32(unsigned int x, unsigned int y);

public:

    CSurfaceRAW(unsigned int width, unsigned int height, int bits)
        : ISurface(width, height, bits)
        , m_buffer(NULL)
        , m_hbmp(NULL)
    {
        memset(&m_bmpInfo, 0, sizeof(BITMAPINFO));
    }

    virtual BOOL Init(HDC hDC);
    virtual void Free();
    //
    // overall
    //
    virtual void SetPixel(Units x, Units y, COLORREF color);
    virtual void BlendPixel(Units x, Units y, COLORREF color, COLORREF blend);
    virtual COLORREF GetPixel(Units x, Units y);
    virtual void SetPixels(Units x, Units y, COLORREF* colors, unsigned int count);
    virtual unsigned int GetPixels(Units x, Units y, COLORREF* colors, unsigned int count);
    virtual void Cleanup(COLORREF color);
    virtual void Blt(HDC hDC, RECT* lpRect);

    virtual void Line(POINT& from, POINT& to, COLORREF color);
    virtual void Circle(POINT& position, Units radius, COLORREF color);
};

class CSurfaceDirectX : public ISurface
{
private:

    void			   *m_buffer;
    HBITMAP				m_hbmp;
    BITMAPINFO          m_bmpInfo;


    //
    // 640x480x32
    //
    void iSetPixel640xNx32(unsigned int x, unsigned int y, COLORREF color);
    void iBlendPixel640xNx32(unsigned int x, unsigned int y, COLORREF color, COLORREF blend);
    COLORREF iGetPixel640xNx32(unsigned int x, unsigned int y);

    void iCleanup(COLORREF color);
    void iBlt(HDC hDC, RECT *lpRect);

public:

    CSurfaceDirectX(unsigned int width, unsigned int height, int bits)
        : ISurface(width, height, bits)
        , m_buffer(NULL)
        , m_hbmp(NULL)
    {
    }

    virtual BOOL Init(HDC hDC);
    virtual void Free();
    //
    // overall
    //
    virtual void SetPixel(unsigned int x, unsigned int y, COLORREF color);
    virtual void BlendPixel(unsigned int x, unsigned int y, COLORREF color, COLORREF blend);
    virtual COLORREF GetPixel(unsigned int x, unsigned int y);
    virtual void SetPixels(unsigned int x, unsigned int y, COLORREF* colors, unsigned int count);
    virtual unsigned int GetPixels(unsigned int x, unsigned int y, COLORREF* colors, unsigned int count);
};
#endif // __SURFACE_H__