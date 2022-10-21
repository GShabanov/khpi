
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "FPArray.h"

const float myArray[] = { 
    71, 115, 86, 103, 102, 76, 103, 46, 120, 52, 55, 88, 102, 97, 62, 95, 102, 70, 99, 115, 72, 120, 90, 111, 91, 74, 91, 107, 85, 101, 75, 87, 102, 131, 89, 116, 72, 103, 114, 77 
};

const float sampleArray[] = {
    183, 170, 176, 178, 176, 180, 176, 185, 184, 174, 168, 174, 189, 172, 175, 167, 179, 176, 169, 178,
    169, 171, 170, 177, 176, 179, 174, 176, 188, 178, 172, 176, 167, 166, 180, 183, 176, 182, 178, 172,
    185, 183, 175, 174, 180, 166, 169, 171, 178, 169, 170, 179, 171, 178, 173, 177 
};


void
DumpTable(CFPArray<float> &data)
{
    size_t  rover = 0;

    for (int i = 0; i < (data.Size() / 5); i++)
    {
        _tprintf(_T("* "));

        for (int j = 0; j < 5; j++)
        {
            _tprintf(_T("%4.4f "), data[i * 5 + j]);
            rover++;
        }

        _tprintf(_T("\n"));
    }


    if ((data.Size() % 5) > 0)
    {
        _tprintf(_T("* "));

        for (size_t i = rover; i < data.Size(); i++)
        {
            _tprintf(_T("%4.4f "), data[i]);
        }

        _tprintf(_T("\n"));
    }

}

//
// клас которий описуЇ одну группу
//
class CValuesClass
{
private:


    float  m_arithmeticMean;
    float  m_standartDeviation;

    float  m_Bx;
    float  m_Bxx;
    float  m_dX;
    float  m_Z;
    //const float *m_inputTable;          // вх≥дна таблиц€
    //size_t       m_InputTableSize;      // њњ розм≥р

    CFPArray<float>  m_valuesTable;       // таблиц€ значень побудована

    CFPArray<float>  m_inputTable;        // вх≥дна таблиц€
public:

    CValuesClass(
        const CFPArray<float>& inputArray,
        float                  arithmeticMean,    // arithmetic mean of the base table
        float                  standartDeviation  // standart deviation of the base table
        )
        : m_valuesTable()
        , m_inputTable(inputArray)
    {

        m_arithmeticMean = arithmeticMean;

        m_standartDeviation = standartDeviation;
    }

    ~CValuesClass()
    {
    }

    float GetMean()
    {
        return m_valuesTable.GetMean();
    }

    int GetFrequency()
    {
        return (int)m_valuesTable.Size();
    }

    float GetBx()
    {
        return m_Bx;
    }

    float GetBxx()
    {
        return m_Bxx;
    }

    float GetDx()
    {
        return m_dX;
    }

    float GetZ()
    {
        return m_Z;
    }



    CFPArray<float> &GetValuesTable()
    {
        return m_valuesTable;
    }

    bool Build(float minValue, float maxValue)
    {

        int     count = 0;

        //
        // precalculate future table size
        //
        for (int i = 0; i < m_inputTable.Size(); i++)
        {

            if (m_inputTable[i] >= minValue && m_inputTable[i] < maxValue)
            {
                count++;
            }
        }

        m_valuesTable = CFPArray<float>(count);

        count = 0;

        //
        // insert values
        //
        for (int i = 0; i < m_inputTable.Size(); i++)
        {

            if (m_inputTable[i] >= minValue && m_inputTable[i] < maxValue)
            {
                m_valuesTable[count] = (float)m_inputTable[i];

                count++;
            }
        }

        m_Bx = m_valuesTable.Size() * m_valuesTable.GetMean();
        m_Bxx = m_valuesTable.Size() * m_valuesTable.GetMean() * m_valuesTable.GetMean();
        m_dX = m_valuesTable.GetMean() - m_arithmeticMean;

        //
        // Z = | (X - X') / S |
        //
        m_Z = m_dX / m_standartDeviation;

        //
        // module
        //
        if (m_Z < 0)
            m_Z = -m_Z;

        return true;
    }
};

