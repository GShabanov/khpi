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
#include "MFCGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  CHART_BACKGROUND_COLOR     RGB(0x20, 0x20, 0x20)
#define  CHART_AXIS_COLOR           RGB(0xFF, 0xFF, 0x20)
#define  CHART_GRID_COLOR           RGB(0x50, 0x50, 0x50)
#define  CHART_NAMES_COLOR          RGB(0x50, 0xFF, 0x50)
#define  CHART_GRAPH_COLOR          RGB(0x20, 0x20, 0xFF)
#define  CHART_CROSSPOINT_OFFSET    16
#define  CHART_ARROW_LENGTH         12


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
    ON_WM_LBUTTONDOWN()
    ON_WM_CONTEXTMENU()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGraphControl construction

CGraphControl::CGraphControl()
    : CWnd()
    , m_absolute(false)
{
    //m_graphs.SetSize(0);

    m_horizontalAxisName = "X";
    m_vericalAxisName = "Y";
    m_chartName = "";
}

CGraphControl::~CGraphControl()
{
    m_vericalAxisName.Empty();
    m_horizontalAxisName.Empty();
    m_chartName.Empty();

    for (INT_PTR i = 0; i < this->m_graphs.GetCount(); i++)
    {
        delete m_graphs[i];
    }

    m_graphs.RemoveAll();
}

void
CGraphControl::OnDestroy()
{
    CWnd::OnDestroy();

    m_memoryBitmap.DeleteObject();
    m_memoryDC.DeleteDC();
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

    if (m_hWnd != NULL)
    {

        m_memoryDC.DeleteDC();
        m_memoryBitmap.DeleteObject();

        m_memoryDC.CreateCompatibleDC(this->GetDC());

        m_memoryBitmap.CreateCompatibleBitmap(this->GetDC(), cx, cy);

        CBitmap* oldBitmap = m_memoryDC.SelectObject(&m_memoryBitmap);

        oldBitmap->DeleteObject();
    }



    CRect   wndRect;
    this->ClientToScreen(wndRect);

}

void
CGraphControl::InsertGraph(int index, CGraph  &graph)
{

    m_chartUpdateLock.Lock();

    INT_PTR chartsSize = m_graphs.GetSize();

    ASSERT(index <= chartsSize);

    if (index > chartsSize) {

        m_chartUpdateLock.Unlock();
        return;
    }

    //if (index > m_graphs.GetSize() || m_graphs.GetSize() == 0)
    if (chartsSize == 0 || index > (chartsSize - 1))
    {
        m_graphs.InsertAt(index, graph.clone(graph));

    }
    else
    {
        delete m_graphs[index];
        m_graphs.SetAt(index, graph.clone(graph));

    }

    m_chartUpdateLock.Unlock();

    if (this->IsWindowVisible())
    {
        this->Invalidate(TRUE);
    }
}

void CGraphControl::DeleteGraph(int index)
{
    m_chartUpdateLock.Lock();

    INT_PTR chartsSize = m_graphs.GetSize();

    if (chartsSize == 0 || 
        index > chartsSize - 1) {

        m_chartUpdateLock.Unlock();
        return;
    }

    delete m_graphs[index];
    m_graphs.RemoveAt(index, 1);

    m_chartUpdateLock.Unlock();

    if (this->IsWindowVisible())
    {
        this->Invalidate(TRUE);
    }
}

