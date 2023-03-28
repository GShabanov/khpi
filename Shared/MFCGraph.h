
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
#include <vector>

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

class CGraph
{

protected:
    enum class Type {

        DottedGraph,
        LinedGraph,
        PixelGraph,
        ColorPixelGraph,
    };

public:

    typedef struct _GRAPH_POINT {

        double   x;
        double   y;

    } GRAPH_POINT;

    typedef struct _GRAPH_POINT2 : public GRAPH_POINT {

        COLORREF   color;

    } GRAPH_POINT2;

protected:
    CString                 m_graphName;
    COLORREF                m_chartColor;
    Type                    m_GraphType;
    bool                    m_visible;
    bool                    m_referenceGraph;

    bool                    m_minmaxInitialized;
    double                  m_minX;
    double                  m_maxX;
    double                  m_minY;
    double                  m_maxY;


    virtual void  InitializeMinMax() {};

    virtual void  DrawGraph(
        CDC* cdc,
        const CRect &boundingRect,
        const CPoint& centralPoint,
        double  horisontalAspectRatio,
        double  verticalAspectRatio
    ) { };


    virtual CGraph& Copy(const CGraph& right)
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

        return *this;
    }

public:


    CGraph()
        : m_visible(false)
        , m_minmaxInitialized(false)
        , m_referenceGraph(false)
        , m_GraphType(Type::DottedGraph)
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
        m_GraphType = type;
        m_referenceGraph = referenceGraph;
        m_GraphType = type;
        m_graphName = name;
    }

    virtual CGraph* clone(const CGraph& right) {
        return NULL;
    }

    CGraph(const CGraph &right)
    {
        Copy(right);
    }

    virtual ~CGraph()
    {
    }

    CGraph& operator= (const CGraph& right)
    /*
        just for CArray<CGraph>
    */
    {
        if (this != &right)
        {
            //
            // we can't use Right's reloaded function (!)
            //
            this->Copy(right);
        }


        return *this;
    }

    virtual SIZE_T GetCount() {
        throw(1);
        return  0;
    };


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


    friend class CGraphControl;
    friend class CPixelGraph;

};

class CDottedGraph : public CGraph
{

private:
    CArray<GRAPH_POINT>     m_chartValues;
    int                     m_DotSize;


    virtual void  InitializeMinMax();

    virtual void  DrawGraph(
        CDC* cdc,
        const CRect& boundingRect,
        const CPoint& centralPoint,
        double  horisontalAspectRatio,
        double  verticalAspectRatio
    );

    virtual CGraph* clone(const CGraph& right) {

        CDottedGraph* data = new CDottedGraph();
        CDottedGraph& r = (CDottedGraph&)right;

        data->Copy(r);

        data->m_DotSize = r.m_DotSize;
        data->m_chartValues.RemoveAll();
        data->m_chartValues.Copy(r.m_chartValues);

        return data;
    }

    void DrawCircle(CDC* cdc, const CPoint& coordinates, SIZE_T size);

public:


    CDottedGraph()
        : CGraph()
    {
        m_GraphType = Type::DottedGraph;
        m_DotSize = 3;
    }

    CDottedGraph(const CString& name, bool referenceGraph = false)
        : CGraph(Type::DottedGraph, name, referenceGraph)
    {
        m_DotSize = 3;
    }

    virtual ~CDottedGraph()
    {
        this->m_chartValues.RemoveAll();
    }

    CDottedGraph& operator= (const CDottedGraph& right)
        /*
            just for CArray<CGraph>
        */
    {
        if (this == &right) {
            return *this;
        }

        Copy(right);

        return *this;
    }


    virtual SIZE_T GetCount()
    {
        return m_chartValues.GetCount();
    }

    void SetDotSize(int size) {

        m_DotSize = size;
    }

    void UpdateValues(const GRAPH_POINT* values, size_t count)
    {
        m_chartValues.RemoveAll();

        for (size_t i = 0; i < count; i++)
        {
            m_chartValues.Add(values[i]);
        }
    }

    void UpdateValues(const CArray<GRAPH_POINT>& values)
    {
        m_chartValues.RemoveAll();

        m_chartValues.Copy(values);
    }

};