int
_tmain(int argc, TCHAR* argv[])
{

    //CFPArray<float>  inputArray(myArray, ARRAYSIZE(myArray));
    CFPArray<float>  inputArray(sampleArray, ARRAYSIZE(sampleArray));

    CFPArray<float>  deviationTable;

    float            maxValue = 0;
    float            minValue = 0;
    float            groupCount = 0.0;
    int              groupCountMin = 0;
    int              groupCountMax = 0;

    _tprintf(_T("Lab 1\n"));

    _tprintf(_T("Array size %d\n"), (int)inputArray.Size());

    //
    // prepare calculation for 'X'
    //
    float averageNumber = 0;

    maxValue = inputArray[0];
    minValue = inputArray[0];

    for (int i = 0; i < inputArray.Size(); i++) {

        averageNumber += inputArray[i];

        if (inputArray[i] > maxValue) {

            maxValue = inputArray[i];
        }

        if (inputArray[i] < minValue) {

            minValue = inputArray[i];
        }
    }

    averageNumber /= inputArray.Size();

    _tprintf(_T("Average number %4.4f\n"), averageNumber);
    _tprintf(_T("Max number %4.4f\n"), maxValue);
    _tprintf(_T("Min number %4.4f\n"), minValue);
    _tprintf(_T("delta %4.4f\n"), maxValue - minValue);
    //
    // Sturgerts classes
    //
    groupCount = (float)(1 + 3.32 * log10f((float)inputArray.Size()));
    _tprintf(_T("aproximation groups count %4.4f\n"), groupCount);

    groupCountMin = (int)groupCount;
    groupCountMax = (int)groupCount + ((groupCount - (float)(unsigned int)groupCount) > 0.2 ? 1 : 0);

    

    _tprintf(_T("groups count min %d\n"), groupCountMin);
    _tprintf(_T("groups count max %d\n"), groupCountMax);

    if (groupCountMax > groupCountMin) {

        //groupCountMin = groupCountMax;

        _tprintf(_T("assume max value for group count %d\n"), groupCountMin);
    }

    int stepsize = (int)ceil((maxValue - minValue) / groupCountMin);

    _tprintf(_T("step size %d\n"), stepsize);

    //
    // calulating deviation table
    //
    deviationTable = CFPArray<float>(inputArray.Size());

    for (int i = 0; i < inputArray.Size(); i++)
    {
        deviationTable[i] = inputArray[i] - averageNumber;
    }

    //
    // dump deviation table
    //
    _tprintf(_T("deviation table\n"));
    DumpTable(deviationTable);

    //
    // corrected sample standard deviation
    //
    float  standartDeviation = 0;

    for (int i = 0; i < inputArray.Size(); i++)
    {
        standartDeviation += (deviationTable[i] * deviationTable[i]);
    }

    standartDeviation /= (inputArray.Size() - 1);

    _tprintf(_T("Squre S %4.4f\n"), standartDeviation);

    standartDeviation = sqrt(standartDeviation);

    _tprintf(_T("sample standard deviation S %f\n"), standartDeviation);


    CValuesClass** m_classes = new CValuesClass*[groupCountMin];

    //
    // for future iteration we use min value in table
    //
    float  baseValue = minValue;
    for (int i = 0; i < groupCountMin; i++)
    {

        m_classes[i] = new CValuesClass(inputArray, averageNumber, standartDeviation);

        _tprintf(_T("Group %d\n"), i + 1);
        _tprintf(_T("from %4.4f to %4.4f\n"), baseValue, floor(baseValue + stepsize));

        m_classes[i]->Build(baseValue, floor(baseValue + stepsize));

        DumpTable(m_classes[i]->GetValuesTable());

        _tprintf(_T("frequency %d\n"), (int)m_classes[i]->GetFrequency());
        _tprintf(_T("mean value %4.4f\n"), m_classes[i]->GetMean());

        _tprintf(_T("B*x %4.4f\n"), m_classes[i]->GetBx());
        _tprintf(_T("B*x^2 %4.4f\n"), m_classes[i]->GetBxx());

        _tprintf(_T("delta X %4.4f\n"), m_classes[i]->GetDx());
        _tprintf(_T("Z %4.4f\n"), m_classes[i]->GetZ());

        _tprintf(_T("\n"));

        baseValue += stepsize;
    }




    for (int i = 0; i < groupCountMin; i++)
    {
        delete m_classes[i];
    }

    delete[] m_classes;


    return 0;
}

