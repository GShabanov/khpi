#pragma once

#include "HtmlElement.h"

#include <vector>


class CHtmlSelect : public CHTMLElement
{

private:
    std::vector<std::string> m_options;

public:
    CHtmlSelect(const std::string& id, std::vector<std::string>& options)
    {
        m_id = id;
        m_options = options;
    }

    std::string GetId()
    {
        return m_id;
    }


    virtual std::string GetData()
    {
        std::string  output = "<select id=" + m_id + ">\n";

        //
        // обходимо всі дочерні елементи та будуємо внутрішню частину
        //
        for (int i = 0; i != m_options.size(); i++)
        {
            output += "    <option>" + m_options[i] + "</option>\n";
        }

        output += "</select>";

        return output;

    }
};
