
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
    m_scaleFactor = 1.0;
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

    double   orbitRadius1 = m_orbitRadius1 * m_scaleFactor;
    double   orbitRadius2 = m_orbitRadius2 * m_scaleFactor;

    while (fi < 6.28)
    {


        //
        // rotated ellipse on the angle thetta
        // 
        // LONG X = (LONG)(m_orbitRadius1 * cos(fi) - m_orbitRadius2 * sin(fi));
        //
        //

        LONG X = (LONG)(orbitRadius1 * m_math.cos(fi) * m_math.cos(m_orbitAngle) - orbitRadius2 * m_math.sin(fi) * m_math.sin(m_orbitAngle));
        LONG Y = (LONG)(orbitRadius1 * m_math.cos(fi) * m_math.sin(m_orbitAngle) + orbitRadius2 * m_math.sin(fi) * m_math.cos(m_orbitAngle));

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


    double   orbitRadius1 = m_orbitRadius1 * m_scaleFactor;
    double   orbitRadius2 = m_orbitRadius2 * m_scaleFactor;

    //
    // rotated ellipse on the angle thetta
    // 
    LONG Xplanet = (LONG)(orbitRadius1 * m_math.cos(m_currentAngle) * m_math.cos(m_orbitAngle) - orbitRadius2 * m_math.sin(m_currentAngle) * m_math.sin(m_orbitAngle));
    LONG Yplanet = (LONG)(orbitRadius1 * m_math.cos(m_currentAngle) * m_math.sin(m_orbitAngle) + orbitRadius2 * m_math.sin(m_currentAngle) * m_math.cos(m_orbitAngle));


    Xplanet += center.x;
    Yplanet += center.y;

    CRect   planetRect;

    double  radius = m_radius * m_scaleFactor;

    LONG  r_div2 = (LONG)(radius / 2);

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
// Mercury planet specific
//

CMercury::CMercury()
{
    m_color = RGB(56, 131, 52);
    m_radius = 24;              // radius 2439,7
    m_speed = 0.4736;           // speed  47,36


    m_orbitRadius1 = 57.9;      // велика піввісь  57 909 227 км
                                // ексентриситтет  0,01671123

    double e = 0.205630;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 0.0;
    m_orbitAngle = 0.0;
}

BOOL
CMercury::Init()
{
    m_Planet.Init(MAKEINTRESOURCE(IDR_MERCURY), _T("RT_JPG"));
    return TRUE;
}

void
CMercury::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.22;
}

//
// Venus planet specific
//

CVenus::CVenus()
{
    m_color = RGB(56, 131, 52);
    m_radius = 60;         // 6051,8
    m_speed = 0.3502;      // 35,02 км/с


    m_orbitRadius1 = 108.2; // велика піввісь  108 208 930 км
                            // ексентриситтет  0,01671123

    double e = 0.0068;
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 0.0;
    m_orbitAngle = 0.0;
}

BOOL
CVenus::Init()
{
    m_Planet.Init(MAKEINTRESOURCE(IDR_VENUS), _T("RT_JPG"));
    return TRUE;
}

void
CVenus::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.25;
}


//
// Earth planet specific
//

CEarth::CEarth()
{
    m_color = RGB(56, 131, 52);
    m_radius = 64;              // 6378,1 км
    m_speed = 0.29785;          // 29,783 км/c


    m_orbitRadius1 = 149.5;     // велика піввісь  149 598 261 км
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
    m_radius = 34;                  // 3396,2
    m_speed = 0.2413;               // 24,13 км/с

    m_orbitRadius1 = 227.9; // велика піввісь  227 943 820   // 2,2794382⋅108 км
                            // ексентриситтет 0,0933941

    double e = 0.0933941;   // 0,0933941
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
    m_radius = 714;          // 71 492
    m_speed = 0.1307;        // 13,07 км/с

    m_orbitRadius1 = 778.5; // велика піввісь  778 547 200 
                                // ексентриситтет 0,0484

    double e = 0.0484;   // 0,048775
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
    m_radius = 602;            // 60 268
    m_speed = 0.969;          // 9,69 км/с

    m_orbitRadius1 = 1429; // велика піввісь  1 429 394 069      
                                // ексентриситтет 0,0484

    double e = 0.055723219;      // 0,055723219
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


//
// Uranus planet specific
//
CUranus::CUranus()
{
    m_color = RGB(53, 149, 137);
    m_radius = 249;            // 24 973
    m_speed = 0.681;          // 6,81 км/с

    m_orbitRadius1 = 2876; // велика піввісь  2 876 679 082
                                // ексентриситтет 0,0484

    double e = 0.044405;      // 0,044 405 586
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 3.5;
    m_orbitAngle = 2.0;

}

BOOL
CUranus::Init()
{
    m_Planet.Init(MAKEINTRESOURCE(IDR_URANUS), _T("RT_JPG"));

    return TRUE;
}

void
CUranus::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.005;
}

//
// Neptun planet specific
//
CNeptun::CNeptun()
{
    m_color = RGB(53, 149, 137);
    m_radius = 247;          // 24 764
    m_speed = 0.5449;        // 5,4349 км/с

    m_orbitRadius1 = 4503;      // велика піввісь  4 503 443 661
                                // ексентриситтет 0,0484

    double e = 0.011214269;    // 0,011214269
    m_orbitRadius2 = m_orbitRadius1 * sqrt(1 - e * e);
    m_currentAngle = 3.5;
    m_orbitAngle = 2.0;

}

BOOL
CNeptun::Init()
{
    m_Planet.Init(MAKEINTRESOURCE(IDR_NEPTUN), _T("RT_JPG"));

    return TRUE;
}

void
CNeptun::Update(double speedFactor)
{
    m_currentAngle += (m_speed * speedFactor);
    m_orbitAngle = 0.005;
}
