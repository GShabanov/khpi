#pragma once

#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>
#include <iostream>


class IFileManager
{

public:
    virtual unsigned char* ReadFromDisk(std::string& localPath, size_t& dataSize) = 0;

};

class CFileManager : public IFileManager
{

public:
    virtual unsigned char*
    ReadFromDisk(std::string& localPath, size_t& dataSize)
    {
        unsigned char* buffer = new unsigned char[0x1000];

        if (buffer == nullptr)
            return nullptr;


        //
        // just for test
        //

        for (size_t i = 0; i < 0x1000; i++)
        {
            buffer[i] = (unsigned char)(i << 3);
        }


        dataSize = 0x1000;

        return buffer;
    }

};

#endif // __FILESYSTEM_H__