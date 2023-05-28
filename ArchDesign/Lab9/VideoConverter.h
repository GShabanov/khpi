#pragma once

#ifndef __VIDEOCONVERTER_H__
#define __VIDEOCONVERTER_H__

#include <string>
#include <iostream>


class IVideoConverter
{

public:
    virtual unsigned char *StreamConvert(unsigned char *inputStream, size_t &size) = 0;

};

class CVideoConverter : public IVideoConverter
{

public:

    virtual unsigned char* StreamConvert(unsigned char* inputStream, size_t& size)
    {
        std::cout << "[*] convert data stream...";
        std::cout << "[+] success" << std::endl;

        return inputStream;

    }

};

#endif // __VIDEOCONVERTER_H__
