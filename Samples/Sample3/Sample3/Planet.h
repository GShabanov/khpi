#pragma once
#ifndef __PLANET_H__
#define __PLANET_H__

#include "Object.h"

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

private:
    void  DrawOrbite(CDC& dc, CRect& drawRect, CRect& boundingRect);

public:
    CPlanet();

    virtual BOOL Init();
    virtual void Draw(CDC& dc, CRect& drawRect, CRect& boundingRect);
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

#endif // __PLANET_H__