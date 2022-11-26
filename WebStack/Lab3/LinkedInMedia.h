#pragma once


#include "SocialMedia.h"

class CLinkedInMedia : public ISocialMedia
{


public:
    CLinkedInMedia()
        : ISocialMedia()
    {
    }


    virtual bool Authorize(std::string email, std::string password)
    {

        if (email.empty())
            throw std::string("invalid email");

        if (password.empty())
            throw std::string("invalid password");

        m_isAuthorized = true;

        std::cout << "LinkedIn: " << "Authorized" << std::endl;

        return m_isAuthorized;
    }

    virtual bool PostMessage(std::string message)
    {
        if (!m_isAuthorized)
            throw std::string("not authorized");

        std::cout << "LinkedIn: " << message << std::endl;

        return true;
    }


};
