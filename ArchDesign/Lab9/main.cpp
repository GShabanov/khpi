
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

#include "YoutubeClient.h" 


int
_tmain(int argc, TCHAR* argv[])
{

    //
    // Фасад — це структурний патерн проектування, який надає простий інтерфейс до складної системи класів, бібліотеки або фреймворку.
    //
    CYoutubeClient   youTubeClient("mylogin", "mypassword");

    //
    // вкажкий інтерфейс обгорнутий у виклик одного апі 
    //
    youTubeClient.UploadFile("C:\\localshare\\myfile.mp4");

    return 0;
}

