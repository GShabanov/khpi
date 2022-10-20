#pragma once

using namespace std;
#include <locale>

//
// see main.cpp for comments
//

class ILogger
{
protected:

    ILogger()
    {
    }

public:

    virtual void LogEvent(string event) = 0;

};


class CConsoleLogger : public ILogger
{

public:

    CConsoleLogger()
        : ILogger()
    {
    }


    void LogEvent(string event);

};
