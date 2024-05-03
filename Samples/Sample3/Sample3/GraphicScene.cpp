
#include "pch.h"
#include "framework.h"
#include "GraphicScene.h"
#include "Planet.h"

CGraphicScene::CGraphicScene()
{

}

BOOL
CGraphicScene::Init()
{
    CPlanet* planet = new CEarth();

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

    return TRUE;
}

void
CGraphicScene::DrawCrosshair(CDC& paintDc, CRect& rect)
{

    CPoint  start, end;

    start.x = rect.Width() / 2;
    start.y = 0;

    end = start;
    end.y = rect.Height();



    paintDc.MoveTo(start);
    paintDc.LineTo(end);


    start.x = 0;
    start.y = rect.Height() / 2;

    end = start;
    end.x = rect.Width();

    paintDc.MoveTo(start);
    paintDc.LineTo(end);

}


void
CGraphicScene::Draw(CDC& dc, CRect& drawRect, CRect& boundingRect)
{

    DrawCrosshair(dc, drawRect);

    //
    // draw Sun
    //
    CPen    drawPenGraph(PS_SOLID, 1, RGB(0xFF, 0xFF, 0x00));
    CBrush  drawBrush;
    drawBrush.CreateSolidBrush(RGB(0xFF, 0xFF, 0x00));

    CPen* oldPen = dc.SelectObject(&drawPenGraph);
    CBrush* oldBrush = dc.SelectObject(&drawBrush);

    CPoint   center;

    center.x = drawRect.Width() / 2;
    center.y = drawRect.Height() / 2;

    CRect   solarRect;

    solarRect.left = center.x - 50;
    solarRect.right = center.x + 50;
    solarRect.top = center.y - 50;
    solarRect.bottom = center.y + 50;

    dc.Ellipse(&solarRect);

    dc.SelectObject(oldBrush);
    dc.SelectObject(oldPen);




    for (_planets i = m_Planetarium.begin(); i != m_Planetarium.end(); i++)
    {
        CPlanet* planet = *i;

        planet->Draw(dc, drawRect, boundingRect);
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

}
