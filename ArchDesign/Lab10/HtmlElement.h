#pragma once

#include <string>
#include <list>
#include <iostream>


struct CHTMLElementBase
{
    std::string  m_id;

    virtual std::string GetData() = 0;
};


class CHTMLElement : public CHTMLElementBase
{

protected:
    std::list<CHTMLElement*> m_childrens;

public:

    CHTMLElement()
        : CHTMLElementBase()
    {

    }

    virtual ~CHTMLElement()
    {
        for (std::list<CHTMLElement*>::iterator i = m_childrens.begin(); i != m_childrens.end(); i++)
        {
            delete (*i);
        }
    }


    void AddChild(CHTMLElement *element)
    {
        m_childrens.push_back(element);
    }

};