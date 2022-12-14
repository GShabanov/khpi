#pragma once

#ifndef __LAB3_IMPL_H__
#define __LAB3_IMPL_H__

#include "matrix.h"
template <class T>
class  CSolver
{

private:

    CMatrix<T>    m_inputData;
    CMatrix<T>    m_SecondMatrix;
    CMatrix<T>    m_ThirdMatrix;
    CMatrix<T>    m_ThirdMatrix2;
    CMatrix<T>    m_AMatrix;
    CMatrix<T>    m_BMatrix;
    CMatrix<T>    m_XMatrix;

    CMatrix<T>    m_AMatrix2;
    CMatrix<T>    m_BMatrix2;
    CMatrix<T>    m_XMatrix2;


public:

    CSolver<T>()
        : m_inputData()
        , m_SecondMatrix()
        , m_ThirdMatrix()
        , m_ThirdMatrix2()
        , m_AMatrix()
        , m_BMatrix()
        , m_XMatrix()
        , m_AMatrix2()
        , m_BMatrix2()
        , m_XMatrix2()
    {
    }

    bool   Initialize();
    bool   BuildCoefficients();

    const CMatrix<T>& getInput() const
    {
        return m_inputData;
    }

    const CMatrix<T>& getCoefficientsMatrix() const
    {
        return m_SecondMatrix;
    }


    const CMatrix<T>& getCheckMatrix() const
    {
        return m_ThirdMatrix;
    }

    const CMatrix<T>& getAMatrix() const
    {
        return m_AMatrix;
    }

    const CMatrix<T>& getBMatrix() const
    {
        return m_BMatrix;
    }

    const CMatrix<T>& getXMatrix() const
    {
        return m_XMatrix;
    }

    const CMatrix<T>& getCheckMatrix2() const
    {
        return m_ThirdMatrix2;
    }


    const CMatrix<T>& getAMatrix2() const
    {
        return m_AMatrix2;
    }

    const CMatrix<T>& getBMatrix2() const
    {
        return m_BMatrix2;
    }

    const CMatrix<T>& getXMatrix2() const
    {
        return m_XMatrix2;
    }

};

extern template bool CSolver<double>::Initialize();

extern template bool CSolver<double>::BuildCoefficients();



#endif // __LAB3_IMPL_H__
