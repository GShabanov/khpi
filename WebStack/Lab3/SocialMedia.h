#pragma once

#ifndef __SOCIAL_MEDIA_H__
#define __SOCIAL_MEDIA_H__

#include <string>

class ISocialMedia
{

protected:
    bool m_isAuthorized;

    ISocialMedia()
    {
        m_isAuthorized = false;
    }


public:
    enum SocialNetworkType {

        Facebook = 0,
        LinkedIn,
        Unknown
    };

    virtual bool Authorize(std::string username, std::string password) = 0;

    virtual bool PostMessage(std::string message) = 0;


    static ISocialMedia* Get(SocialNetworkType type);

};


#endif // __SOCIAL_MEDIA_H__