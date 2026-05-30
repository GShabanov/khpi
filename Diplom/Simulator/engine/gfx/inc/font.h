/***************************************************************************************\
*   File:                                                                               *
*       font.h                                                                          *
*                                                                                       *
*   Abstract:                                                                           *
*       draw text on user output                                                        *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 2-Apr-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 54 $
#ifndef __FONT_H__
#define __FONT_H__

class IFont : public IDisposable
{
protected:

    FONT_INFO         m_fontInfo;
    unsigned int      m_PreferedWidth;

    IFont(unsigned int preferedWidth)
        : IDisposable()
        , m_PreferedWidth(preferedWidth)
    {
        memset(&m_fontInfo, 0, sizeof(&m_fontInfo));
    }

public:
    virtual BOOL Init(class IDataStream* pStream) = 0;

    virtual BOOL DrawCharacter(ISurface* surface, UCHAR alpha, LONG x, LONG y, COLORREF rgb, COLORREF transparency = 0) = 0;

    virtual BOOL PrintF(ISurface* surface, LONG x, LONG y, COLORREF rgb, COLORREF transparency, TCHAR* format, ...) = 0;

    unsigned int Width() { return m_fontInfo.PixelWidth; };
    unsigned int Height() { return m_fontInfo.PixelHeight; };
};

class CFont : public IFont
{
protected:


    class IDataStream *m_pFontStorage;
public:
	CFont(unsigned int preferedWidth)
		: IFont(preferedWidth)
        , m_pFontStorage(0)
	{
	}

    virtual BOOL Init(class IDataStream* pStream);
    virtual void Free();

    virtual void* Reference();
    virtual BOOL Dereference();
    virtual void WaitDereference(HANDLE hWaitObject);

    virtual BOOL DrawCharacter(ISurface* surface, UCHAR alpha, LONG x, LONG y, COLORREF rgb, COLORREF transparency);
    virtual BOOL PrintF(ISurface* surface, LONG x, LONG y, COLORREF rgb, COLORREF transparency, TCHAR* format, ...);
};

#endif // __FONT_H__
