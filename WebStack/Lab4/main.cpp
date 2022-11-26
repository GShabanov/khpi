
#include <windows.h>
#include <tchar.h>
#include <string>

#include "EmailNotification.h"
#include "Slack.h"
#include "SlackAdapter.h"
#include "Sms.h"
#include "SmsAdapter.h"

using namespace std;


void SendMessage(INotification *notification, string title, string message)
{
    notification->Send(title, message);
}

int
_tmain(int argc, TCHAR* argv[])
{


    string message = "Hello World!";

    INotification *emailNotification = new CEmailNotification("mymail@email.com");

    SendMessage(emailNotification, "title", message);

    CSlack *slack = new CSlack();
    CSlackAdapter *slackAdapter = new CSlackAdapter(slack);
    slackAdapter->Authorize("mylogin", "1231242");
    slackAdapter->SetChatId("chatId");

    SendMessage(slackAdapter, "", message);

    CSms *sms = new CSms();
    CSmsAdapter *smsAdapter = new CSmsAdapter(sms);
    smsAdapter->SetSenderNumber("+38(063)112-123-123");
    smsAdapter->SetRecepientNumber("+38(072)112-123-123");

    SendMessage(smsAdapter, "", message);


    return 0;
}

