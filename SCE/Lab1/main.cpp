
#include <windows.h>
#include <tchar.h>
#include <math.h>

const float myArray[] = { 
    71, 115, 86, 103, 102, 76, 103, 46, 120, 52, 55, 88, 102, 97, 62, 95, 102, 70, 99, 115, 72, 120, 90, 111, 91, 74, 91, 107, 85, 101, 75, 87, 102, 131, 89, 116, 72, 103, 114, 77 
};

void
DumpTable(float* table, size_t arraySize)
{
    size_t  rover = 0;

    for (int i = 0; i < (arraySize / 5); i++)
    {
        _tprintf(_T("* "));

        for (int j = 0; j < 5; j++)
        {
            _tprintf(_T("%4.4f "), table[i * 5 + j]);
            rover++;
        }

        _tprintf(_T("\n"));
    }


    if ((arraySize % 5) > 0)
    {
        _tprintf(_T("* "));

        for (size_t i = rover; i < arraySize; i++)
        {
            _tprintf(_T("%4.4f "), table[i]);
        }

        _tprintf(_T("\n"));
    }

}

class CValuesClass
{
private:
    float* m_values;
    size_t m_valuesCount;
    float  m_avaregeValue;


    const float *m_inputTable;
    size_t       m_InputTableSize;

public:

    CValuesClass(const float* table, size_t arraySize)
    {
        m_values = NULL;
        m_valuesCount = 0;
        m_avaregeValue = 0;

        m_inputTable     = table;
        m_InputTableSize = arraySize;


    }

    ~CValuesClass()
    {
        if (m_values != NULL) {
            delete m_values;
        }
    }

    float GetAvarage()
    {
        _tprintf(_T("avarage value %4.4f\n\n"), m_avaregeValue);
        return m_avaregeValue;
    }

    int GetFrequency()
    {
        return (int)m_InputTableSize;
    }

    bool Build(float minValue, float maxValue)
    {
        _tprintf(_T("from %4.4f to %4.4f\n"), minValue, maxValue);

        int   count = 0;

        for (int i = 0; i < m_InputTableSize; i++)
        {

            if (m_inputTable[i] >= minValue && m_inputTable[i] < maxValue)
            {
                //values[count] = (float)table[i];

                count++;
            }
        }

        m_values = new float[count];

        if (m_values == NULL)
            return false;

        count = 0;

        for (int i = 0; i < m_InputTableSize; i++)
        {

            if (m_inputTable[i] >= minValue && m_inputTable[i] < maxValue)
            {
                m_values[count] = (float)m_inputTable[i];

                count++;
            }
        }

        m_valuesCount = count;
        _tprintf(_T("count %d\n"), count);

        float summ = 0.0;

        if (count > 0)
        {
            _tprintf(_T(" *"));
        }

        for (int i = 0; i < count; i++)
        {
            _tprintf(_T(" %4.4f"), myArray[i]);
            summ += m_values[i];
        }

        _tprintf(_T("\n"));

        m_avaregeValue = summ / count;

        return true;
    }
};

int
_tmain(int argc, TCHAR* argv[])
{
    size_t        arraySize = ARRAYSIZE(myArray);
    const float  *initialArray = myArray;
    float         maxValue = 0;
    float         minValue = 0;
    float         groupCount = 0.0;
    int           groupCountMin = 0;
    int           groupCountMax = 0;

    _tprintf(_T("Lab 1\n"));

    _tprintf(_T("Array size %d\n"), (int)arraySize);

    //
    // prepare calculation for 'X'
    //
    float averageNumber = 0;

    maxValue = initialArray[0];
    minValue = initialArray[0];

    for (int i = 0; i < arraySize; i++) {

        averageNumber += initialArray[i];

        if (initialArray[i] > maxValue) {

            maxValue = initialArray[i];
        }

        if (initialArray[i] < minValue) {

            minValue = initialArray[i];
        }
    }

    averageNumber /= arraySize;

    _tprintf(_T("Average number %4.4f\n"), averageNumber);
    _tprintf(_T("Max number %4.4f\n"), maxValue);
    _tprintf(_T("Min number %4.4f\n"), minValue);
    _tprintf(_T("delta %4.4f\n"), maxValue - minValue);
    //
    // Sturgerts classes
    //
    groupCount = (float)(1 + 3.32 * log10f((float)arraySize));
    _tprintf(_T("aproximation groups count %4.4f\n"), groupCount);

    groupCountMin = (int)groupCount;
    groupCountMax = (int)groupCount + ((groupCount - (float)(unsigned int)groupCount) > 0.2 ? 1 : 0);

    

    _tprintf(_T("groups count min %d\n"), groupCountMin);
    _tprintf(_T("groups count max %d\n"), groupCountMax);

    if (groupCountMax > groupCountMin) {

        groupCountMin = groupCountMax;

        _tprintf(_T("assume max value for group count %d\n"), groupCountMin);
    }

    int stepsize = (int)((maxValue - minValue) / groupCountMin);

    _tprintf(_T("step size %d\n"), stepsize);

    //
    // calulate displacion
    //
    float* di = new float[arraySize];

    for (int i = 0; i < arraySize; i++)
    {
        di[i] = initialArray[i] - averageNumber;
    }

    //
    // dump displacement table
    //
    _tprintf(_T("Displacion table\n"));
    DumpTable(di, arraySize);

    //
    // calculate quadratic displacement avarage sum
    //
    float  Ssq = 0;

    for (int i = 0; i < arraySize; i++)
    {
        Ssq += (di[i] * di[i]);
    }

    Ssq /= (arraySize - 1);

    _tprintf(_T("Squre S %4.4f\n"), Ssq);

    _tprintf(_T("Avarage displacement S %f\n"), sqrt(Ssq));


    CValuesClass** m_classes = new CValuesClass*[groupCountMin];

    //
    // for future iteration we use min value in table
    //
    float  baseValue = minValue;
    for (int i = 0; i < groupCountMin; i++)
    {

        m_classes[i] = new CValuesClass(initialArray, arraySize);

        _tprintf(_T("Group %d\n"), i + 1);

        m_classes[i]->Build(baseValue, baseValue + stepsize);

        m_classes[i]->GetAvarage();

        baseValue += stepsize;
    }




    for (int i = 0; i < groupCountMin; i++)
    {
        delete m_classes[i];
    }

    delete[] m_classes;


    return 0;
}

