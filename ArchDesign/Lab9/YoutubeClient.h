#pragma once

#ifndef __YOURTUBECLIENT_H__
#define __YOURTUBECLIENT_H__

#include <string>
#include <iostream>
#include "FileSystem.h"
#include "YouTubeApi.h"
#include "VideoConverter.h"

class CYoutubeClient
{

private:

    IYouTubeApi     *m_youTubeApi;
    IFileManager    *m_fileManager;
    IVideoConverter *m_videoConverter;
    std::string     m_Name;
    std::string     m_Password;

public:
    CYoutubeClient(std::string name, std::string password)
    {
        m_Name = name;
        m_Password = password;

        m_youTubeApi = new CYouTubeApi();
        m_fileManager = new CFileManager();
        m_videoConverter = new CVideoConverter();
    }

    ~CYoutubeClient()
    {        
        delete m_youTubeApi;
        delete m_fileManager;
        delete m_videoConverter;
    }

    bool UploadFile(std::string localPath)
    {
        size_t   dataSize;

        //
        // read from source
        //
        unsigned char *data = m_fileManager->ReadFromDisk(localPath, dataSize);

        //
        // authorize on you tube service
        //
        m_youTubeApi->Authorise(m_Name, m_Password);

        //
        // convert data to same format
        //
        data = m_videoConverter->StreamConvert(data, dataSize);


        m_youTubeApi->UploadData(data, dataSize);

        delete[] data;

        return true;
    }

};


#endif // __YOURTUBECLIENT_H__
