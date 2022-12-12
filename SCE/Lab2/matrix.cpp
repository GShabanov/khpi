/***************************************************************************************\
*   File:                                                                               *
*       Matrix.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Matrix implementation                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Dec-2022                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#include "pch.h"

#ifndef __MATRIX_CPP__
#define __MATRIX_CPP__
#include "matrix.h"

//
// Parameter Constructor
//
template<typename Type>
CMatrix<Type>::CMatrix()
    : m_rows(0)
    , m_cols(0)
{
}


template<typename Type>
CMatrix<Type>::CMatrix(unsigned rows, unsigned cols, const Type& initialValue)
{

    mat.resize(rows);

    for (unsigned i = 0; i < mat.size(); i++) {
        mat[i].resize(cols, initialValue);
    }

    m_rows = rows;
    m_cols = cols;
}

//
// copy constructor
//
template<typename Type>
CMatrix<Type>::CMatrix(const CMatrix<Type>& rhs)
{
    mat = rhs.mat;
    m_rows = rhs.rows();
    m_cols = rhs.cols();
}

//
// destructor
//
template<typename Type>
CMatrix<Type>::~CMatrix()
{

    for (unsigned i = 0; i < mat.size(); i++) {

        mat[i].clear();
    }

    mat.clear();

}

//
// Assignment Operator
//
template<typename Type>
CMatrix<Type>& CMatrix<Type>::operator= (const CMatrix<Type>& other)
{

    if (&other == this)
        return *this;

    unsigned new_rows = other.rows();
    unsigned new_cols = other.cols();

    mat.resize(new_rows);

    for (size_t i = 0; i < mat.size(); i++)
    {
        mat[i].resize(new_cols);
    }

    for (size_t i = 0; i < new_rows; i++) {

        for (size_t j = 0; j < new_cols; j++)
        {
            mat[i][j] = other(i, j);
        }
    }

    m_rows = new_rows;
    m_cols = new_cols;

    return *this;
}

//
// Addition of two matrices
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::operator+ (const CMatrix<Type>& other)
{
    CMatrix result(m_rows, m_cols, 0.0);

    for (unsigned i = 0; i < m_rows; i++) {

        for (unsigned j = 0; j < m_cols; j++) {

            result(i, j) = this->mat[i][j] + other(i, j);
        }
    }

    return result;
}

//
// Cumulative addition of this matrix and another
//
template<typename Type>
CMatrix<Type>&
CMatrix<Type>::operator+= (const CMatrix<Type>& other)
{
    unsigned rows = other.rows();
    unsigned cols = other.cols();

    for (unsigned i = 0; i < rows; i++) {

        for (unsigned j = 0; j < cols; j++) {

            this->mat[i][j] += other(i, j);
        }
    }

    return *this;
}

//
// Subtraction of this matrix and another
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::operator- (const CMatrix<Type>& other)
{
    unsigned rows = other.rows();
    unsigned cols = other.cols();

    CMatrix result(rows, cols, 0.0);

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {

            result(i, j) = this->mat[i][j] - other(i, j);
        }
    }

    return result;
}

//
// Cumulative subtraction of this matrix and another
//
template<typename Type>
CMatrix<Type>&
CMatrix<Type>::operator-= (const CMatrix<Type>& other)
{
    unsigned rows = other.rows();
    unsigned cols = other.cols();

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            this->mat[i][j] -= other(i, j);
        }
    }

    return *this;
}

//
// Left multiplication of this matrix and another
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::operator* (const CMatrix<Type>& other)
{

    unsigned rows = other.rows();
    unsigned cols = other.cols();

    CMatrix result(rows, cols, 0.0);

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            for (unsigned k = 0; k < rows; k++)
            {

                result(i, j) += this->mat[i][k] * other(k, j);
            }
        }
    }

    return result;
}

//
// Cumulative left multiplication of this matrix and another
//
template<typename Type>
CMatrix<Type>&
CMatrix<Type>::operator*= (const CMatrix<Type>& other)
{
    CMatrix result = (*this) * other;
    (*this) = result;
    return *this;
}

//
// Calculate a transpose of this matrix
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::transpose()
{
    CMatrix result(m_rows, m_cols, 0.0);

    for (size_t i = 0; i < m_rows; i++)
    {

        for (size_t j = 0; j < m_cols; j++)
        {
            result(i, j) = this->mat[j][i];
        }
    }

    return result;
}

//
// Matrix / scalar addition
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::operator+ (const Type& other)
{

    CMatrix result(m_rows, m_cols, 0.0);

    for (size_t i = 0; i < m_rows; i++)
    {

        for (size_t j = 0; j < m_cols; j++)
        {

            result(i, j) = this->mat[i][j] + other;
        }
    }

    return result;
}

//
// Matrix / scalar subtraction
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::operator- (const Type& other)
{

    CMatrix result(m_rows, m_cols, 0.0);

    for (size_t i = 0; i < m_rows; i++)
    {
        for (size_t j = 0; j < m_cols; j++) {

            result(i, j) = this->mat[i][j] - other;
        }
    }

    return result;
}

//
// Matrix/scalar multiplication
//
template<typename Type>
CMatrix<Type> CMatrix<Type>::operator* (const Type& other)
{

    CMatrix result(m_rows, m_cols, 0.0);

    for (size_t i = 0; i < m_rows; i++)
    {

        for (size_t j = 0; j < m_cols; j++)
        {
            result(i, j) = this->mat[i][j] * other;
        }
    }

    return result;
}

//
// Matrix/scalar division
//
template<typename Type>
CMatrix<Type>
CMatrix<Type>::operator/ (const Type& other)
{
    CMatrix result(m_rows, m_cols, 0.0);

    for (size_t i = 0; i < m_rows; i++)
    {

        for (size_t j = 0; j < m_cols; j++) {

            result(i, j) = this->mat[i][j] / other;
        }
    }

    return result;
}

//
// Multiply a matrix with a vector
//
template<typename Type>
std::vector<Type>
CMatrix<Type>::operator*(const std::vector<Type>& other)
{
    std::vector<Type> result(other.size(), 0.0);

    for (size_t i = 0; i < m_rows; i++)
    {

        for (size_t j = 0; j < m_cols; j++)
        {

            result[i] = this->mat[i][j] * other[j];
        }
    }

    return result;
}

template<typename Type>
Type 
CMatrix<Type>::columnSumm(const unsigned& col) const
{
    Type output = 0;

    for (unsigned i = 0; i < m_rows; i++)
    {
        output += this->mat[i][col];
    }

    return output;
}

template<typename Type>
Type
CMatrix<Type>::columnMin(const unsigned& col) const
{
    Type  min = this->mat[0][col];

    for (unsigned i = 0; i < m_rows; i++) {

        if (this->mat[i][col] < min)
            min = this->mat[i][col];
    }

    return min;
}

template<typename Type>
Type
CMatrix<Type>::columnMax(const unsigned& col) const
{
    Type  max = this->mat[0][col];

    for (unsigned i = 0; i < m_rows; i++) {

        if (this->mat[i][col] > max)
            max = this->mat[i][col];
    }

    return max;
}

template<typename Type>
Type
CMatrix<Type>::rowMin(const unsigned& row) const
{
    Type  min = this->mat[row][0];

    for (unsigned i = 0; i < m_cols; i++) {

        if (this->mat[row][i] < min)
            min = this->mat[row][i];
    }

    return min;
}

template<typename Type>
Type
CMatrix<Type>::rowMax(const unsigned& row) const
{
    Type  max = this->mat[row][0];

    for (unsigned i = 0; i < m_cols; i++) {

        if (this->mat[row][i] > max)
            max = this->mat[row][i];
    }

    return max;
}

//
// Access the individual elements
//
template<typename Type>
Type&
CMatrix<Type>::operator()(const unsigned& row, const unsigned& col)
{
    if ((row + 1) > m_rows)
    {
        this->mat.resize(row + 1);
        m_rows++;

        for (size_t i = 0; i < mat.size(); i++) {
            this->mat[i].resize(m_cols);
        }
    }

    if ((col + 1) > m_cols)
    {
        for (size_t i = 0; i < mat.size(); i++) {
            this->mat[i].resize(col + 1);
        }

        m_cols++;
    }

    return this->mat[row][col];
}


//
// Access the individual elements (const)
//
template<typename Type>
const Type&
CMatrix<Type>::operator()(const unsigned& row, const unsigned& col) const
{

    assert(row < m_rows);
    assert(col < m_cols);

    return this->mat[row][col];
}

//
// access to rows
//
template<typename Type>
std::vector<Type> &
CMatrix<Type>::operator() (const unsigned& row)
{
    if ((row + 1) > m_rows)
    {
        this->mat.resize(row);
        m_rows++;
    }

    return this->mat[row];
}

template<typename Type>
const std::vector<Type> &
CMatrix<Type>::operator() (const unsigned& row) const
{

    assert(row < m_rows);

    return this->mat[row];
}

#endif // __MATRIX_CPP__
