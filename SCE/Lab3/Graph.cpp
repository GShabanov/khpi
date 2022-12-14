/***************************************************************************************\
*   File:                                                                               *
*       Graph.cpp                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#include "pch.h"
#include "framework.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  CHART_BACKGROUND_COLOR     RGB(0x20, 0x20, 0x20)
#define  CHART_AXIS_COLOR           RGB(0xFF, 0xFF, 0x20)
#define  CHART_GRID_COLOR           RGB(0x50, 0x50, 0x50)
#define  CHART_NAMES_COLOR          RGB(0x20, 0xFF, 0x20)
#define  CHART_GRAPH_COLOR          RGB(0x20, 0x20, 0xFF)
#define  CHART_CROSSPOINT_OFFSET    10
#define  CHART_ARROW_LENGTH         4


//---------------------------------------------------------------------------
//
// CHART CONTROL IMPLEMENTAION
//
//---------------------------------------------------------------------------

// CChartControlApp
BEGIN_MESSAGE_MAP(CGraphControl, CWnd)
   //{{AFX_MSG_MAP(CGraphControl)
   ON_WM_PAINT()
   ON_WM_SIZE()
   ON_WM_MOVING()
   ON_WM_CONTEXTMENU()
   ON_WM_DESTROY()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGraphControl construction

CGraphControl::CGraphControl()
    : CWnd()
    , m_absolute(false)
{
    m_graphs.SetSize(4);

    for (int i = 0; i < 4; i++)
    {
        m_graphs[i] = CGraph();
    }

    m_horizontalAxisName = "X";
    m_vericalAxisName = "Y";
    m_chartName = "";
}

CGraphControl::~CGraphControl()
{
    m_vericalAxisName.Empty();
    m_horizontalAxisName.Empty();
    m_chartName.Empty();
    m_graphs.RemoveAll();

}

void
CGraphControl::OnDestroy()
{
    CWnd::OnDestroy();

    //_ballon.DestroyWindow();
    m_axisFont.DeleteObject();
}

BOOL
CGraphControl::Create(
    DWORD dwStyle, const CRect& rect, CWnd* pParentWnd)
{

    if (CWnd::Create(NULL, NULL, dwStyle | WS_CHILD, rect, pParentWnd, NULL, NULL) != TRUE)
    {
        return FALSE;
    }
    
    //CRect   ballonRect = rect;

    //ballonRect.InflateRect(ballonRect.Width() / 8, ballonRect.Height() / 8);
    //_ballon.Create(NULL, this);
    //_ballon.ShowWindow(SW_SHOW);

    SetOwner(pParentWnd);
    
    this->UpdateData(FALSE);

    HGDIOBJ  hDefaultGuiFont = reinterpret_cast<HFONT>(
        ::GetStockObject(DEFAULT_GUI_FONT));

    LOGFONT logfont;
    GetObject(hDefaultGuiFont, sizeof(logfont), &logfont);

    m_axisFont.CreateFontW(-12, 5, 0, 0, FW_THIN, 0, 0, 0, 0, 0, 0, 0, 
                DEFAULT_PITCH | FF_DONTCARE, logfont.lfFaceName);

    this->Invalidate(TRUE);

    return TRUE;
}

void
CGraphControl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
    UNREFERENCED_PARAMETER(pWnd);
    UNREFERENCED_PARAMETER(pos);

    this->Default();
}

void
CGraphControl::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    CRect   wndRect;
    this->ClientToScreen(wndRect);

}

void
CGraphControl::UpdateChart(int index, const CGraph &controlGraph)
{
    INT_PTR chartsSize = m_graphs.GetSize();
    ASSERT(index <= chartsSize);

    if (index > chartsSize)
        return;

    m_chartUpdateLock.Lock();

    m_graphs.SetAt(index, controlGraph);

    m_chartUpdateLock.Unlock();

    if (this->IsWindowVisible())
    {
        this->Invalidate(TRUE);
    }
}

void
CGraphControl::DrawAxis(CDC *cdc, const CRect &rect, const GRAPH_MINMAX& minMax)
{
    CPen *oldPen;
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;



    //
    // iteration constans for vertical and horizontal points
    //
    horisontalAspectRatio = (float)rect.Width() / (float)abs(minMax.max_x - minMax.min_x);
    verticalAspectRatio = (float)rect.Height() / (float)abs(minMax.max_y - minMax.min_y);

    CPoint  crossPoint(rect.left, rect.bottom);

    //
    // in case we have negative values move axis to 0
    //
    if (minMax.min_x < 0) {

        crossPoint.x -= (LONG)(minMax.min_x * horisontalAspectRatio);
    }

    if (minMax.min_y < 0) {

        crossPoint.y += (LONG)(minMax.min_y * verticalAspectRatio);
    }

    LONG  xStep = rect.Width() / 8;
    LONG  yStep = rect.Height() / 8;

    CPoint  verticalAxis(crossPoint);
    CPoint  horizontalAxis(crossPoint);

    crossPoint.x += CHART_CROSSPOINT_OFFSET;
    crossPoint.y -= CHART_CROSSPOINT_OFFSET;


    CPen  gridPen(PS_DOT, 1, CHART_GRID_COLOR);

    cdc->SetTextColor(CHART_GRID_COLOR);

    oldPen = cdc->SelectObject(&gridPen);

    CPoint  gridAxis(crossPoint);

    gridAxis.x += xStep;

    CString  textOut;

    int i = 1;
    //
    // vertical grids
    //
    while (gridAxis.x < (rect.right - CHART_ARROW_LENGTH))
    {

        cdc->MoveTo(gridAxis);

        cdc->LineTo(gridAxis.x, rect.top + (CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH));

        textOut.Format(L"%2.1f", (minMax.max_x / 7) * i);

        cdc->TextOutW(gridAxis.x, gridAxis.y + 2, textOut);

        i++;
        gridAxis.x += xStep;
    }

    gridAxis = CPoint(crossPoint);


    gridAxis.y -= yStep;

    i = 1;
    //
    // horizontal grids
    //
    while (gridAxis.y > (rect.top + CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH))
    {

        cdc->MoveTo(gridAxis);

        cdc->LineTo(rect.right - (CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH), gridAxis.y);

        textOut.Format(L"%2.0f", (minMax.max_y / 7) * i);

        cdc->TextOutW(rect.left, gridAxis.y, textOut);

        i++;
        gridAxis.y -= yStep;
    }



    verticalAxis.x += CHART_CROSSPOINT_OFFSET;
    horizontalAxis.y -= CHART_CROSSPOINT_OFFSET;

    //
    // draw axis
    //
    CPen  drawPen(PS_SOLID, 1, CHART_AXIS_COLOR);
    cdc->SelectObject(&drawPen);


    cdc->MoveTo(verticalAxis);

    cdc->LineTo(verticalAxis.x, rect.top + CHART_ARROW_LENGTH);                                 // from down to top line
    cdc->LineTo(verticalAxis.x + 1, rect.top + (CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH)); // right arrow part
    cdc->MoveTo(verticalAxis.x, rect.top + CHART_ARROW_LENGTH);
    cdc->LineTo(verticalAxis.x - 1, rect.top + (CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH)); // left arrow  part

    cdc->MoveTo(horizontalAxis);
    cdc->LineTo(rect.right - CHART_ARROW_LENGTH, horizontalAxis.y);    // from left to right
    cdc->LineTo(rect.right - (CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH), horizontalAxis.y - 1); // top arrow part
    cdc->MoveTo(rect.right - CHART_ARROW_LENGTH, horizontalAxis.y);
    cdc->LineTo(rect.right - (CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH), horizontalAxis.y + 1); // bottom arrow part

    //
    // print axis names
    //
    cdc->SetTextColor(CHART_NAMES_COLOR);

    cdc->TextOutW(verticalAxis.x + CHART_ARROW_LENGTH, rect.top + CHART_ARROW_LENGTH + CHART_CROSSPOINT_OFFSET, m_vericalAxisName);
    cdc->TextOutW(rect.right - (CHART_CROSSPOINT_OFFSET + m_horizontalAxisName.GetLength() * 5), horizontalAxis.y + 2, m_horizontalAxisName);

    cdc->SelectObject(&oldPen);
}

void
CGraphControl::DrawCircle(CDC* cdc, const CPoint& coordinates, SIZE_T size)
{
    CRect shape;

    //
    // Shape Coordinates
    //
    shape.left = coordinates.x - (LONG)size;
    shape.top = coordinates.y - (LONG)size;
    shape.right = coordinates.x + (LONG)size;
    shape.bottom = coordinates.y + (LONG)size;

    cdc->Ellipse(shape);
}

void
CGraphControl::DrawPointsGraph(
    CDC *cdc,
    CGraph &graph,
    const GRAPH_MINMAX &minMax,
    const CRect  &drawRect,
    const CPoint &normalizationPoint,
    const CPoint &relocationPoint
    )
{
    //
    // calculate zoom aspect ratios
    //
    INT_PTR  elementsCount = graph.m_chartValues.GetCount();
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;

    if (elementsCount <= 0)
    {
        return;
    }

    //
    // absolute maximum or local maximum for graph
    //
    if (m_absolute == true)
    {
        //
        // we use global min max from all graph
        //

        horisontalAspectRatio = (double)(drawRect.right) / abs(minMax.max_x - minMax.min_x);
        verticalAspectRatio = (double)(drawRect.bottom) / abs(minMax.max_y - minMax.min_y);

    }
    else
    {

        //
        // we use local min max from this graph
        //
        double maxXValue = graph.getMaxX();
        double maxYValue = graph.getMaxY();
        double minXValue = graph.getMinX();
        double minYValue = graph.getMinY();

        horisontalAspectRatio = (double)(drawRect.right) / abs(maxXValue - minXValue);
        verticalAspectRatio = (double)(drawRect.bottom) / abs(maxYValue - minYValue);
    }


    CPen    drawPenGraph(PS_SOLID, 1, graph.m_chartColor);
    CBrush  drawBrush;
    drawBrush.CreateSolidBrush(graph.m_chartColor);

    //CHusBrush  drawBrush(BS_SOLID, graph.m_chartColor);
    CPen   *oldPen = cdc->SelectObject(&drawPenGraph);
    CBrush *oldBrush = cdc->SelectObject(&drawBrush);

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel;

        //
        // invert coordinate for Y and left as is for X
        //
        pixel.x = (LONG)(graph.m_chartValues[i].x * horisontalAspectRatio + (horisontalAspectRatio * normalizationPoint.x));
        pixel.y = drawRect.bottom - (LONG)(graph.m_chartValues[i].y * verticalAspectRatio + (verticalAspectRatio * normalizationPoint.y));

        pixel += relocationPoint;


        DrawCircle(cdc, pixel, 3);
    }

    cdc->SelectObject(oldBrush);
    cdc->SelectObject(oldPen);
}

void
CGraphControl::DrawLinedGraph(
    CDC* cdc,
    CGraph& graph,
    const GRAPH_MINMAX& minMax,
    const CRect& drawRect,
    const CPoint& normalizationPoint,
    const CPoint& relocationPoint
    )
{
    //
    // calculate zoom aspect ratios
    //
    INT_PTR  elementsCount = graph.m_chartValues.GetCount();
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;

    if (elementsCount <= 0)
    {
        return;
    }

    //
    // absolute maximum or local maximum for graph
    //
    if (m_absolute == true)
    {
        //
        // we use global min max from all graph
        //

        horisontalAspectRatio = (double)drawRect.right / abs(minMax.max_x - minMax.min_x);
        verticalAspectRatio = (double)drawRect.bottom / abs(minMax.max_y - minMax.min_y);
    }
    else
    {

        //
        // we use local min max from this graph
        //
        double maxXValue = graph.getMaxX();
        double maxYValue = graph.getMaxY();
        double minXValue = graph.getMinX();
        double minYValue = graph.getMinY();

        horisontalAspectRatio = (double)drawRect.right / abs(maxXValue - minXValue);
        verticalAspectRatio = (double)drawRect.bottom / abs(maxYValue - minYValue);
    }


    CPoint  pointForDot;
    CPoint  pointForDotRelocatated;
    CPen    drawPenGraph(PS_SOLID, 1, graph.m_chartColor);
    CPen* oldPen = cdc->SelectObject(&drawPenGraph);

    /*for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel = CPoint(graph.m_chartValues[i]);

        pixel += normalizationPoint;

        //
        // invert coordinate for Y and left as is for X
        //
        pixel.x = (LONG)(pixel.x * horisontalAspectRatio);
        pixel.y = drawRect.bottom - (LONG)(pixel.y * verticalAspectRatio);

        pixel += relocationPoint;

    }
    //pointForDot.x = 0;
    //pointForDot.y = innerRect.bottom - (LONG)(graph.m_chartValues[0].y * verticalAspectRatio);

    pointForDotRelocatated = pointForDot;
    pointForDotRelocatated += relocationVector;
    cdc->MoveTo(pointForDotRelocatated);

    for (INT_PTR i = 1; i < elementsCount; i++)
    {
        pointForDot.x = (LONG)(i * horisontalAspectRatio);
        pointForDot.y = innerRect.bottom - (LONG)(graph.m_chartValues[i].y * verticalAspectRatio);

        pointForDotRelocatated = pointForDot;
        pointForDotRelocatated += relocationVector;

        cdc->LineTo(pointForDotRelocatated);
    }*/

    cdc->SelectObject(oldPen);
}