CGraph::GRAPH_POINT
CGraphControl::GetPointFromClient(int x, int y)
{
    CRect   worksArrea;
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;

    //
    // copy chart array to local buffer, so _chartValues can be freely updates 
    //
    CArray<CGraph*>  graphLocal;

    GetClientRect(&worksArrea);

    //
    // make work area
    //
    worksArrea.DeflateRect(0, CHART_CROSSPOINT_OFFSET);
    worksArrea.top += CHART_ARROW_LENGTH;
    worksArrea.bottom -= CHART_CROSSPOINT_OFFSET;

    m_chartUpdateLock.Lock();

    graphLocal.Copy(m_graphs);

    m_chartUpdateLock.Unlock();

    GRAPH_MINMAX  minmax = { 0, 0, 0, 0 };

    BuildMinMax(graphLocal, minmax);

    //
    // iteration constans for vertical and horizontal points
    //
    horisontalAspectRatio = (float)worksArrea.Width() / (float)abs(minmax.max_x - minmax.min_x);
    verticalAspectRatio = (float)worksArrea.Height() / (float)abs(minmax.max_y - minmax.min_y);

    //
    // central point of the whole graph
    //
    CPoint  centralPoint(worksArrea.left, worksArrea.bottom);

    //
    // in case we have negative values move axis to 0
    //
    if (minmax.min_x < 0) {

        centralPoint.x -= (LONG)(minmax.min_x * horisontalAspectRatio);
    }

    if (minmax.min_y < 0) {

        centralPoint.y += (LONG)(minmax.min_y * verticalAspectRatio);
    }

    //
    // move central point if we are near to order dimensions
    //
    if (centralPoint.x < (worksArrea.left + CHART_CROSSPOINT_OFFSET))
    {
        centralPoint.x += CHART_CROSSPOINT_OFFSET;
    }

    if (centralPoint.y > (worksArrea.bottom - CHART_CROSSPOINT_OFFSET))
    {
        //centralPoint.y -= CHART_CROSSPOINT_OFFSET;
    }

    CGraph::GRAPH_POINT  a;

    a.x = (x - centralPoint.x) / horisontalAspectRatio;

    a.y = (centralPoint.y - y) / verticalAspectRatio;


    return a;
}

void
CGraphControl::OnLButtonDown(UINT nFlags, CPoint point)
{
    __super::OnLButtonDown(nFlags, point);

    CRect  worksArrea;

    GetClientRect(&worksArrea);

    //
    // make work area
    //
    worksArrea.DeflateRect(0, CHART_CROSSPOINT_OFFSET);
    worksArrea.top += CHART_ARROW_LENGTH;
    worksArrea.bottom -= CHART_CROSSPOINT_OFFSET;

    if (PtInRect(worksArrea, point))
    {

        CGraph::GRAPH_POINT  a = GetPointFromClient(point.x, point.y);

        GetParent()->SendMessage(WM_USER + 1, (WPARAM)&a, NULL);
    }
}

void
CGraphControl::BuildMinMax(const CArray<CGraph*> &graph, GRAPH_MINMAX& minmaxRect)
{

    minmaxRect.min_x = 0;
    minmaxRect.max_x = 0;
    minmaxRect.min_y = 0;
    minmaxRect.max_y = 0;

    //
    // initialize by first valid
    //
    for (INT_PTR i = 0, j = graph.GetSize(); i < j; i++)
    {
        if (graph[i]->IsVisible())
        {
            minmaxRect.min_x = graph[i]->getMinX();
            minmaxRect.max_x = graph[i]->getMaxX();
            minmaxRect.min_y = graph[i]->getMinY();
            minmaxRect.max_y = graph[i]->getMaxY();
            break;
        }
    }

    //
    // build min max rect
    //
    for (INT_PTR i = 0, j = graph.GetSize(); i < j; i++)
    {
        if (graph[i]->IsVisible())
        {

            double value = graph[i]->getMaxX();

            if (value > minmaxRect.max_x) {

                minmaxRect.max_x = value;
            }

            value = graph[i]->getMaxY();

            if (value > minmaxRect.max_y) {

                minmaxRect.max_y = value;
            }

            value = graph[i]->getMinX();

            if (value < minmaxRect.min_x) {

                minmaxRect.min_x = value;
            }

            value = graph[i]->getMinY();

            if (value < minmaxRect.min_y) {

                minmaxRect.min_y = value;
            }

        }
    }
}

