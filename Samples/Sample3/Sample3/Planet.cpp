
#include "pch.h"
#include "framework.h"
#include "Planet.h"


CPlanet::CPlanet()
{
    m_color = RGB(0, 0, 0);
    m_radius = 16;
    m_speed = 0.01;
    m_orbitRadius1 = 0;
    m_orbitRadius2 = 0;
    m_currentAngle = 0.0;
    m_orbitAngle = 0.0;
}

BOOL
CPlanet::Init()
{
    return TRUE;
}

void
CPlanet::DrawOrbite(CDC& dc, CRect& drawRect, CRect& boundingRect)
{
    CPoint   center;

    center.x = drawRect.Width() / 2;
    center.y = drawRect.Height() / 2;

    double   fi = 0.0;

    while (fi < 6.28)
    {


        //
        // rotated ellipse on the angle thetta
        // 
        // LONG X = (LONG)(m_orbitRadius1 * cos(fi) - m_orbitRadius2 * sin(fi));
        //
        //

        LONG X = (LONG)(m_orbitRadius1 * cos(fi) * cos(m_orbitAngle) - m_orbitRadius2 * sin(fi) * sin(m_orbitAngle));
        LONG Y = (LONG)(m_orbitRadius1 * cos(fi) * sin(m_orbitAngle) + m_orbitRadius2 * sin(fi) * cos(m_orbitAngle));

        X += center.x;
        Y += center.y;

        dc.SetPixel(X, Y, m_color);

        fi += 0.01;
    }

}

void
CPlanet::Draw(CDC& dc, CRect& drawRect, CRect& boundingRect)
{

    DrawOrbite(dc, drawRect, boundingRect);

    CPoint   center;

    center.x = drawRect.Width() / 2;
    center.y = drawRect.Height() / 2;


    //
    // rotated ellipse on the angle thetta
    // 
    LONG Xplanet = (LONG)(m_orbitRadius1 * cos(m_currentAngle) * cos(m_orbitAngle) - m_orbitRadius2 * sin(m_currentAngle) * sin(m_orbitAngle));
    LONG Yplanet = (LONG)(m_orbitRadius1 * cos(m_currentAngle) * sin(m_orbitAngle) + m_orbitRadius2 * sin(m_currentAngle) * cos(m_orbitAngle));


    Xplanet += center.x;
    Yplanet += center.y;


    CPen    drawPenGraph2(PS_SOLID, 1, m_color);
    CBrush  drawBrush2;
    drawBrush2.CreateSolidBrush(m_color);

    CPen   *oldPen = dc.SelectObject(&drawPenGraph2);
    CBrush *oldBrush = dc.SelectObject(&drawBrush2);

    CRect   planetRect;

    LONG  r_div2 = (LONG)(m_radius / 2);

    planetRect.left = Xplanet - r_div2;
    planetRect.right = Xplanet + r_div2;
    planetRect.top = Yplanet - r_div2;
    planetRect.bottom = Yplanet + r_div2;

    dc.Ellipse(&planetRect);


    dc.SelectObject(oldBrush);
    dc.SelectObject(oldPen);

}

void
CPlanet::Update(double speedFactor)
{
}

CPlanet::~CPlanet()
{
}


//
// Earth planet specific
//

CEarth::CEarth()
{
    m_color = RGB(56, 131, 52);
    m_radius = 16;
    m_speed = 0.01;


    m_orbitRadius1 = 149.5 / 2; // велика піввісь  149 598 261 км
                                // ексентриситтет  0,01671123

    double e = 0.01671123;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 0.0;
    m_orbitAngle = 2.0;
}

BOOL
CEarth::Init()
{
    return TRUE;
}

void
CEarth::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.25;
}

//
// Mars planet specific
//
CMars::CMars()
{
    m_color = RGB(169, 67, 46);
    m_radius = 15;
    m_speed = 0.02;

    m_orbitRadius1 = 227.9 / 2; // велика піввісь  227 943 820
                            // ексентриситтет 0,0933941

    double e = 0.0933941;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 1.0;
    m_orbitAngle = 1.2;
}

BOOL
CMars::Init()
{
    return TRUE;
}

void
CMars::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.005;
}


//
// Jupiter planet specific
//
CJupiter::CJupiter()
{
    m_color = RGB(160, 116, 61);
    m_radius = 28;
    m_speed = 0.005;

    m_orbitRadius1 = 778.5 / 2; // велика піввісь  778 547 200 
                                // ексентриситтет 0,0484

    double e = 0.0484;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 3.0;
    m_orbitAngle = 2.0;

}

BOOL
CJupiter::Init()
{
    return TRUE;
}

void
CJupiter::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.005;
}
