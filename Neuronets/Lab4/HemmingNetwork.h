#pragma once

#ifndef __HEMMING_NETWORK_H__
#define __HEMMING_NETWORK_H__

#include "Neuron.h"


template <class Type>
class  CHemmingNetwork
{

    bool        m_TrainingDataInitialized;
    bool        m_Trained;
    bool        m_Initialized;

    SIZE_T      m_InputsCount;

    CArray<Type>    m_Effectors;    // вх≥дний вектор
    CArray<Type>    m_weightMatrix1; // матриц€ ваг≥в першого р≥вн€
    CArray<Type>    m_weightMatrix2; // матриц€ ваг≥в другого р≥вн€

    CArray<CNeuron<Type> *>    m_Neurons;

    static Type sign(Type a)
    {
        if (a < 0)
        {
            return -1.0;
        }
        else
        {
            return 1.0;
        }
    }

public:
    CHemmingNetwork(size_t neuronsCount = (64 * 64));

    bool  Init();

    void  Reset();

    void Teach(const CArray<Type>& data);

    void Recovery(const CArray<Type>& input, CArray<Type>& output, CNetworkUpdateCallback<Type> *callbackClass = NULL);

    ~CHemmingNetwork();
};



#include "HemmingNetwork.cpp"

#endif // __HEMMING_NETWORK_H__