void
CGraphControl::DrawAxis(CDC *cdc, const CRect &rect, const CPoint& centralPoint, const GRAPH_MINMAX& minMax)
{
    CPen *oldPen;
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;

    //
    // iteration constans for vertical and horizontal points
    //
    horisontalAspectRatio = (double)(rect.Width()) / abs(minMax.max_x - minMax.min_x);
    verticalAspectRatio = (double)(rect.Height()) / abs(minMax.max_y - minMax.min_y);


    CPen  gridPen(PS_DOT, 1, CHART_GRID_COLOR);

    cdc->SetTextColor(CHART_GRID_COLOR);

    oldPen = cdc->SelectObject(&gridPen);

    //
    // calculate overall grid lines for this dimension
    //
    LONG xGridLines = rect.Width() / 40; // 40 pixels by step
    LONG yGridLines = rect.Height() / 40; // 40 pixels by step

    if (xGridLines > 0)
    {
        LONG  xStep = rect.Width() / xGridLines;


        CPoint  gridAxis(rect.left, rect.bottom);

        gridAxis.x += xStep;

        CString  textOut;

        int i = 1;
        //
        // vertical grids
        //
        while (gridAxis.x < (rect.right - CHART_ARROW_LENGTH))
        {

            cdc->MoveTo(gridAxis);

            cdc->LineTo(gridAxis.x, rect.top);

            textOut.Format(L"%2.1f", minMax.min_x + ((minMax.max_x - minMax.min_x) / xGridLines) * i);

            cdc->TextOutW(gridAxis.x, gridAxis.y + 2, textOut);

            i++;
            gridAxis.x += xStep;
        }
    }

    if (yGridLines > 0)
    {

        LONG  yStep = rect.Height() / yGridLines;

        CPoint  gridAxis = CPoint(rect.left, rect.bottom);


        gridAxis.y -= yStep;

        CString  textOut;

        int i = 1;
        //
        // horizontal grids
        //
        while (gridAxis.y > rect.top)
        {

            cdc->MoveTo(gridAxis);

            cdc->LineTo(rect.right, gridAxis.y);

            textOut.Format(L"%2.1f", minMax.min_y + ((minMax.max_y - minMax.min_y) / yGridLines) * i);

            cdc->TextOutW(rect.left, gridAxis.y + 2, textOut);

            i++;
            gridAxis.y -= yStep;
        }
    }



    //verticalAxis.x += CHART_CROSSPOINT_OFFSET;
    //horizontalAxis.y -= CHART_CROSSPOINT_OFFSET;

    CPoint  verticalAxis(centralPoint.x, rect.bottom);
    CPoint  horizontalAxis(rect.left, centralPoint.y);

    //
    // draw axis
    //
    CPen  drawPen(PS_SOLID, 1, CHART_AXIS_COLOR);
    cdc->SelectObject(&drawPen);


    cdc->MoveTo(verticalAxis);

    cdc->LineTo(verticalAxis.x, rect.top - CHART_ARROW_LENGTH * 2);     // from down to top line
    cdc->LineTo(verticalAxis.x + 2, rect.top - CHART_ARROW_LENGTH); // right arrow part
    cdc->MoveTo(verticalAxis.x, rect.top - CHART_ARROW_LENGTH * 2);
    cdc->LineTo(verticalAxis.x - 2, rect.top - CHART_ARROW_LENGTH); // left arrow  part

    cdc->MoveTo(horizontalAxis);
    cdc->LineTo(rect.right, horizontalAxis.y);    // from left to right
    cdc->LineTo(rect.right - CHART_ARROW_LENGTH, horizontalAxis.y - 2); // top arrow part
    cdc->MoveTo(rect.right, horizontalAxis.y);
    cdc->LineTo(rect.right - CHART_ARROW_LENGTH, horizontalAxis.y + 2); // bottom arrow part

    //
    // print axis names
    //
    cdc->SetTextColor(CHART_NAMES_COLOR);

    cdc->TextOutW(verticalAxis.x + 4, rect.top - CHART_ARROW_LENGTH, m_vericalAxisName);
    cdc->TextOutW(rect.right - CHART_ARROW_LENGTH - m_horizontalAxisName.GetLength() * 5, horizontalAxis.y + 2, m_horizontalAxisName);

    cdc->SelectObject(&oldPen);
}

