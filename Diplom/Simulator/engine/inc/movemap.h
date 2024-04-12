/***************************************************************************************\
*   File:                                                                               *
*       movemap.h                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       sprite renderer                                                                 *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __MOVEMAP_H__
#define __MOVEMAP_H__

class IMoveMap : public IObject
{
protected:
    unsigned int   m_width;
    unsigned int   m_height;

    class  ITexture *m_pTexture;
    COLORREF   *m_pHeightBuffer;

    IMoveMap(class ITexture *texture)
        : m_pTexture(texture)
        , m_pHeightBuffer(0)
    {
    }
public:
    virtual BOOL Init(RECT* dimensions, COLORREF baseColor = RGB(0, 0, 0)) = 0;

    virtual COLORREF GetValue(unsigned int x, unsigned int y) = 0;
};

class CMoveMap : public IMoveMap
{
private:

    BOOL Actualize(COLORREF baseColor);
public:
    CMoveMap(class ITexture *texture)
        : IMoveMap(texture)
    {
    }

    virtual BOOL Init(RECT* dimensions, COLORREF baseColor = RGB(0, 0, 0));
    virtual void Free();
    virtual DWORD GetValue(unsigned int x, unsigned int y);
};

#endif // __MOVEMAP_H__