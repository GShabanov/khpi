#pragma once


#include <iostream>

class CSms
{

public:

    CSms()
    {
    }

    void SendMessage(std::string senderPhone, std::string recepientPhone, std::string text)
    {

        std::cout << "Sms: from: " << senderPhone << " to: " << recepientPhone << " Text: " << text << std::endl;
    }
};