void
CGraphControl::DrawGraph(
    CDC *cdc,
    CGraph &graph,
    const GRAPH_MINMAX &minMax,
    const CRect  &drawRect,
    const CPoint &centralPoint
    )
{
    //
    // calculate zoom aspect ratios
    //
    INT_PTR  elementsCount = graph.GetCount();
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

        horisontalAspectRatio = (double)(drawRect.Width()) / abs(minMax.max_x - minMax.min_x);
        verticalAspectRatio = (double)(drawRect.Height()) / abs(minMax.max_y - minMax.min_y);

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

        horisontalAspectRatio = (double)(drawRect.Width()) / abs(maxXValue - minXValue);
        verticalAspectRatio = (double)(drawRect.Height()) / abs(maxYValue - minYValue);
    }

    graph.DrawGraph(cdc, drawRect, centralPoint, horisontalAspectRatio, verticalAspectRatio);

}

void
CGraphControl::Draw(CDC  *cdc)
/*
    draw control
*/
{
    LONG position = 0;
    CRect   worksArrea;
    double  verticalAspectRatio = 0.0;
    double  horisontalAspectRatio = 0.0;

    //
    // copy chart array to local buffer, so _chartValues can be freely updates 
    //
    CArray<CGraph*>  graphLocal;


    m_chartUpdateLock.Lock();

    graphLocal.Copy(m_graphs);

    m_chartUpdateLock.Unlock();



    GetClientRect(&worksArrea);

    CFont  *oldFont = cdc->SelectObject(&m_axisFont);
    //
    // fill background
    //
    cdc->FillSolidRect(&worksArrea, CHART_BACKGROUND_COLOR);

    LONG  textLength = 0;

    //
    // draw chart name
    //
    if (this->m_chartName.GetLength() > 0)
    {
        CPen   drawPenGraph(PS_SOLID, 1, CHART_AXIS_COLOR);
        cdc->SetTextColor(CHART_NAMES_COLOR);

        CPen* oldPen = cdc->SelectObject(&drawPenGraph);

        position = worksArrea.Width() / 2 - (this->m_chartName.GetLength() * 6 / 2);

        if (position < 0)
            position = 0;

        cdc->TextOutW(position, 0, this->m_chartName);

        cdc->SelectObject(oldPen);
    }

    for (INT_PTR i = 0, j = graphLocal.GetSize(); i < j; i++)
    {

        if (graphLocal[i]->IsVisible())
        {
            int length = graphLocal[i]->m_graphName.GetLength();

            if (length > 0)
            {
                textLength += length;
                textLength += sizeof("#- ");
            }
        }

    }

    //
    //  draw underscoore
    //
    position = worksArrea.Width() / 2 - (textLength * 8 / 2);
    if (position < 0)
        position = 0;

    for (INT_PTR i = 0, j = graphLocal.GetSize(); i < j; i++)
    {

        if (graphLocal[i]->IsVisible())
        {
            int length = graphLocal[i]->m_graphName.GetLength();

            if (length > 0)
            {
                cdc->SetTextColor(graphLocal[i]->m_chartColor);

                cdc->TextOutW(position, worksArrea.Height() - CHART_CROSSPOINT_OFFSET, CString("# - ") + graphLocal[i]->m_graphName);

                position += (length * 8 + sizeof("#- "));
            }

        }

    }


    //
    // make work area
    //
    worksArrea.DeflateRect(0, CHART_CROSSPOINT_OFFSET);
    worksArrea.top += CHART_ARROW_LENGTH;
    worksArrea.bottom -= CHART_CROSSPOINT_OFFSET;


    GRAPH_MINMAX  minmax = { 0, 0, 0, 0 };

    BuildMinMax(graphLocal, minmax);

    //
    // iteration constans for vertical and horizontal points
    //
    horisontalAspectRatio = (float)worksArrea.Width() / (float)abs(minmax.max_x - minmax.min_x);
    verticalAspectRatio = (float)worksArrea.Height() / (float)abs(minmax.max_y - minmax.min_y);


    //
    // central point of the whole graph
    //
    CPoint  centralPoint(worksArrea.left, worksArrea.bottom);

    //
    // in case we have negative values move axis to 0
    //
    if (minmax.min_x < 0) {

        centralPoint.x -= (LONG)(minmax.min_x * horisontalAspectRatio);
    }

    if (minmax.min_y < 0) {

        centralPoint.y += (LONG)(minmax.min_y * verticalAspectRatio);
    }

    //
    // move central point if we are near to order dimensions
    //
    if (centralPoint.x < (worksArrea.left + CHART_CROSSPOINT_OFFSET))
    {
        centralPoint.x += CHART_CROSSPOINT_OFFSET;
    }

    if (centralPoint.y > (worksArrea.bottom - CHART_CROSSPOINT_OFFSET))
    {
        //centralPoint.y -= CHART_CROSSPOINT_OFFSET;
    }



    this->DrawAxis(cdc, worksArrea, centralPoint, minmax);


    for (INT_PTR i = 0; i < graphLocal.GetSize(); i++)
    {

        if (graphLocal[i]->IsVisible())
        {

            DrawGraph(cdc, *(graphLocal[i]), minmax, worksArrea, centralPoint);
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
    // draw in memory
    //
    Draw(&m_memoryDC);

    //
    // copy to main DC
    //
    dc.BitBlt(0, 0, width, height, &m_memoryDC, 0, 0, SRCCOPY);
}

void
CDottedGraph::InitializeMinMax()
{
    INT_PTR  elementsCount = m_chartValues.GetCount();

    m_maxX = m_chartValues[0].x;
    m_minX = m_chartValues[0].x;
    m_maxY = m_chartValues[0].y;
    m_minY = m_chartValues[0].y;

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        if (m_chartValues[i].x < m_minX) {
            m_minX = m_chartValues[i].x;
        }

        if (m_chartValues[i].x > m_maxX) {
            m_maxX = m_chartValues[i].x;
        }

        if (m_chartValues[i].y < m_minY) {
            m_minY = m_chartValues[i].y;
        }

        if (m_chartValues[i].y > m_maxY) {
            m_maxY = m_chartValues[i].y;
        }
    }

    m_minmaxInitialized = true;

}

void
CDottedGraph::DrawCircle(
    CDC* cdc,
    const CPoint& coordinates,
    SIZE_T size
    )
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
CDottedGraph::DrawGraph(
    CDC* cdc,
    const CRect& boundingRect,
    const CPoint& centralPoint,
    double  horisontalAspectRatio,
    double  verticalAspectRatio
)
{ 
    INT_PTR  elementsCount = m_chartValues.GetCount();

    CPen    drawPenGraph(PS_SOLID, 1, m_chartColor);
    CBrush  drawBrush;
    drawBrush.CreateSolidBrush(m_chartColor);

    //CHusBrush  drawBrush(BS_SOLID, graph.m_chartColor);
    CPen* oldPen = cdc->SelectObject(&drawPenGraph);
    CBrush* oldBrush = cdc->SelectObject(&drawBrush);

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel;

        //
        // invert coordinate for Y and left as is for X
        //
        //pixel.x = boundingRect.left + (LONG)(m_chartValues[i].x * horisontalAspectRatio + centralPoint.x);
        //pixel.y = boundingRect.bottom - (LONG)(m_chartValues[i].y * verticalAspectRatio + centralPoint.y) + boundingRect.top;
        pixel.x = centralPoint.x + (LONG)(m_chartValues[i].x * horisontalAspectRatio);
        pixel.y = centralPoint.y - (LONG)(m_chartValues[i].y * verticalAspectRatio);


        DrawCircle(cdc, pixel, m_DotSize);
    }

    cdc->SelectObject(oldBrush);
    cdc->SelectObject(oldPen);
}

