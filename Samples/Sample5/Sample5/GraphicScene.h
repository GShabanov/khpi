#pragma once
#ifndef __GRAPHIC_SCENE_H__
#define __GRAPHIC_SCENE_H__

#include "Object.h"
#include "Planet.h"
#include "Sprite.h"
#include <list>

#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

class CGraphicObject
{
protected:

    CDC                             m_memoryDC;
    CBitmap                         m_memoryBitmap;

    PVOID                           m_rawPixels;

public:
    DWORD* GetPixels() {
        return (DWORD*)m_rawPixels;
    }

};



class CGraphicScene : public CBaseObject
{

private:

    std::list<CPlanet*>   m_Planetarium;

    typedef std::list<CPlanet*>::iterator  _planets;

    CSprite               m_Background;
    CSprite               m_Sun;

    void DrawCrosshair(DWORD* canvas, CRect& canvasSize);



public:

    CGraphicScene();



    virtual BOOL Init();
    virtual void Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect);
    virtual void Update(double speedFactor);

    virtual ~CGraphicScene();

};


#endif // __GRAPHIC_SCENE_H__