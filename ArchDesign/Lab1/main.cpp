
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "MyObjects.h"


class CPoiObject
{
public:

    virtual const wchar_t* GetName() = 0;
};

class CMaidan : public CPoiObject
{
    const wchar_t* GetName()
    {
        return L"Maidan";
    }
};

class CSofiKievska : public CPoiObject
{
    const wchar_t* GetName()
    {
        return L"Sofia Kyivska";
    }
};

class COskaniaNova : public CPoiObject
{
    const wchar_t* GetName()
    {
        return L"Okania Nova";
    }
};

class CSofievka : public CPoiObject
{
    const wchar_t* GetName()
    {
        return L"Sofievka";
    }
};

int
_tmain(int argc, TCHAR* argv[])
{

    CPoiArray<CPoiObject *>  poiArray;

    poiArray.AddNew(new CMaidan());
    poiArray.AddNew(new CSofiKievska());
    poiArray.AddNew(new COskaniaNova());
    poiArray.AddNew(new CSofievka());

    //
    // standart Array iteration
    //
    std::wcout << L"Walk using simple array (just for test) \n";

    for (int i = 0; i < poiArray.Size(); i++)
    {
        const wchar_t *poi = poiArray[i]->GetName();

        std::wcout << L"name: " << poi << L"\n";

    }

    std::wcout << L"\n";
    std::wcout << L"Walk using Turist iterator \n";

    for (Iterator<CPoiObject *> *a = poiArray.getTouristIterator(); !a->isDone(); a->next())
    {
        const wchar_t* poi = a->currentItem()->GetName();

        std::wcout << L"name: " << poi << L"\n";

    }

    std::wcout << L"\n";
    std::wcout << L"Walk using Navigator iterator \n";

    for (Iterator<CPoiObject*>* a = poiArray.getNavigatorIterator(); !a->isDone(); a->next())
    {
        const wchar_t* poi = a->currentItem()->GetName();

        std::wcout << L"name: " << poi << L"\n";

    }

    std::wcout << L"\n";
    std::wcout << L"Walk using Gyde iterator \n";

    for (Iterator<CPoiObject*>* a = poiArray.getGydeIterator(); !a->isDone(); a->next())
    {
        const wchar_t* poi = a->currentItem()->GetName();

        std::wcout << L"name: " << poi << L"\n";

    }


    return 0;
}