void
CColorDottedGraph::InitializeMinMax()
{
    INT_PTR  elementsCount = m_chartValues.GetCount();

    m_maxX = m_chartValues[0].x;
    m_minX = m_chartValues[0].x;
    m_maxY = m_chartValues[0].y;
    m_minY = m_chartValues[0].y;

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        if (m_chartValues[i].x < m_minX) {
            m_minX = m_chartValues[i].x;
        }

        if (m_chartValues[i].x > m_maxX) {
            m_maxX = m_chartValues[i].x;
        }

        if (m_chartValues[i].y < m_minY) {
            m_minY = m_chartValues[i].y;
        }

        if (m_chartValues[i].y > m_maxY) {
            m_maxY = m_chartValues[i].y;
        }
    }

    m_minmaxInitialized = true;

}

void
CColorDottedGraph::DrawCircle(
    CDC* cdc,
    const CPoint& coordinates,
    SIZE_T size
)
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
CColorDottedGraph::DrawGraph(
    CDC* cdc,
    const CRect& boundingRect,
    const CPoint& centralPoint,
    double  horisontalAspectRatio,
    double  verticalAspectRatio
)
{
    INT_PTR  elementsCount = m_chartValues.GetCount();

    CPen    drawPenGraph(PS_SOLID, 1, m_chartColor);
    CBrush  drawBrush;

    drawBrush.CreateSolidBrush(m_chartColor);

    //CHusBrush  drawBrush(BS_SOLID, graph.m_chartColor);
    CPen* oldPen = cdc->SelectObject(&drawPenGraph);
    CBrush* oldBrush = cdc->SelectObject(&drawBrush);

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel;

        CPen    drawPenGraph(PS_SOLID, 1, m_chartValues[i].color);
        CBrush  drawBrush;

        drawBrush.CreateSolidBrush(m_chartValues[i].color);

        cdc->SelectObject(&drawPenGraph);
        cdc->SelectObject(&drawBrush);

        //
        // invert coordinate for Y and left as is for X
        //
        pixel.x = centralPoint.x + (LONG)(m_chartValues[i].x * horisontalAspectRatio);
        pixel.y = centralPoint.y - (LONG)(m_chartValues[i].y * verticalAspectRatio);

        DrawCircle(cdc, pixel, m_DotSize);
    }

    cdc->SelectObject(oldBrush);
    cdc->SelectObject(oldPen);
}

