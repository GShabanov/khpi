
#include "SlackAdapter.h"
#include "Slack.h"

CSlackAdapter::CSlackAdapter(CSlack* slack)
{
    m_slack = slack;
}

void CSlackAdapter::Send(std::string title, std::string message)
{
    m_slack->SendMessage(m_chatId, message);
}

void CSlackAdapter::Authorize(std::string login, std::string apiKey)
{
    m_slack->Authorize(login, apiKey);
}

