
#include "Sms.h"
#include "SmsAdapter.h"

void CSmsAdapter::Send(std::string title, std::string message)
{
    if (m_senderPhoneNumber.empty())
        throw std::string("phone empty");

    if (m_recepientPhoneNumber.empty())
        throw std::string("phone empty");


    m_sms->SendMessage(m_senderPhoneNumber, m_recepientPhoneNumber, message);
}

void CSmsAdapter::SetSenderNumber(std::string number)
{
    m_senderPhoneNumber = number;
}

void CSmsAdapter::SetRecepientNumber(std::string number)
{
    m_recepientPhoneNumber = number;
}

CSmsAdapter::CSmsAdapter(CSms* sms)
{
    m_sms = sms;
}
