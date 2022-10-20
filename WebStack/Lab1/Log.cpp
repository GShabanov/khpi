#define _CRT_SECURE_NO_WARNINGS

#include <locale>
#include <iostream>
#include <sstream>
#include <time.h>
#include "log.h"

using namespace std;



void
CConsoleLogger::LogEvent(string event)
{

    std::time_t result = std::time(nullptr);
    

    std::cout << std::asctime(std::localtime(&result))
        <<  event << std::endl;

}