class CColorDottedGraph : public CGraph
{

private:
    CArray<GRAPH_POINT2>     m_chartValues;
    int                      m_DotSize;

    virtual void  InitializeMinMax();

    virtual void  DrawGraph(
        CDC* cdc,
        const CRect& boundingRect,
        const CPoint& centralPoint,
        double  horisontalAspectRatio,
        double  verticalAspectRatio
    );

    virtual CGraph* clone(const CGraph& right) {

        CColorDottedGraph* data = new CColorDottedGraph();
        CColorDottedGraph& r = (CColorDottedGraph&)right;

        data->Copy(r);

        data->m_DotSize = r.m_DotSize;
        data->m_chartValues.RemoveAll();
        data->m_chartValues.Copy(r.m_chartValues);

        return data;
    }

    void DrawCircle(CDC* cdc, const CPoint& coordinates, SIZE_T size);

public:


    CColorDottedGraph()
        : CGraph()
    {
        m_GraphType = Type::DottedGraph;
        m_DotSize = 3;
    }

    CColorDottedGraph(const CString& name, bool referenceGraph = false)
        : CGraph(Type::DottedGraph, name, referenceGraph)
    {
        m_DotSize = 3;
    }

    virtual ~CColorDottedGraph()
    {
        this->m_chartValues.RemoveAll();
    }

    CColorDottedGraph& operator= (const CColorDottedGraph& right)
        /*
            just for CArray<CGraph>
        */
    {
        if (this == &right) {
            return *this;
        }

        Copy(right);

        return *this;
    }


    virtual SIZE_T GetCount()
    {
        return m_chartValues.GetCount();
    }

    void SetDotSize(int size) {

        m_DotSize = size;
    }

    void UpdateValues(const GRAPH_POINT2* values, size_t count)
    {
        m_chartValues.RemoveAll();

        for (size_t i = 0; i < count; i++)
        {
            m_chartValues.Add(values[i]);
        }
    }

    void UpdateValues(const CArray<GRAPH_POINT2>& values)
    {
        m_chartValues.RemoveAll();

        m_chartValues.Copy(values);
    }

};


class CLinedGraph : public CGraph
{

private:
    CArray<GRAPH_POINT>     m_chartValues;
    int                     m_thickness;


    virtual void  InitializeMinMax();

    virtual void  DrawGraph(
        CDC* cdc,
        const CRect& boundingRect,
        const CPoint& centralPoint,
        double  horisontalAspectRatio,
        double  verticalAspectRatio
    );

    virtual CGraph* clone(const CGraph& right) {

        CLinedGraph* data = new CLinedGraph();
        CLinedGraph& r = (CLinedGraph &)right;

        data->Copy(r);

        data->m_chartValues.RemoveAll();
        data->m_chartValues.Copy(r.m_chartValues);

        return data;
    }

public:


    CLinedGraph()
        : CGraph()
    {
        m_GraphType = Type::LinedGraph;
        m_thickness = 1;

    }

    CLinedGraph(const CString& name, bool referenceGraph = false)
        : CGraph(Type::LinedGraph, name, referenceGraph)
    {
        m_thickness = 1;
    }

    virtual ~CLinedGraph()
    {
        this->m_chartValues.RemoveAll();
    }

    void SetThickness(int thickness) {

        m_thickness = thickness;
    }

    virtual SIZE_T GetCount()
    {
        return m_chartValues.GetCount();
    }

    void UpdateValues(const GRAPH_POINT* values, size_t count)
    {
        m_chartValues.RemoveAll();

        for (size_t i = 0; i < count; i++)
        {
            m_chartValues.Add(values[i]);
        }
    }

    void UpdateValues(const CArray<GRAPH_POINT>& values)
    {
        m_chartValues.RemoveAll();

        m_chartValues.Copy(values);
    }
};

class CPixelGraph : public CGraph
{
private:
    CArray<GRAPH_POINT>     m_chartValues;


    virtual void  InitializeMinMax();