void
CLinedGraph::InitializeMinMax()
{
    INT_PTR  elementsCount = m_chartValues.GetCount();

    m_maxX = m_chartValues[0].x;
    m_minX = m_chartValues[0].x;
    m_maxY = m_chartValues[0].y;
    m_minY = m_chartValues[0].y;

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        if (m_chartValues[i].x < m_minX) {
            m_minX = m_chartValues[i].x;
        }

        if (m_chartValues[i].x > m_maxX) {
            m_maxX = m_chartValues[i].x;
        }

        if (m_chartValues[i].y < m_minY) {
            m_minY = m_chartValues[i].y;
        }

        if (m_chartValues[i].y > m_maxY) {
            m_maxY = m_chartValues[i].y;
        }
    }

    m_minmaxInitialized = true;

}

void
CLinedGraph::DrawGraph(
    CDC* cdc,
    const CRect& boundingRect,
    const CPoint& centralPoint,
    double  horisontalAspectRatio,
    double  verticalAspectRatio
    )
{
    CPoint  pixel;
    CPen    drawPenGraph(PS_SOLID, m_thickness, m_chartColor);

    CPen   *oldPen = cdc->SelectObject(&drawPenGraph);

    if (m_chartValues.GetCount() < 2)
        return;

    //
    // invert coordinate for Y and left as is for X
    //
    pixel.x = centralPoint.x + (LONG)(m_chartValues[0].x * horisontalAspectRatio);
    pixel.y = centralPoint.y - (LONG)(m_chartValues[0].y * verticalAspectRatio);

    cdc->MoveTo(pixel);

    for (INT_PTR i = 1, j = m_chartValues.GetCount(); i < j; i++)
    {

        pixel.x = centralPoint.x + (LONG)(m_chartValues[i].x * horisontalAspectRatio);
        pixel.y = centralPoint.y - (LONG)(m_chartValues[i].y * verticalAspectRatio);

        cdc->LineTo(pixel);
    }

    cdc->SelectObject(oldPen);
}


