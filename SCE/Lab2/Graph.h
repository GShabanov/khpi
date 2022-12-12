
/***************************************************************************************\
*   File:                                                                               *
*       Graph.h                                                                         *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for drawing graphs                                                    *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#pragma once

#ifndef __MFC_GRAPH_H__
#define __MFC_GRAPH_H__


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcontrolbars.h>

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

class CGraph
{

public:
    enum class Type {

        DottedGraph,
        LinedGraph,
        PixelGraph
    };

    typedef struct _GRAPH_POINT {

        double   x;
        double   y;

    } GRAPH_POINT;

private:
    CString                 m_graphName;
    COLORREF                m_chartColor;
    CArray<GRAPH_POINT>     m_chartValues;
    Type                    m_GraphType;
    bool                    m_visible;
    bool                    m_referenceGraph;

    bool                    m_minmaxInitialized;
    double                  m_minX;
    double                  m_maxX;
    double                  m_minY;
    double                  m_maxY;


    void  InitializeMinMax()
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

public:


    CGraph()
        : m_visible(false)
        , m_minmaxInitialized(false)
        , m_referenceGraph(false)
        , m_GraphType(Type::LinedGraph)
    {
        m_minX = 0;
        m_maxX = 0;
        m_minY = 0;
        m_maxY = 0;

        m_graphName = "";
        m_chartColor = RGB(0x20, 0x20, 0x20);
    }

    CGraph(CGraph::Type type, const CString& name, bool referenceGraph = false)
        : CGraph()
    {
        m_referenceGraph = referenceGraph;
        m_GraphType = type;
        m_graphName = name;
        m_chartColor = RGB(0x20, 0x20, 0x20);
    }

    CGraph(const CGraph &right)
    {
        this->m_chartColor = right.m_chartColor;
        this->m_graphName = right.m_graphName;
        this->m_visible = right.m_visible;
        this->m_GraphType = right.m_GraphType;
        this->m_minX = right.m_minX;
        this->m_maxX = right.m_maxX;
        this->m_minY = right.m_minY;
        this->m_maxY = right.m_maxY;
        this->m_minmaxInitialized = right.m_minmaxInitialized;
        this->m_chartValues.RemoveAll();
        this->m_chartValues.Copy(right.m_chartValues);
    }

    ~CGraph()
    {
        this->m_chartValues.RemoveAll();
    }

    CGraph& operator= (const CGraph& right)
    /*
        just for CArray<CGraph>
    */
    {
        if (this == &right) {
            return *this;
        }

        this->m_chartColor = right.m_chartColor;
        this->m_graphName = right.m_graphName;
        this->m_visible = right.m_visible;
        this->m_GraphType = right.m_GraphType;
        this->m_minX = right.m_minX;
        this->m_maxX = right.m_maxX;
        this->m_minY = right.m_minY;
        this->m_maxY = right.m_maxY;
        this->m_minmaxInitialized = right.m_minmaxInitialized;
        this->m_chartValues.RemoveAll();
        this->m_chartValues.Copy(right.m_chartValues);
        return *this;
    }


    double  getMaxX()
    {
        if (!m_minmaxInitialized)
        {
            InitializeMinMax();
        }

        return m_maxX;

    }

    double  getMaxY()
    {
        if (!m_minmaxInitialized)
        {
            InitializeMinMax();
        }

        return m_maxY;
    }

    double  getMinX()
    {
        if (!m_minmaxInitialized)
        {
            InitializeMinMax();
        }

        return m_minX;
    }

    double  getMinY()
    {
        if (!m_minmaxInitialized)
        {
            InitializeMinMax();
        }

        return m_minY;
    }

    void SetColor(const COLORREF  color) {
        m_chartColor = color;
    }

    void SetVisible(const bool visible) {
        m_visible = visible;
    }

    bool IsVisible() const {
        return m_visible;
    }

    void UpdateValues(const GRAPH_POINT *values, size_t count)
    {
        m_chartValues.RemoveAll();

        for (size_t i = 0; i < count; i++)
        {
            m_chartValues.Add(values[i]);
        }
    }

    void UpdateValues(const CArray<GRAPH_POINT> &values)
    {
        m_chartValues.RemoveAll();

        m_chartValues.Copy(values);
    }


    friend class CGraphControl;
};

//---------------------------------------------------------------------------
//
// GRAPH CONTROL IMPLEMENTAION
//
//---------------------------------------------------------------------------

class CGraphControl : public CWnd
{
public:
    CGraphControl();
    ~CGraphControl();

private:
    typedef struct _GRAPH_MINMAX {

        double   min_x;
        double   max_x;
        double   min_y;
        double   max_y;

    } GRAPH_MINMAX;

    CFont                           m_axisFont;
    CString                         m_vericalAxisName;
    CString                         m_horizontalAxisName;
    CString                         m_chartName;
    CCriticalSection                m_chartUpdateLock;
    CArray<CGraph>                  m_graphs;
    bool                            m_absolute;

    void Draw(CDC  *cdc);

    void DrawCircle(CDC* cdc, const CPoint& coordinates, SIZE_T size);
    void DrawAxis(CDC *cdc, const CRect &rect, const GRAPH_MINMAX &minMax);

    void DrawLinedGraph(
        CDC* cdc,
        CGraph& graph,
        const GRAPH_MINMAX &minMax,
        const CRect& drawRect,
        const CPoint& normalizationPoint,
        const CPoint& relocationPoint);

    void DrawPointsGraph(
        CDC* cdc,
        CGraph &graph,
        const GRAPH_MINMAX &minMax,
        const CRect& drawRect,
        const CPoint& normalizationPoint,
        const CPoint& relocationVector);

    void DrawPixelGraph(
        CDC* cdc,
        CGraph& graph,
        const GRAPH_MINMAX& minMax,
        const CRect& drawRect,
        const CPoint& normalizationPoint,
        const CPoint& relocationVector);

public:
    virtual BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd);

    void  setHorizontalName(const CString &name) {
        m_horizontalAxisName = name;
    }

    void  setVerticalName(const CString &name) {
        m_vericalAxisName = name;
    }

    void setChartName(const CString &name) {

        m_chartName = name;
    }

    //
    // use absolutely coordinates for axis for all graphs
    //
    void setAbsoluteAxis(const bool absolute) {

        m_absolute = absolute;
    }

    void UpdateChart(int index, const CGraph &controlGraph);

    CGraphControl& operator= (const CGraphControl& ) {

        return *this;
    }

protected:

    //
    // message handlers
    //
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
    DECLARE_MESSAGE_MAP()
};

#endif // __MFC_GRAPH_H__