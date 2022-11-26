
#include "SocialMedia.h"
#include "FacebookMedia.h"
#include "LinkedInMedia.h"

ISocialMedia*
ISocialMedia::Get(SocialNetworkType type)
{
    ISocialMedia* _return;
    switch (type)
    {
    case SocialNetworkType::Facebook:

        _return = new CFacebookMedia();
        break;

    case SocialNetworkType::LinkedIn:
        _return = new CLinkedInMedia();
        break;

    default:
        _return = NULL;
    }

    return _return;
}