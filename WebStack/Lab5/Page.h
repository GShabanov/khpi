#pragma once

#ifndef __PAGE_H__
#define __PAGE_H__

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class IPage
{

public:

    virtual std::string HtmlOutput() = 0;

    virtual std::string JsonOutput() = 0;

    virtual std::string XmlOutput() = 0;

};

/// <summary>
/// 
///  Simple page
/// 
/// </summary>

class CSimplePage : public IPage
{
    std::string m_title;
    std::string m_content;

public:
    CSimplePage(const std::string& title, const std::string& content)
        : IPage()
    {

        m_title = title;
        m_content = content;
    }


    virtual std::string HtmlOutput()
    {

        std::stringstream sstream;

        sstream << "<body>\n<h3>" << m_title << "</h3>\n";
        sstream << "<p>" << m_content << "</p>\n";
        sstream << "\n</body>";

        return sstream.str();
    }

    virtual std::string JsonOutput()
    {
        std::string out = "{{ 'title': '" + m_title + ",\n' content': '" + m_content + "'\n}}";

        return out;
    }

    virtual std::string XmlOutput()
    {

        std::stringstream sstream;

        sstream << "<SimplePage>\n";
        sstream << "\n    <Title>" << m_title << "</Title> \n";
        sstream << "\n    <Content>" << m_content << "</Content>\n";
        sstream << "\n</SimplePage>";

        return sstream.str();

    }

};

/// <summary>
/// 
///  Product page
/// 
/// </summary>

class CProductPage : public IPage
{
    unsigned int m_id;
    std::string m_name;
    std::string m_description;
    std::string m_imageUrl;

public:
    CProductPage(
        unsigned int id,
        const std::string name,
        const std::string description,
        const std::string imageUrl)
        : IPage()
    {

        m_id = id;
        m_name = name;
        m_description = description;
        m_imageUrl = imageUrl;
    }


    virtual std::string HtmlOutput()
    {

        std::stringstream sstream;

        sstream << "<body>\n<h3> Id =" << std::dec << m_id;
        sstream << "\n  </h3><p><h1>" << m_name << "</h1> \n";
        sstream << "<h3>" << m_description << "</h3> \n";
        sstream << "<image src =" << m_imageUrl << "/> \n";
        sstream << "\n</p></body>";

        return sstream.str();
    }

    virtual std::string JsonOutput()
    {
        std::stringstream sstream;

        sstream << "{{'product': { {'id': " << std::dec << m_id;
        sstream << "'name' : '" << m_name << "',";
        sstream << "'description' : '" << m_description << "',";
        sstream << "'imageUrl' : '" << m_imageUrl << "'}}";


        return sstream.str();
    }

    virtual std::string XmlOutput()
    {
        std::stringstream sstream;

        sstream << "<ProductPage>\n  <Product> <Id>" << std::dec << m_id << "</Id>";
        sstream << "\n    <Name>" << m_name << "</Name> \n";
        sstream << "\n    <Description>" << m_description << "</Description> \n";
        sstream << "\n    <ImageUrl>" << m_imageUrl << "</ImageUrl>\n";
        sstream << "\n  </Product>\n</ProductPage>";

        return sstream.str();

    }

};

#endif // __PAGE_H__