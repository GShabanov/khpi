
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "singleton.h"

class IDisk
{
public:
    virtual bool  Read(void* buffer, size_t sizeToRead, size_t& readed) = 0;

    virtual bool  Write(void* buffer, size_t sizeToWrite, size_t& written) = 0;
};

class CLocalDisk : public Singleton<CLocalDisk>, public IDisk
{
public:
    CLocalDisk()
    {
        std::cout << "CLocalDisk::CLocalDisk()" << std::endl;
    }

    virtual bool  Read(void* buffer, size_t sizeToRead, size_t& readed)
    {
        std::cout << "CLocalDisk::Read" << std::endl;
        return true;
    }

    virtual bool  Write(void* buffer, size_t sizeToWrite, size_t& written)
    {
        std::cout << "CLocalDisk::Write" << std::endl;
        return true;
    }
};

class CAmazonDisk : public Singleton<CAmazonDisk>, public IDisk
{


public:
    CAmazonDisk()
    {
        std::cout << "CAmazonDisk::CAmazonDisk()" << std::endl;
    }

    virtual bool  Read(void* buffer, size_t sizeToRead, size_t& readed)
    {
        std::cout << "CAmazonDisk::Read" << std::endl;
        return true;
    }

    virtual bool  Write(void* buffer, size_t sizeToWrite, size_t& written)
    {
        std::cout << "CAmazonDisk::Write" << std::endl;
        return true;
    }
};



CLocalDisk    globalDisk;

int
_tmain(int argc, TCHAR* argv[])
{
    unsigned char buffer[100];
    size_t        ioCount = 0;

    IDisk* disk1 = CLocalDisk::Get();

    IDisk* disk3 = CLocalDisk::Get();

    //
    // this is must be one instance of one class !
    //
    if (disk1 != disk3)
    {
        std::cout << "Error: classes instances are not same !" << std::endl;
        return -1;
    }

    IDisk* disk2 = CAmazonDisk::Get();

    IDisk* disk4 = CAmazonDisk::Get();

    //
    // this is must be one instance of one class !
    //
    if (disk2 != disk4)
    {
        std::cout << "Error: classes instances are not same !" << std::endl;
        return -1;
    }

    //
    // they are different instances !
    //
    if (disk1 == disk2)
    {
        std::cout << "Error: classes are sharing same space !" << std::endl;
        return -1;

    }


    disk1->Write(buffer, sizeof(buffer), ioCount);

    disk2->Write(buffer, sizeof(buffer), ioCount);

    disk1->Read(buffer, sizeof(buffer) / 2, ioCount);

    disk2->Read(buffer, sizeof(buffer) / 2, ioCount);


    globalDisk.Write(buffer, sizeof(buffer), ioCount);
    globalDisk.Read(buffer, sizeof(buffer), ioCount);



    return 0;
}

