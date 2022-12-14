/***************************************************************************************\
*   File:                                                                               *
*       Matrix.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       Matrix implementation                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#pragma once

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>

template <typename Type = double>
class CMatrix
{

private:
    std::vector<std::vector<Type> > mat;

    unsigned m_rows;
    unsigned m_cols;

public:
    CMatrix();

    CMatrix(unsigned rows, unsigned cols, const Type& initialValue);
    CMatrix(const CMatrix<Type>& other);

    virtual ~CMatrix();

    //
    // Operators
    //
    CMatrix<Type>& operator=(const CMatrix<Type>& other);

    //
    // Matrix math operations
    //
    CMatrix<Type> operator+(const CMatrix<Type>& other);
    CMatrix<Type>& operator+=(const CMatrix<Type>& other);
    CMatrix<Type> operator-(const CMatrix<Type>& other);
    CMatrix<Type>& operator-=(const CMatrix<Type>& other);
    CMatrix<Type> operator*(const CMatrix<Type>& other);
    CMatrix<Type>& operator*=(const CMatrix<Type>& other);
    CMatrix<Type> transpose();
    CMatrix<Type> cofactor();
    CMatrix<Type> inverse();
    Type determinant();


    //
    // Matrix/scalar operations
    //
    CMatrix<Type> operator+(const Type& other);
    CMatrix<Type> operator-(const Type& other);
    CMatrix<Type> operator*(const Type& other);
    CMatrix<Type> operator/(const Type& other);

    //
    // Matrix/vector operations
    //
    std::vector<Type> operator*(const std::vector<Type>& other);

    //
    // Access the individual elements
    //
    Type& operator()(const unsigned& row, const unsigned& col);
    const Type& operator()(const unsigned& row, const unsigned& col) const;

    //
    // access to rows
    //
    std::vector<Type>& operator()(const unsigned& row);
    const std::vector<Type>& operator()(const unsigned& row) const;


    Type columnSumm(const unsigned& col) const;
    Type columnMin(const unsigned& col) const;
    Type columnMax(const unsigned& col) const;
    Type rowMin(const unsigned& col) const;
    Type rowMax(const unsigned& col) const;


    //
    // Access the row and column sizes
    //
    unsigned rows() const {
        return this->m_rows;
    }

    unsigned cols() const {
        return this->m_cols;
    }


};

#include "matrix.cpp"

#endif // __MATRIX_H__