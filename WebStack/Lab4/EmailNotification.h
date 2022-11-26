#pragma once

#include <iostream>
#include "Notification.h"


class CEmailNotification : public INotification
{
private:
    std::string m_adminEmail;

public:
    void Send(std::string title, std::string message)
    {

        std::cout << "Email: title <" << title << "> to <" << m_adminEmail << "> message: " << message << std::endl;
    }

    CEmailNotification(std::string adminEmail)
        : INotification()
    {
        m_adminEmail = adminEmail;
    }
};