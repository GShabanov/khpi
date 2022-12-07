#pragma once

#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <iostream>


class IDownloader
{

public:

    virtual unsigned char *Download(std::string& url) = 0;

};

class CSimpleDownloader : public IDownloader
{
public:

    CSimpleDownloader()
        : IDownloader()
    {
    }

    virtual unsigned char* Download(std::string& url)
    {

        std::cout << "Downloader: do download\n";

        unsigned char* data = new unsigned char[0x1000];

        srand(1000);

        for (int i = 0; i < 0x1000; i++)
        {
            data[i] = (unsigned char)rand();
        }


        return data;
    }


};


#endif // __DOWNLOADER_H__
