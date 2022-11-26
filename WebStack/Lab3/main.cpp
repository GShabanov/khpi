
#include <windows.h>
#include <tchar.h>
#include <string>

#include "SocialMedia.h"
#include "FacebookMedia.h"
#include "LinkedInMedia.h"



int
_tmain(int argc, TCHAR* argv[])
{


    ISocialMedia* facebookMedia = ISocialMedia::Get(ISocialMedia::SocialNetworkType::Facebook);

    if (facebookMedia != NULL)
    {
        facebookMedia->Authorize("myUser", "123124");


        facebookMedia->PostMessage("Message!!");
    }

    ISocialMedia* LinkedInMedia = ISocialMedia::Get(ISocialMedia::SocialNetworkType::LinkedIn);

    if (LinkedInMedia != NULL)
    {
        LinkedInMedia->Authorize("myUser@email.com", "123124");


        LinkedInMedia->PostMessage("Message!!");
    }

    return 0;
}

