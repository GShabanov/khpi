#pragma once
#ifndef __GRAPHIC_SCENE_H__
#define __GRAPHIC_SCENE_H__

#include "Object.h"
#include "Planet.h"
#include "Sprite.h"
#include <list>


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