#pragma once
#include <string>
#include <iostream>


#include "SocialMedia.h"

class CFacebookMedia : public ISocialMedia
{


public:
    CFacebookMedia()
        : ISocialMedia()
    {
    }


    virtual bool Authorize(std::string login, std::string password)
    {

        if (login.empty())
            throw std::string("invalid login");

        if (password.empty())
            throw std::string("invalid password");

        m_isAuthorized = true;

        std::cout << "Facebook: " << "Authorized" << std::endl;

        return m_isAuthorized;
    }

    virtual bool PostMessage(std::string message)
    {
        if (!m_isAuthorized)
            throw std::string("not authorized");

        std::cout << "Facebook: " << message << std::endl;


        return true;
    }


};