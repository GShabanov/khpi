#pragma once

#include "HtmlElement.h"

#include <vector>

class CHtmlInput : public CHTMLElement
{

public:
    CHtmlInput(const std::string &id)
    {
        m_id = id;
    }


    virtual std::string GetData()
    {
        std::string  output = "<input id=" + m_id + "/>";

        return output;

    }
};
