#pragma once

#ifndef __PROXYDOWNLOADER_H__
#define __PROXYDOWNLOADER_H__

#include <string>
#include <iostream>
#include "Downloader.h"

class CProxyDownloader : public IDownloader
{

    IDownloader *m_realDownloader;
    std::string  m_lastUrl;
    unsigned char* m_data;

public:

    CProxyDownloader(IDownloader *realDownloader)
        : IDownloader()
    {

        m_realDownloader = realDownloader;

        m_data = NULL;

        m_lastUrl = "";
    }

    virtual unsigned char* Download(std::string& url)
    {

        if (m_lastUrl == url)        
        {

            std::cout << "Proxy: download from proxy\n";

            unsigned char* dataCopy = new unsigned char[0x1000];

            memcpy(dataCopy, m_data, 0x1000);

            return dataCopy;
        }

        if (m_data != NULL)
        {
            delete[] m_data;
        }



        std::cout << "Proxy: download from real\n";
        m_lastUrl = url;
        unsigned char* data = m_realDownloader->Download(url);

        unsigned char* dataCopy = new unsigned char[0x1000];

        memcpy(dataCopy, data, 0x1000);

        m_data = data;

        return dataCopy;
    }


};


#endif // __PROXYDOWNLOADER_H__
