#pragma once

#include "HtmlElement.h"

class CHtmlFieldset : public CHTMLElement
{

public:
    CHtmlFieldset(const std::string& id, std::list<CHTMLElement*>& childs)
    {
        m_id = id;
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
        std::string  output = "<fieldset id=" + m_id + ">\n";

        //
        // обходимо всі дочерні елементи та будуємо внутрішню частину
        //
        for (std::list<CHTMLElement*>::iterator i = m_childrens.begin(); i != m_childrens.end(); i++)
        {
            output += (*i)->GetData();

            output += "\n";
        }

        output += "</fieldset>";

        return output;

    }
};