void
CGraphControl::DrawPixelGraph(
    CDC* cdc,
    CGraph& graph,
    const GRAPH_MINMAX& minMax,
    const CRect& drawRect,
    const CPoint& normalizationPoint,
    const CPoint& relocationPoint
)
{
    //
    // calculate zoom aspect ratios
    //
    INT_PTR  elementsCount = graph.m_chartValues.GetCount();
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;

    if (elementsCount <= 0)
    {
        return;
    }

    //
    // absolute maximum or local maximum for graph
    //
    if (m_absolute == true)
    {
        //
        // we use global min max from all graph
        //

        horisontalAspectRatio = (double)drawRect.right / abs(minMax.max_x - minMax.min_x);
        verticalAspectRatio = (double)drawRect.bottom / abs(minMax.max_y - minMax.min_y);
    }
    else
    {

        //
        // we use local min max from this graph
        //
        double maxXValue = graph.getMaxX();
        double maxYValue = graph.getMaxY();
        double minXValue = graph.getMinX();
        double minYValue = graph.getMinY();

        horisontalAspectRatio = (double)drawRect.right / abs(maxXValue - minXValue);
        verticalAspectRatio = (double)drawRect.bottom / abs(maxYValue - minYValue);
    }


    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel;

        //
        // invert coordinate for Y and left as is for X
        //
        pixel.x = (LONG)(graph.m_chartValues[i].x * horisontalAspectRatio + (horisontalAspectRatio * normalizationPoint.x));
        pixel.y = drawRect.bottom - (LONG)(graph.m_chartValues[i].y * verticalAspectRatio + (verticalAspectRatio * normalizationPoint.y));


        pixel += relocationPoint;


        cdc->SetPixel(pixel.x, pixel.y, graph.m_chartColor);
    }

}

