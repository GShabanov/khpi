#pragma once

#include <iostream>
#include "Notification.h"

class CSlackAdapter : public INotification
{

private:
    class CSlack* m_slack;
    std::string m_chatId;

public:
    void Send(std::string title, std::string message);
    void Authorize(std::string login, std::string apiKey);

    void SetChatId(std::string chatId)
    {
        m_chatId = chatId;
    }

    CSlackAdapter(class CSlack* slack);
};