    virtual void  DrawGraph(
        CDC* cdc,
        const CRect& boundingRect,
        const CPoint& centralPoint,
        double  horisontalAspectRatio,
        double  verticalAspectRatio
    );

    virtual CGraph* clone(const CGraph& right) {

        CPixelGraph* data = new CPixelGraph();
        CPixelGraph& r = (CPixelGraph&)right;

        data->Copy(r);

        data->m_chartValues.RemoveAll();
        data->m_chartValues.Copy(r.m_chartValues);

        return data;
    }

public:


    CPixelGraph()
        : CGraph()
    {
        m_GraphType = Type::PixelGraph;
    }

    CPixelGraph(const CString& name, bool referenceGraph = false)
        : CGraph(Type::PixelGraph, name, referenceGraph)
    {
    }

    virtual ~CPixelGraph()
    {
        this->m_chartValues.RemoveAll();
    }

    virtual SIZE_T GetCount()
    {
        return m_chartValues.GetCount();
    }


    void UpdateValues(const GRAPH_POINT* values, size_t count)
    {
        m_chartValues.RemoveAll();

        for (size_t i = 0; i < count; i++)
        {
            m_chartValues.Add(values[i]);
            //m_chartValues.push_back(values[i]);
        }
    }

    void UpdateValues(const CArray<GRAPH_POINT>& values)
    {
        m_chartValues.RemoveAll();

        m_chartValues.Copy(values);
    }
};

class CColorPixelGraph : public CGraph
{

private:
    CArray<GRAPH_POINT2>    m_chartValues;


    virtual void  InitializeMinMax();

    virtual void  DrawGraph(
        CDC* cdc,
        const CRect& boundingRect,
        const CPoint& centralPoint,
        double  horisontalAspectRatio,
        double  verticalAspectRatio
    );

    virtual CGraph* clone(const CGraph& right) {

        CColorPixelGraph* data = new CColorPixelGraph();
        CColorPixelGraph& r = (CColorPixelGraph&)right;

        data->Copy(r);

        data->m_chartValues.RemoveAll();
        data->m_chartValues.Copy(r.m_chartValues);

        return data;
    }

public:


    CColorPixelGraph()
        : CGraph()
    {
        m_GraphType = Type::ColorPixelGraph;
        this->m_chartValues.SetSize(1);
    }

    CColorPixelGraph(const CString& name, bool referenceGraph = false)
        : CGraph(Type::ColorPixelGraph, name, referenceGraph)
    {
        this->m_chartValues.SetSize(1);
    }


    virtual ~CColorPixelGraph()
    {
        this->m_chartValues.RemoveAll();
    }


    virtual SIZE_T GetCount() {

        return m_chartValues.GetCount();
    }

    void UpdateValues(const GRAPH_POINT2* values, size_t count)
    {
        m_chartValues.RemoveAll();

        for (size_t i = 0; i < count; i++)
        {
            m_chartValues.Add(values[i]);
        }
    }

    void UpdateValues(const CArray<GRAPH_POINT2>& values)
    {
        m_chartValues.RemoveAll();

        m_chartValues.Copy(values);
    }
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
    CArray<CGraph *>                m_graphs;
    bool                            m_absolute;

    void Draw(CDC  *cdc);

    void BuildMinMax(const CArray<CGraph*>& graph, GRAPH_MINMAX  &minmaxRect);
    void DrawCircle(CDC* cdc, const CPoint& coordinates, SIZE_T size);
    void DrawAxis(CDC *cdc, const CRect &rect, const CPoint& centralPoint,  const GRAPH_MINMAX &minMax);

    void DrawGraph(
        CDC* cdc,
        CGraph& graph,
        const GRAPH_MINMAX& minMax,
        const CRect& drawRect,
        const CPoint& centralPoint
    );

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

    void InsertGraph(int index, CGraph &graph);
    void DeleteGraph(int index);

    CGraphControl& operator= (const CGraphControl& ) {

        return *this;
    }

    CGraph::GRAPH_POINT GetPointFromClient(int x, int y);

protected:

    //
    // message handlers
    //
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
    DECLARE_MESSAGE_MAP()
};

#endif // __MFC_GRAPH_H__