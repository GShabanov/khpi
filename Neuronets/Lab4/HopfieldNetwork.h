#pragma once

#ifndef __HOPFIELD_NETWORK_H__
#define __HOPFIELD_NETWORK_H__

#include "Neuron.h"

template <class Type>
class CNetworkUpdateCallback
{
public:
    virtual void NetworkUpdateCallback(const CArray<Type>& output) = 0;
};

template <class Type>
class  CHopfieldNetwork
{

    bool        m_TrainingDataInitialized;
    bool        m_Trained;
    bool        m_Initialized;

    SIZE_T      m_InputsCount;

    CArray<Type>    m_Effectors;    // ������� ������
    CArray<Type>    m_weightMatrix; // ������� ����

    CArray<CNeuron<Type> *>    m_Neurons;

public:
    CHopfieldNetwork(size_t neuronsCount = (64 * 64));

    bool  Init();

    void  Reset();

    void Teach(const CArray<Type>& data);

    void Recovery(const CArray<Type>& input, CArray<Type>& output, CNetworkUpdateCallback<Type> *callbackClass = NULL);

    ~CHopfieldNetwork();
};



#include "HopfieldNetwork.cpp"

#endif // __HOPFIELD_NETWORK_H__