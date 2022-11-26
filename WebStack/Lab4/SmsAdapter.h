#pragma once

#include <iostream>
#include "Notification.h"


class CSmsAdapter : public INotification
{

private:
    class CSms* m_sms;
    std::string m_senderPhoneNumber;
    std::string m_recepientPhoneNumber;

public:
    void Send(std::string title, std::string message);

    void SetSenderNumber(std::string number);

    void SetRecepientNumber(std::string number);

    CSmsAdapter(class CSms* sms);
};