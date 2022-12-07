#pragma once

#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <string>
#include "Page.h"

using namespace std;

class IRenderer
{

public:

    virtual std::string Render() = 0;


};


class CHtmlRenderer : public IRenderer
{

    IPage* m_page;

public:
    std::string Render()
    {
        return m_page->HtmlOutput();
    }

    CHtmlRenderer(IPage* page)
    {
        m_page = page;
    }
};

class CJsonRenderer : public IRenderer
{

    IPage* m_page;

public:
    std::string Render()
    {
        return m_page->JsonOutput();
    }

    CJsonRenderer(IPage* page)
    {
        m_page = page;
    }
};

class CXmlRenderer : public IRenderer
{

    IPage* m_page;

public:
    std::string Render()
    {
        return m_page->XmlOutput();
    }

    CXmlRenderer(IPage* page)
    {
        m_page = page;
    }
};

#endif // __RENDERER_H__
