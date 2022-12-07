
#include <windows.h>
#include <tchar.h>
#include <string>

#include "Client.h"
#include "Page.h"

using namespace std;


int
_tmain(int argc, TCHAR* argv[])
{


    CClient *client = new CClient();

    IPage *simplePage = new CSimplePage("Title", "Content");
    IPage* productPage = new CProductPage(1, "My Product", "my best product", "./image.jpeg");

    IRenderer *htmlRenderer1 = new CHtmlRenderer(simplePage);
    client->Render(htmlRenderer1);

    IRenderer *htmlRenderer2 = new CHtmlRenderer(productPage);
    client->Render(htmlRenderer2);

    IRenderer *jsonRenderer = new CJsonRenderer(simplePage);
    client->Render(jsonRenderer);

    IRenderer* xmlRenderer = new CXmlRenderer(productPage);
    client->Render(xmlRenderer);


    return 0;
}

