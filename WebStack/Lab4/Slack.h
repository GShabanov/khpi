#pragma once

#include <iostream>

class CSlack
{
private:
    bool m_isAuthorized;

public:

    CSlack()
    {
        m_isAuthorized = false;
    }

    void Authorize(std::string login, std::string apiKey)
    {
        if (login.empty())
            throw std::string("invalid login");

        if (apiKey.empty())
            throw std::string("invalid key");

        m_isAuthorized = true;

        std::cout << "Slack: " << "Authorized" << std::endl;

    }

    void SendMessage(std::string chatId, std::string message)
    {
        if (!m_isAuthorized)
            throw std::string("Not Authorized");

        std::cout << "Slack: " << chatId << ": message" << std::endl;
    }
};