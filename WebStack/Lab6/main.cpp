
#include <windows.h>
#include <tchar.h>
#include <string>

#include "Downloader.h"
#include "ProxyDownloader.h"

using namespace std;


class DownloadClient
{
public:
    unsigned char *Download(IDownloader* downloader, string url)
    {
        unsigned char* data = downloader->Download(url);

        return data;
    }
};

int
_tmain(int argc, TCHAR* argv[])
{

    IDownloader *downloader = new CSimpleDownloader();
    IDownloader *cachedDownloader = new CProxyDownloader(downloader);

    DownloadClient *client = new DownloadClient();

    unsigned char* data;

    data = client->Download(cachedDownloader, "ftp://warezzz.com/file1");

    delete[] data;
    //
    // must be cached in proxy
    //
    data = client->Download(cachedDownloader, "ftp://warezzz.com/file1");

    delete[] data;

    data = client->Download(cachedDownloader, "ftp://warezzz.com/file2");

    delete[] data;


    return 0;
}

