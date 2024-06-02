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

    double      m_scaleFactor;

    CSprite     m_Planet;

private:
    void DrawOrbite(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect);
    void DrawFilledCircle(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect);

public:
    CPlanet();

    void   SetScaleFactor(double  scaleFactor) {

        m_scaleFactor = scaleFactor;
    }

    virtual BOOL Init();
    virtual void Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect);
    virtual void Update(double speedFactor);

    virtual ~CPlanet();

};

class CMercury : public CPlanet
{
public:
    CMercury();

    virtual BOOL Init();
    virtual void Update(double speedFactor);

};

class CVenus : public CPlanet
{
public:
    CVenus();

    virtual BOOL Init();
    virtual void Update(double speedFactor);

};

class CEarth : public CPlanet
{
public:
    CEarth();

    virtual BOOL Init();
    virtual void Update(double speedFactor);

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


class CUranus : public CPlanet
{
public:
    virtual BOOL Init();
    virtual void Update(double speedFactor);
    CUranus();
};

class CNeptun : public CPlanet
{
public:
    virtual BOOL Init();
    virtual void Update(double speedFactor);
    CNeptun();
};

#endif // __PLANET_H__