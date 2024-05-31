
#include "pch.h"
#include "framework.h"
#include "Planet.h"

#include "resource.h"


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
CPlanet::DrawOrbite(DWORD* canvas, CRect& canvasSize, CRect& drawRect)
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

        LONG X = (LONG)(m_orbitRadius1 * m_math.cos(fi) * m_math.cos(m_orbitAngle) - m_orbitRadius2 * m_math.sin(fi) * m_math.sin(m_orbitAngle));
        LONG Y = (LONG)(m_orbitRadius1 * m_math.cos(fi) * m_math.sin(m_orbitAngle) + m_orbitRadius2 * m_math.sin(fi) * m_math.cos(m_orbitAngle));

        X += center.x;
        Y += center.y;

        if (Y < 0 || X < 0) {

            fi += 0.01;
            continue;
        }

        if (Y < (LONG)canvasSize.bottom &&
            X < (LONG)canvasSize.right)
        {

            canvas[Y * canvasSize.Width() + X] = m_color;
        }

        fi += 0.01;
    }

}

void
CPlanet::DrawFilledCircle(DWORD* canvas, CRect& canvasSize, CRect& drawRect)
{

    DWORD Radius = drawRect.Width() / 2;
    DWORD* drawOffset = canvas + (drawRect.top * canvasSize.Width());
    //
    // Y loop
    //
    for (LONG i = 0; i < drawRect.Height(); i++)
    {
        double fi = asin(((double)Radius - i) / (double)Radius);

        DWORD X = (DWORD)(Radius * m_math.cos(fi));

        LONG X1 = drawRect.left + Radius - X;
        LONG X2 = drawRect.left + Radius + X;

        if ((i + drawRect.top) > canvasSize.bottom)
            break;

        if (X1 < canvasSize.left)
            break;

        //
        // X loop
        //
        LONG j = X1;

        while (j < X2)
        {
            if (j > canvasSize.right)
                break;

            *(drawOffset + j) = m_color;

            j++;
        }

        drawOffset += canvasSize.Width();
    }
}

void
CPlanet::Draw(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect)
{

    DrawOrbite(canvas, canvasDimensions, drawRect);

    CPoint   center;

    center.x = drawRect.Width() / 2;
    center.y = drawRect.Height() / 2;


    //
    // rotated ellipse on the angle thetta
    // 
    LONG Xplanet = (LONG)(m_orbitRadius1 * m_math.cos(m_currentAngle) * m_math.cos(m_orbitAngle) - m_orbitRadius2 * m_math.sin(m_currentAngle) * m_math.sin(m_orbitAngle));
    LONG Yplanet = (LONG)(m_orbitRadius1 * m_math.cos(m_currentAngle) * m_math.sin(m_orbitAngle) + m_orbitRadius2 * m_math.sin(m_currentAngle) * m_math.cos(m_orbitAngle));


    Xplanet += center.x;
    Yplanet += center.y;

    CRect   planetRect;

    LONG  r_div2 = (LONG)(m_radius / 2);

    planetRect.left = Xplanet - r_div2;
    planetRect.right = Xplanet + r_div2;
    planetRect.top = Yplanet - r_div2;
    planetRect.bottom = Yplanet + r_div2;

    if (m_Planet.IsLoaded())
    {
        m_Planet.Draw(canvas, canvasDimensions, planetRect, FALSE);
    }
    else
    {
        DrawFilledCircle(canvas, canvasDimensions, planetRect);
    }

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
    m_speed = 0.017;


    m_orbitRadius1 = 149.5 / 2; // велика піввісь  149 598 261 км
                                // ексентриситтет  0,01671123

    double e = 0.01671123;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 0.0;
    m_orbitAngle = 0.0;
}

BOOL
CEarth::Init()
{
    m_Planet.Init(MAKEINTRESOURCE(IDR_EARTH), _T("RT_JPG"));
    return TRUE;
}

void
CEarth::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.22;
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
    m_Planet.Init(MAKEINTRESOURCE(IDR_MARS), _T("RT_JPG"));
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
    m_Planet.Init(MAKEINTRESOURCE(IDR_JUPITER), _T("RT_JPG"));

    return TRUE;
}

void
CJupiter::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.005;
}


//
// Saturn planet specific
//
CSaturn::CSaturn()
{
    m_color = RGB(53, 149, 137);
    m_radius = 26;
    m_speed = 0.007;

    m_orbitRadius1 = 1000 / 2; // велика піввісь  1 429 394 069
                                // ексентриситтет 0,0484

    //double e = 0.5;
    double e = 0.055723219;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 3.5;
    m_orbitAngle = 2.0;

}

BOOL
CSaturn::Init()
{
    m_Planet.Init(MAKEINTRESOURCE(IDR_SATURN), _T("RT_JPG"));

    return TRUE;
}

void
CSaturn::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.005;
}
