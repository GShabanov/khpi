#pragma once

#include "HtmlElement.h"

class CHtmlPage : public CHTMLElement
{

public:
    CHtmlPage(std::list<CHTMLElement*>& childs)
    {
        m_id = "";
        m_childrens = childs;
    }

    std::string GetId()
    {
        return m_id;
    }

    std::list<CHTMLElement*>
        GetChildList()
    {
        return m_childrens;
    }


    virtual std::string GetData()
    {
        std::string  output = "<html>\n";

        //
        // обходимо всі дочерні елементи та будуємо внутрішню частину
        //
        for (std::list<CHTMLElement*>::iterator i = m_childrens.begin(); i != m_childrens.end(); i++)
        {
            output += (*i)->GetData();

            output += "\n";
        }

        output += "</html>";

        return output;

    }
};
