#pragma once
#ifndef __GRAPHIC_SCENE_H__
#define __GRAPHIC_SCENE_H__

#include "Object.h"
#include "Planet.h"
#include <list>


class CGraphicScene : public CBaseObject
{

private:

    std::list<CPlanet*>   m_Planetarium;

    typedef std::list<CPlanet*>::iterator  _planets;

    void DrawCrosshair(CDC& paintDc, CRect& rect);


public:
    CGraphicScene();

    virtual BOOL Init();
    virtual void Draw(CDC& dc, CRect& drawRect, CRect& boundingRect);
    virtual void Update(double speedFactor);

    virtual ~CGraphicScene();

};


#endif // __GRAPHIC_SCENE_H__