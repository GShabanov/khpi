
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <assert.h>
#include "FPArray.h"

#define Pi  ((double)3.1415926535)


const double myArray[] = { 
    71, 115, 86, 103, 102, 76, 103, 46, 120, 52, 55, 88, 102, 97, 62, 95, 102, 70, 99, 115, 72, 120, 90, 111, 91, 74, 91, 107, 85, 101, 75, 87, 102, 131, 89, 116, 72, 103, 114, 77 
};

const double sampleArray[] = {
    183, 170, 176, 178, 176, 180, 176, 185, 184, 174, 168, 174, 189, 172, 175, 167, 179, 176, 169, 178,
    169, 171, 170, 177, 176, 179, 174, 176, 188, 178, 172, 176, 167, 166, 180, 183, 176, 182, 178, 172,
    185, 183, 175, 174, 180, 166, 169, 171, 178, 169, 170, 179, 171, 178, 173, 177 
};

template <class Type>
void
DumpTable(CFPArray<Type> &data)
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
template <class Type>
class CValuesClass
{
private:


    Type  m_arithmeticMean;
    Type  m_standartDeviation;

    Type  m_Bx;
    Type  m_Bxx;
    Type  m_dX;
    Type  m_Z;
    Type  m_fZ;
    Type  m_K1;
    //const float *m_inputTable;          // вх≥дна таблиц€
    //size_t       m_InputTableSize;      // њњ розм≥р

    CFPArray<Type>  m_valuesTable;       // таблиц€ значень побудована

    CFPArray<Type>  m_inputTable;        // вх≥дна таблиц€
public:

    CValuesClass(
        const CFPArray<Type>& inputArray,
        Type                  arithmeticMean,    // arithmetic mean of the base table
        Type                  standartDeviation  // standart deviation of the base table
        )
        //: m_valuesTable()
        : m_inputTable(inputArray)
    {

        m_arithmeticMean = arithmeticMean;

        m_standartDeviation = standartDeviation;
    }

    ~CValuesClass()
    {
    }

    Type GetMean() { return m_valuesTable.GetMean(); }

    int GetFrequency() { return (int)m_valuesTable.Size(); }

    Type GetBx() { return m_Bx; }

    Type GetBxx() { return m_Bxx; }

    Type GetDx() { return m_dX; }

    Type GetZ() { return m_Z; }

    Type GetFz() { return m_fZ; }

    Type GetK(Type S) { return m_K1 / S; }


    CFPArray<Type> &GetValuesTable() {
        return m_valuesTable;
    }

    bool Build(Type minValue, Type maxValue)
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

        m_valuesTable = CFPArray<Type>(count);

        count = 0;

