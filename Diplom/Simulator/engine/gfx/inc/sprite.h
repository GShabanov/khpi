/***************************************************************************************\
*   File:                                                                               *
*       sprite.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       sprite renderer                                                                 *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Id:  $
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "bresenham.h"

typedef struct _TEXEL {
    LONG  x;
    LONG  y;
    LONG  z;
} TEXEL, *PTEXEL;
typedef struct _VERTEX {
    TEXEL coord;
    float  z1;
    float  uz;
    float  vz;
    float  u;  // texture U
    float  v;  // texture V
} VERTEX, *PVERTEX;

class Coord2D {
public:
    Coord2D() throw() {};
    POINT coord;
    POINT   capturedAt;
    BOOL    bCaptured;
    Coord2D(LONG _x, LONG _y) throw() {
        coord.x = _x; coord.y = _y;
        capturedAt.x = 0; capturedAt.y = 0;
    }
};

class Coord3D {
public:
    Coord3D() throw() {};

    TEXEL    m_coord;
    POINT    m_capturedAt;
    BOOL     m_bCaptured;

    TEXEL& getCoords()
    {
        return m_coord;
    }

    Coord3D(LONG _x, LONG _y, LONG _z) throw() {
        m_coord.x = _x; m_coord.y = _y; m_coord.z = _z;
        m_capturedAt.x = 0; m_capturedAt.y = 0;
        m_bCaptured = FALSE;
    }
};

class ISprite : public IDisposable
{
protected:

    class  ITexture *m_pTexture;

    ISprite(class ITexture *texture)
        : IDisposable()
        , m_pTexture(texture)
    {
    }
public:

    virtual ~ISprite() { };

    virtual void Draw(ISurface* surface, RECT* dimensions, BOOL drawBackground, COLORREF transparency = 0) = 0;

    virtual  class ITexture* GetTextureReference() = 0;
};

class ISprite3D : public IDisposable
{
protected:

    class  ITexture *m_pTexture;
    SIZE_T ObjSize() { return sizeof(*this); };

    ISprite3D(class ITexture *texture)
        : IDisposable()
        , m_pTexture(texture)
    {
    }
public:

    virtual void Draw(ISurface* surface, VERTEX* vertexes, BOOL drawBackground, COLORREF transparency = 0) = 0;
    virtual void Draw4(ISurface* surface, VERTEX* vertexes, BOOL drawBackground, COLORREF transparency = 0) = 0;
    virtual class ITexture* GetTextureReference() = 0;
};

class CSprite : public ISprite
{

public:
    CSprite(class ITexture *texture)
        : ISprite(texture)
    {
    }

    virtual BOOL Init();
    virtual void Free();


    virtual void Draw(ISurface* surface, RECT* dimensions, BOOL drawBackground, COLORREF transparency);
    virtual class ITexture* GetTextureReference();
};

class CSprite3D : public ISprite3D
{

    void DrawHLine();
public:
    CSprite3D(class ITexture *texture)
        : ISprite3D(texture)
    {
    }

    virtual BOOL Init();
    virtual void Free();

    virtual void Draw(ISurface* surface, VERTEX* dimensions, BOOL drawBackground, COLORREF transparency);
    void Draw4(ISurface* surface, VERTEX* vertexes, BOOL drawBackground, COLORREF transparency);
    virtual class ITexture* GetTextureReference();
};

#endif // __SPRITE_H__