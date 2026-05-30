/***************************************************************************************\
*   File:                                                                               *
*       texture.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       texture container                                                               *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 63 $
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class ITexture : public IDisposable
{
protected:

    class IDataStream *m_pComressedStream;
    void              *m_pBufferRaw;
    unsigned int       m_Width;
    unsigned int       m_Height;
    COLORREF           m_bgColor;

    ITexture()
        : IDisposable()
    {
    }

public:
    virtual BOOL Init(IDataStream* pStream) = 0;

    COLORREF BgColor() { return this->m_bgColor; };
    unsigned int Width() { return this->m_Width; };
    unsigned int Height() { return this->m_Height; };
};

class CTexture : public ITexture
{
protected:


public:
    CTexture()
        : ITexture()
    {
    }

    virtual BOOL Init(IDataStream* pStream);
    virtual void Free();
    virtual void* Reference();
    virtual BOOL Dereference();
    virtual void WaitDereference(HANDLE hWaitObject);
};

#endif // __TEXTURE_H__