void
CPixelGraph::InitializeMinMax()
{
    INT_PTR  elementsCount = m_chartValues.GetCount();

    m_maxX = m_chartValues[0].x;
    m_minX = m_chartValues[0].x;
    m_maxY = m_chartValues[0].y;
    m_minY = m_chartValues[0].y;

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        if (m_chartValues[i].x < m_minX) {
            m_minX = m_chartValues[i].x;
        }

        if (m_chartValues[i].x > m_maxX) {
            m_maxX = m_chartValues[i].x;
        }

        if (m_chartValues[i].y < m_minY) {
            m_minY = m_chartValues[i].y;
        }

        if (m_chartValues[i].y > m_maxY) {
            m_maxY = m_chartValues[i].y;
        }
    }

    m_minmaxInitialized = true;

}

void
CPixelGraph::DrawGraph(
    CDC* cdc,
    const CRect& boundingRect,
    const CPoint& centralPoint,
    double  horisontalAspectRatio,
    double  verticalAspectRatio
    )
{
    INT_PTR  elementsCount = this->GetCount();

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel;

        //
        // invert coordinate for Y and left as is for X
        //
        pixel.x = centralPoint.x + (LONG)(m_chartValues[i].x * horisontalAspectRatio);
        pixel.y = centralPoint.y - (LONG)(m_chartValues[i].y * verticalAspectRatio);


        cdc->SetPixel(pixel.x, pixel.y, m_chartColor);
    }
}

void
CColorPixelGraph::InitializeMinMax()
{
    INT_PTR  elementsCount = m_chartValues.GetCount();

    m_maxX = m_chartValues[0].x;
    m_minX = m_chartValues[0].x;
    m_maxY = m_chartValues[0].y;
    m_minY = m_chartValues[0].y;

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        if (m_chartValues[i].x < m_minX) {
            m_minX = m_chartValues[i].x;
        }

        if (m_chartValues[i].x > m_maxX) {
            m_maxX = m_chartValues[i].x;
        }

        if (m_chartValues[i].y < m_minY) {
            m_minY = m_chartValues[i].y;
        }

        if (m_chartValues[i].y > m_maxY) {
            m_maxY = m_chartValues[i].y;
        }
    }

    m_minmaxInitialized = true;

}

void
CColorPixelGraph::DrawGraph(
    CDC* cdc,
    const CRect& boundingRect,
    const CPoint& centralPoint,
    double  horisontalAspectRatio,
    double  verticalAspectRatio
    )
{
    INT_PTR  elementsCount = this->GetCount();

    for (INT_PTR i = 0; i < elementsCount; i++)
    {
        CPoint   pixel;

        //
        // invert coordinate for Y and left as is for X
        //
        //pixel.x = boundingRect.left + (LONG)(m_chartValues[i].x * horisontalAspectRatio + centralPoint.x);
        //pixel.y = boundingRect.bottom - (LONG)(m_chartValues[i].y * verticalAspectRatio + centralPoint.y) + boundingRect.top;
        pixel.x = centralPoint.x + (LONG)(m_chartValues[i].x * horisontalAspectRatio);
        pixel.y = centralPoint.y - (LONG)(m_chartValues[i].y * verticalAspectRatio);


        cdc->SetPixel(pixel.x, pixel.y, m_chartValues[i].color);
    }
}
