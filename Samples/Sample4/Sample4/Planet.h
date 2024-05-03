#pragma once
#ifndef __PLANET_H__
#define __PLANET_H__

#include "Object.h"
#include "Sprite.h"

class CPlanet : public CBaseObject
{
protected:

    LONG        m_radius;
    double      m_speed;     // radial speed

    COLORREF    m_color;

    double      m_currentAngle;
    double      m_orbitAngle;

    double      m_orbitRadius1;
    double      m_orbitRadius2;

    CSprite     m_Planet;

private:
    void DrawOrbite(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect);
    void DrawFilledCircle(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect);

public:
    CPlanet();

    virtual BOOL Init();
    virtual void Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect);
    virtual void Update(double speedFactor);

    virtual ~CPlanet();

};


class CEarth : public CPlanet
{
public:
    virtual BOOL Init();
    virtual void Update(double speedFactor);
    CEarth();

};


class CMars : public CPlanet
{
public:
    virtual BOOL Init();
    virtual void Update(double speedFactor);
    CMars();

};

class CJupiter : public CPlanet
{
public:
    virtual BOOL Init();
    virtual void Update(double speedFactor);
    CJupiter();
};

class CSaturn : public CPlanet
{
public:
    virtual BOOL Init();
    virtual void Update(double speedFactor);
    CSaturn();
};

#endif // __PLANET_H__