
#include "pch.h"
#include "framework.h"
#include "GraphicScene.h"
#include "Planet.h"

#include "resource.h"

CMath    m_math;


CGraphicScene::CGraphicScene()
{
    m_scaleFactor = 1.0;
}

BOOL
CGraphicScene::Init()
{


    if (m_math.Init() != TRUE)
        return FALSE;



    CPlanet* planet = new CMercury();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);

    planet = new CVenus();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);


    planet = new CEarth();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);

    planet = new CMars();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);

    planet = new CJupiter();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);

    planet = new CSaturn();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);

    planet = new CUranus();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);

    planet = new CNeptun();

    if (planet == NULL)
        return FALSE;

    planet->Init();

    m_Planetarium.push_back(planet);


    //m_Background.Init(CString("res\\Space.jpg"));
    //m_Sun.Init(CString("res\\Sun.jpg"));

    m_Background.Init(MAKEINTRESOURCE(IDR_SPACE), _T("RT_JPG"));
    m_Sun.Init(MAKEINTRESOURCE(IDR_SUN), _T("RT_JPG"));

    return TRUE;
}

void
CGraphicScene::DrawCrosshair(DWORD* canvas, CRect& canvasSize)
{

    CPoint  start, end;

    const DWORD  color = RGB(0x80, 0x80, 0x80);


    start.x = 0;
    start.y = canvasSize.Height() / 2;

    end = start;
    end.x = canvasSize.Width();

    DWORD  *drawOffset = canvas + (start.y * canvasSize.Width());

    //
    // horizontal line
    //

    for (DWORD i = 0; ; )
    {
        //
        // color 
        //
        for (int j = 0; j < 3; j++)
        {
            *(drawOffset++) = color;
            i++;

            if (i >= (DWORD)canvasSize.Width())
                break;
        }

        //
        // blank
        //
        for (int j = 0; j < 3; j++)
        {
            i++;
            drawOffset++;

            if (i >= (DWORD)canvasSize.Width())
                break;
        }

        if (i >= (DWORD)canvasSize.Width())
            break;
    }


    start.x = canvasSize.Width() / 2;
    start.y = 0;

    end = start;
    end.y = canvasSize.Height();

    //
    // verical line
    //

    drawOffset = canvas + start.x;

    for (DWORD i = 0; ; )
    {
        //
        // color 
        //
        for (int j = 0; j < 3; j++)
        {
            *drawOffset = color;
            drawOffset += canvasSize.Width();

            i++;
            if (i >= (DWORD)canvasSize.Height())
                break;
        }

        //
        // blank
        //
        for (int j = 0; j < 3; j++)
        {
            i++;
            drawOffset += canvasSize.Width();

            if (i >= (DWORD)canvasSize.Height())
                break;


        }

        if (i >= (DWORD)canvasSize.Height())
            break;
    }

}


void
CGraphicScene::Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect)
{

    m_Background.Draw((DWORD *)canvas, canvasSize, drawRect, TRUE);

    //DrawCrosshair((DWORD*)canvas, canvasSize);

    CPoint   center;

    center.x = drawRect.Width() / 2;
    center.y = drawRect.Height() / 2;

    CRect   solarRect;

    solarRect.left   = center.x - (LONG)(50 * m_scaleFactor);
    solarRect.right  = center.x + (LONG)(50 * m_scaleFactor);
    solarRect.top    = center.y - (LONG)(50 * m_scaleFactor);
    solarRect.bottom = center.y + (LONG)(50 * m_scaleFactor);

    m_Sun.Draw((DWORD*)canvas, canvasSize, solarRect, FALSE);


    for (_planets i = m_Planetarium.begin(); i != m_Planetarium.end(); i++)
    {
        CPlanet* planet = *i;

        planet->Draw((DWORD*)canvas, canvasSize, drawRect);
    }


}

void 
CGraphicScene::SetScaleFactor(double  scaleFactor)
{
    m_scaleFactor = scaleFactor;

    for (_planets i = m_Planetarium.begin(); i != m_Planetarium.end(); i++)
    {
        CPlanet* planet = *i;

        planet->SetScaleFactor(scaleFactor);
    }
}


void
CGraphicScene::Update(double speedFactor)
{
    for (_planets i = m_Planetarium.begin(); i != m_Planetarium.end(); i++)
    {
        CPlanet* planet = *i;

        planet->Update(speedFactor);
    }

}

CGraphicScene::~CGraphicScene()
{
    

    while (!m_Planetarium.empty())
    {
        CPlanet* planet = m_Planetarium.front();

        delete planet;

        m_Planetarium.pop_front();
    }

    CBaseObject::~CBaseObject();

}