void
CGraphControl::Draw(CDC  *cdc)
/*
    draw control
*/
{
    LONG position = 0;
    CRect   clientRect;
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;


    GetClientRect(&clientRect);

    CFont  *oldFont = cdc->SelectObject(&m_axisFont);
    //
    // fill background
    //
    cdc->FillSolidRect(&clientRect, CHART_BACKGROUND_COLOR);

    //
    // make work area
    //
    clientRect.InflateRect(0, -5);

    //
    // copy chart array to local buffer, so _chartValues can be freely updates 
    //
    CArray<CGraph>  graphLocal;

    m_chartUpdateLock.Lock();

    graphLocal.Copy(m_graphs);

    m_chartUpdateLock.Unlock();

    LONG  textLength = 0;
    GRAPH_MINMAX  minmaxRect = { 0, 0, 0, 0 };

    //
    // build min max rect
    //
    for (INT_PTR i = 0; i < graphLocal.GetSize(); i++)
    {
        if (graphLocal[i].IsVisible())
        {

            double value = graphLocal[i].getMaxX();

            if (value > minmaxRect.max_x) {

                minmaxRect.max_x = value;
            }

            value = graphLocal[i].getMaxY();

            if (value > minmaxRect.max_y) {

                minmaxRect.max_y = value;
            }

            value = graphLocal[i].getMinX();

            if (value < minmaxRect.min_x) {

                minmaxRect.min_x = value;
            }

            value = graphLocal[i].getMinY();

            if (value < minmaxRect.min_y) {

                minmaxRect.min_y = value;
            }

        }
    }

    //
    // calculate offset for negative coordinates
    //
    CPoint  normalizationPoint = CPoint(0, 0);

    if (minmaxRect.min_x < 0) {

        normalizationPoint.x = (LONG)-minmaxRect.min_x;
    }

    if (minmaxRect.min_y < 0) {

        normalizationPoint.y = (LONG)-minmaxRect.min_y;
    }



    this->DrawAxis(cdc, clientRect, minmaxRect);

    if (this->m_chartName.GetLength() > 0)
    {
        CPen   drawPenGraph(PS_SOLID, 1, CHART_AXIS_COLOR);
        cdc->SetTextColor(CHART_NAMES_COLOR);

        CPen   *oldPen = cdc->SelectObject(&drawPenGraph);

        position = clientRect.Width() / 2 - (this->m_chartName.GetLength() * 6 / 2);

        if (position < 0)
            position = 0;

        cdc->TextOutW(position, 2, this->m_chartName);

        cdc->SelectObject(oldPen);
    }


    CPoint  crossPoint(clientRect.left, clientRect.bottom);
    CPoint  verticalAxis(crossPoint);
    CPoint  horizontalAxis(crossPoint);

    ULONG   graphWidth = (ULONG)clientRect.Width() - (2 * CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH) - 2;
    ULONG   graphHeight = (ULONG)clientRect.Height() - (2 * CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH) - 1 - 5;

    CPoint  relocationPoint(clientRect.left + CHART_CROSSPOINT_OFFSET + 1, (2 * CHART_CROSSPOINT_OFFSET + CHART_ARROW_LENGTH) + 1);
    CRect   drawRect = CRect(0, 0, graphWidth, graphHeight);

#if 0
    //
    // test for check where the work space
    //
    CRect   solidRect(innerRect);

    solidRect.TopLeft() += relocationVector;
    solidRect.BottomRight() += relocationVector;

    cdc->FillSolidRect(&solidRect, CHART_GRAPH_COLOR);
    return;
#endif

    for (INT_PTR i = 0; i < graphLocal.GetSize(); i++)
    {

        if (graphLocal[i].IsVisible())
        {

            switch (graphLocal[i].m_GraphType)
            {
            case CGraph::Type::DottedGraph:
                DrawPointsGraph(cdc, graphLocal[i], minmaxRect, drawRect, normalizationPoint, relocationPoint);
                break;

            case CGraph::Type::LinedGraph:
                DrawLinedGraph(cdc, graphLocal[i], minmaxRect, drawRect, normalizationPoint, relocationPoint);
                break;

            case CGraph::Type::PixelGraph:
                DrawPixelGraph(cdc, graphLocal[i], minmaxRect, drawRect, normalizationPoint, relocationPoint);
                break;

            default:;
            }

            textLength += graphLocal[i].m_graphName.GetLength();
            textLength += sizeof("#- ");
        }

    }
    //
    //  draw underscoore
    //
    position = clientRect.Width() / 2 - (textLength * 8 / 2);
    if (position < 0)
        position = 0;

    for (INT_PTR i = 0; i < graphLocal.GetSize(); i++)
    {

        if (graphLocal[i].IsVisible())
        {
            cdc->SetTextColor(graphLocal[i].m_chartColor);


            cdc->TextOutW(position, clientRect.Height() - 3, CString("# - ") + graphLocal[i].m_graphName);

            position += graphLocal[i].m_graphName.GetLength() * 8 + sizeof("#- ");

        }

    }

    cdc->SelectObject(oldFont);

    graphLocal.RemoveAll();
}

void 
CGraphControl::OnPaint()
/*
    WM_PAINT dispatcher
*/
{
    CWnd::OnPaint();

    CClientDC dc(this);
    CDC       memoryDC;

    CBitmap memoryBitmap;


    CRect windowRect;
    this->GetWindowRect(&windowRect);

    int  width = windowRect.Width();
    int  height = windowRect.Height();

    //
    // just not try at all to draw something
    //
    if ((width < 10) ||  (height < 10))
    {
        return;
    }

    //
    // create memory buffer for painting
    //
    memoryDC.CreateCompatibleDC(&dc);
    memoryBitmap.CreateCompatibleBitmap(&dc, width, height);

    CBitmap *oldBitmap = memoryDC.SelectObject(&memoryBitmap);

    //
    // draw in memory
    //
    Draw(&memoryDC);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &memoryDC, 0, 0, SRCCOPY);

    memoryDC.SelectObject(oldBitmap);

    memoryBitmap.DeleteObject();
    memoryDC.DeleteDC();
}

