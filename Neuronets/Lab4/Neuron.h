#pragma once

#ifndef __NEURON_H__
#define __NEURON_H__


//
// нейрон
//
template <class DataType>
class  CNeuron
{
    DataType  m_threshold;
    DataType  m_value;


public:

    CNeuron(DataType initialValue)
    {
        m_threshold = 0.5;
        m_value = initialValue;
    }



};


#endif // __NEURON_H__