        //
        // insert values
        //
        for (int i = 0; i < m_inputTable.Size(); i++)
        {

            if (m_inputTable[i] >= minValue && m_inputTable[i] < maxValue)
            {
                m_valuesTable[count] = (Type)m_inputTable[i];

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


        m_fZ = (Type)(1 / sqrt(2 * Pi) * exp(-(m_Z * m_Z / 2)));


        //
        // K' = n * b / S ==>> K1 = n * b
        //
        m_K1 = (Type)(m_inputTable.Size() * m_valuesTable.Size());

        return true;
    }
};

template <class Type>
void
CalculateArray(CFPArray<Type>& inputArray)
{
    Type            maxValue = 0;
    Type            minValue = 0;
    Type            classesCount = 0.0;
    int              classesCountMin = 0;
    int              classesCountMax = 0;

    _tprintf(_T("Array size %d\n"), (int)inputArray.Size());

    //
    // prepare calculation for 'X'
    //
    Type averageNumber = 0;

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
    classesCount = (Type)(1 + 3.32 * log10f((float)inputArray.Size()));
    _tprintf(_T("aproximation classes count %4.4f\n"), classesCount);

    classesCountMin = (int)classesCount;
    classesCountMax = (int)classesCount + ((classesCount - (Type)(unsigned int)classesCount) > 0.2 ? 1 : 0);



    _tprintf(_T("classes count min %d\n"), classesCountMin);
    _tprintf(_T("classes count max %d\n"), classesCountMax);

    if (classesCountMax > classesCountMin) {

        classesCountMin = classesCountMax;

        _tprintf(_T("assume max value for classes count %d\n"), classesCountMin);
    }

    int stepsize = (int)floor((maxValue - minValue) / classesCountMin);

    _tprintf(_T("step size %d\n\n"), stepsize);

    //
    // calulating deviation table
    //
    CFPArray<Type>  deviationTable(inputArray.Size());

    for (int i = 0; i < inputArray.Size(); i++)
    {
        deviationTable[i] = inputArray[i] - averageNumber;
    }

    //
    // dump deviation table
    //
    _tprintf(_T("deviation table\n"));
    _tprintf(_T("-------------------------------------\n"));
    DumpTable(deviationTable);
    _tprintf(_T("-------------------------------------\n"));
    //
    // corrected sample standard deviation
    //
    Type  standartDeviation = 0;

    for (int i = 0; i < inputArray.Size(); i++)
    {
        standartDeviation += (deviationTable[i] * deviationTable[i]);
    }

    standartDeviation /= (inputArray.Size() - 1);

    _tprintf(_T("Squre S %4.4f\n"), standartDeviation);

    //
    // standart deviation
    //
    standartDeviation = (Type)sqrt(standartDeviation);

    _tprintf(_T("sample standard deviation S %f\n"), standartDeviation);


    CValuesClass<Type>** m_classes = new CValuesClass<Type> * [classesCountMin];

    CFPArray<Type>  SumBx(classesCountMin);
    CFPArray<Type>  SumBxx(classesCountMin);
    //
    // for future iteration we use min value in table
    //
    Type  baseValue = minValue;
    for (int i = 0; i < classesCountMin; i++)
    {

        m_classes[i] = new CValuesClass<Type>(inputArray, averageNumber, standartDeviation);

        _tprintf(_T("----------------------------------------------------\n"));
        _tprintf(_T("Class %d\n"), i + 1);
        _tprintf(_T("from %4.4f to %4.4f\n"), baseValue, floor(baseValue + stepsize));

        m_classes[i]->Build(baseValue, (Type)floor(baseValue + stepsize));

        DumpTable<Type>(m_classes[i]->GetValuesTable());

        _tprintf(_T("frequency   = %d\n"), (int)m_classes[i]->GetFrequency());
        _tprintf(_T("mean value  = %4.4f\n"), m_classes[i]->GetMean());

        _tprintf(_T("B*x         = %4.4f\n"), m_classes[i]->GetBx());
        SumBx[i] = m_classes[i]->GetBx();
        _tprintf(_T("B*x^2       = %4.4f\n"), m_classes[i]->GetBxx());
        SumBxx[i] = m_classes[i]->GetBxx();

        _tprintf(_T("delta X     = %4.4f\n"), m_classes[i]->GetDx());
        _tprintf(_T("Z           = %4.4f\n"), m_classes[i]->GetZ());

        _tprintf(_T("\n"));

        baseValue += stepsize;
    }

    // _
    // S  =  SQRT (  summ(Bx^2) - (summ(Bx) ^ 2 / n) / (n - 1) )
    //
    Type S_class1 = SumBxx.GetSumm();
    Type S_class2 = (SumBx.GetSumm() * SumBx.GetSumm() / inputArray.Size());
    Type S_class = (Type)sqrt((S_class1 - S_class2) / (inputArray.Size() - 1));

    _tprintf(_T(" _\n"));
    _tprintf(_T(" S      = %4.4f\n"), S_class);
    _tprintf(_T(" _\n"));
    _tprintf(_T(" X      = %4.4f\n"), averageNumber);
    _tprintf(_T(" Nu     = %4.4f%%\n\n"), (S_class / averageNumber) * 100);




    CFPArray<Type>  FzK(classesCountMin);

    for (int i = 0; i < classesCountMin; i++)
    {
        FzK[i] = m_classes[i]->GetFz() * m_classes[i]->GetK(S_class);
    }


    _tprintf(_T("F(z) * K' table\n"));
    DumpTable<Type>(FzK);
    _tprintf(_T("\n"));


    //
    // Note: Arrays are null based !
    //
    CFPArray<Type>  frequencyE(classesCountMin - 1);
    CFPArray<Type>  BminusE(classesCountMin - 1);
    CFPArray<Type>  BminusE2E(classesCountMin - 1);

    //
    // without last one !
    //
    for (int i = 0; i < classesCountMin - 2; i++)
    {
        frequencyE[i] = FzK[i];
        BminusE[i] = m_classes[i]->GetFrequency() - frequencyE[i];
    }

    //
    // 5 + 6 classes
    //
    Type lastE = FzK[classesCountMin - 2] + FzK[classesCountMin - 1];

    //
    // Note: Arrays are null based !
    //
    frequencyE[classesCountMin - 2] = lastE;
    BminusE[classesCountMin - 2] = m_classes[classesCountMin - 1]->GetFrequency() + m_classes[classesCountMin - 2]->GetFrequency() - lastE;


    _tprintf(_T("E table\n"));
    DumpTable(frequencyE);
    _tprintf(_T("\n"));

    _tprintf(_T("B - E table\n"));
    DumpTable(BminusE);
    _tprintf(_T("\n"));


    for (int i = 0; i < classesCountMin - 1; i++)
    {
        BminusE2E[i] = (BminusE[i] * BminusE[i]) / frequencyE[i];
    }

    _tprintf(_T("(B - E)^2 / E table\n"));
    DumpTable(BminusE2E);
    _tprintf(_T("\n"));

    _tprintf(_T(" ^\n"));
    _tprintf(_T(" X ^ 2         = %4.4f\n"), BminusE2E.GetSumm());

    for (int i = 0; i < classesCountMin; i++)
    {
        delete m_classes[i];
    }

    delete[] m_classes;

}

int
_tmain(int argc, TCHAR* argv[])
{

    CFPArray<double>  inputArray(myArray, ARRAYSIZE(myArray));
    //CFPArray<float>  inputArray(sampleArray, ARRAYSIZE(sampleArray));

    _tprintf(_T("===============================\n"));
    _tprintf(_T("           Lab 1\n"));
    _tprintf(_T("===============================\n"));

    CalculateArray(inputArray);

    _tprintf(_T("===============================\n"));
    _tprintf(_T("==  X' = lg(x) * 100           \n"));
    _tprintf(_T("===============================\n"));

    CFPArray<double>  covertedArray(inputArray);

    for (int i = 0; i < inputArray.Size(); i++)
    {
        covertedArray[i] = log10(inputArray[i]) * 100;
    }

    CalculateArray(covertedArray);


    return 0;
}

