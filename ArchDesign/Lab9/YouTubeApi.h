#pragma once

#ifndef __YOUTUBEAPI_H__
#define __YOUTUBEAPI_H__

#include <string>
#include <iostream>


class IYouTubeApi
{

public:
    virtual bool Authorise(std::string& name, std::string& password) = 0;
    virtual bool UploadData(unsigned char *buffer, size_t& dataSize) = 0;

};

class CYouTubeApi : public IYouTubeApi
{

public:

    virtual bool Authorise(std::string& name, std::string& password)
    {
        std::cout << "[+] authorized succefull" << std::endl;

        return true;
    }

    virtual bool UploadData(unsigned char* buffer, size_t& dataSize)
    {
        std::cout << "[+] data uploaded" << std::endl;
        return true;
    }

};

#endif // __YOUTUBEAPI_H__