#include "pch.h"
#include "framework.h"
#include "Neuron.h"

#ifndef __HEMMING_NETWORK_CPP__
#define __HEMMING_NETWORK_CPP__
#include "HemmingNetwork.h"

template <class Type>
CHemmingNetwork<Type>::CHemmingNetwork(size_t effectorsCount)
{
    m_InputsCount = effectorsCount;
    m_Initialized = false;
    m_Trained = false;
}


template <class Type>
CHemmingNetwork<Type>::~CHemmingNetwork()
{
    if (!m_Initialized)
        return;


    //
    // ��������� �������
    //
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        delete m_Neurons[i];
    }

    m_Neurons.RemoveAll();
}


template <class Type>
void
CHemmingNetwork<Type>::Reset()
{
    //
    // ������ ���
    //
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        for (SIZE_T j = 0; j < m_InputsCount; j++)
        {
            m_weightMatrix1[i * m_InputsCount + j] = (Type)(m_InputsCount / 2);
        }
    }

    //
    // ������ ���
    //
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        for (SIZE_T j = 0; j < m_InputsCount; j++)
        {
            m_weightMatrix2[i * m_InputsCount + j] = 0.0;
        }
    }

    


    m_Initialized = true;
}

template <class Type>
bool
CHemmingNetwork<Type>::Init()
{

    bool  initialized = true;
    //
    // ������ ��� ��������
    //

    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {

        m_Effectors.Add(0.0);
    }

    //
    // ������ ��� ������� (������, ������ ��������)
    //

    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        CNeuron<Type>   *neuron;

        neuron = new CNeuron<Type>(0);

        if (neuron != NULL)
        {
            m_Neurons.Add(neuron);
        }
        else
        {
            initialized = false;
        }
    }

    if (initialized == false)
        return false;

    //
    // ������ �������� ������� ����
    //
    m_weightMatrix1.SetSize(m_InputsCount * m_InputsCount);
    m_weightMatrix2.SetSize(m_InputsCount * m_InputsCount);

    this->Reset();


    m_Initialized = initialized;

    return initialized;
}


template <class Type>
void
CHemmingNetwork<Type>::Teach(const CArray<Type>& data)
{

    //
    // ����������� ���� �������� �� ���������
    //
    for (SIZE_T i = 0; i < m_InputsCount; i++)
    {
        m_Effectors[i] = data[i];
    }



    //
    // ���������� �������� �� ����� �������
    //
    for (SIZE_T j = 0; j < m_InputsCount; j++)    
    {

        Type  n = 0;

        for (SIZE_T i = j; i < m_InputsCount; i++)
        {
            n++;
            //
            // ���� ������� ��������
            //
            if (i == j)
            {
                m_weightMatrix1[j * m_InputsCount + i] = 0;
            }
            else
            {
                Type sum = (m_Effectors[i] * m_Effectors[j]);

                m_weightMatrix1[j * m_InputsCount + i] += (sum / n);
                m_weightMatrix1[i * m_InputsCount + j] += (sum / n);
            }

        }

    }

}

template <class Type>
void
CHemmingNetwork<Type>::Recovery(const CArray<Type>& input, CArray<Type>& output, CNetworkUpdateCallback<Type>* callbackClass)
{
    CArray<Type> outputTmp;

    m_Effectors.SetSize(input.GetSize());
    outputTmp.SetSize(input.GetSize());



    //
    // ����������� ���� �������� �� ���������
    //
    m_Effectors.Copy(input);
    outputTmp.Copy(input);


    //
    // ���������� �����
    //
    output.RemoveAll();
    output.SetSize(input.GetSize());


    //
    // �������� ���� �� ���� ���������� �����
    //
    for ( ; ; )
    {
        //
        // ���������� �������� �� ����� �������
        //
        for (SIZE_T j = 0; j < m_InputsCount; j++)
        {

            Type sum = 0;

            for (SIZE_T i = 0; i < m_InputsCount; i++)
            {

                if (i != j)
                {
                    sum += m_Effectors[i] * m_weightMatrix1[j * m_InputsCount + i];
                }

            }


            outputTmp[j] = sign(sum);
            /*if (sum < 0)
            {
                outputTmp[j] = -1.0;
            }
            else
            {
                outputTmp[j] = 1.0;
            }*/

        }


        if (callbackClass != NULL)
        {
            callbackClass->NetworkUpdateCallback(outputTmp);
        }

        //
        // ��������, �� ������ ��������
        //
        bool identical = true;

        for (SIZE_T i = 0, j = m_Effectors.GetSize(); i < j; i++)
        {
            if (outputTmp[i] != m_Effectors[i])
            {
                identical = false;
                break;
            }
        }

        //
        // ���� �� ��������, �� �������� ����� ���� � �������� � ����������
        //
        if (!identical)
        {

            m_Effectors.Copy(outputTmp);
        }
        else
        {
            output.Copy(outputTmp);
            break;
        }

    }

}

#endif // __HEMMING_NETWORK_CPP__
