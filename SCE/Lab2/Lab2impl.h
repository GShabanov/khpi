#pragma once

#ifndef __LAB2_IMPL_H__
#define __LAB2_IMPL_H__

#include "matrix.h"
template <class T>
class  CSolver
{

private:

    CMatrix<T>    m_inputData;
    CMatrix<T>    m_SecondMatrix;
    T             m_sum_xx;
    T             m_sum_xy;
    T             m_sum_yy;
    T             m_sum_sq_x_y;
    T             m_y_hat1;
    T             m_y_hat2;
    T             m_x_hat1;
    T             m_x_hat2;

    T             m_thetta;
    T             m_thetta1;
    T             m_thetta2;

    T             m_Xmed;
    T             m_Ymed;

    T             m_b0_yx;
    T             m_b1_yx;
    T             m_b1_xy;

    T             m_Corr;

    T             m_Qx;         // # 2.26
    T             m_Qy;         // # 2.27
    T             m_Qxy;        // # 2.28

    T             m_Sx_med;     // # 2.38
    T             m_Sy_med;     // # 2.39
    T             m_Sxy_med;    // # 2.40
    T             m_Syx_med;    // # 2.41

    T             m_S_b1_med;    // # 2.42
    T             m_S_b0_med;    // # 2.43

    T             m_S_med_y_square;
    T             m_S_med_y_square_rest;


public:

    CSolver<T>()
    {
        m_sum_xx = 0.0;
        m_sum_xy = 0.0;
        m_sum_yy = 0.0;

        m_sum_sq_x_y = 0.0;
        m_y_hat1 = 0.0;
        m_y_hat2 = 0.0;
        m_x_hat1 = 0.0;
        m_x_hat2 = 0.0;

        m_thetta = 0.0;
        m_thetta1 = 0.0;
        m_thetta2 = 0.0;
        m_Xmed = 0.0;
        m_Ymed = 0.0;

        m_b0_yx = 0.0;
        m_b1_yx = 0.0;
        m_b1_xy = 0.0;

        m_Corr = 0.0;

        m_Qx = 0.0;
        m_Qy = 0.0;
        m_Qxy = 0.0;

        m_Sx_med = 0.0;     // # 2.38
        m_Sy_med = 0.0;     // # 2.39
        m_Sxy_med = 0.0;    // # 2.40
        m_Syx_med = 0.0;    // # 2.41

        m_S_b1_med = 0.0;    // # 2.42
        m_S_b0_med = 0.0;    // # 2.43

        m_S_med_y_square = 0.0;
        m_S_med_y_square_rest = 0.0;


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

    const T getX_med() const {
        return m_Xmed;
    }

    const T getY_med() const {
        return m_Ymed;
    }

    const T getSumXX() const {
        return m_sum_xx;
    }

    const T getSumYY() const {
        return m_sum_yy;
    }

    const T getSumXY() const {
        return m_sum_xy;
    }

    const T getSumSqXY() const {
        return m_sum_sq_x_y;
    }

    const T getSum6() const {
        return m_SecondMatrix.columnSumm(6);
    }
    const T getSum7() const {
        return m_SecondMatrix.columnSumm(7);
    }

    const T getY_hat_1() const {
        return m_y_hat1;
    }
    const T getY_hat_2() const {
        return m_y_hat2;
    }

    const T getX_hat_1() const {
        return m_x_hat1;
    }
    const T getX_hat_2() const {
        return m_x_hat2;
    }

    const T getThetta() const {
        return m_thetta;
    }

    const T getThetta1() const {
        return m_thetta1;
    }

    const T getThetta2() const {
        return m_thetta2;
    }

    const T getB0yx() const {
        return m_b0_yx;

    }
    const T getB1yx() const {
        return m_b1_yx;
    }

    const T getB1xy() const {
        return m_b1_xy;
    }

    const T getCorr() const {
        return m_Corr;
    }

    const T getQx() const {
        return m_Qx;
    }

    const T getQy() const {
        return m_Qy;
    }

    const T getQxy() const {
        return m_Qxy;
    }


    const T getSx_med() const {
        return m_Sx_med;
    }

    const T getSy_med() const {
        return m_Sy_med;
    }

    const T getSxy_med() const {
        return m_Sxy_med;
    }

    const T getSyx_med() const {
        return m_Syx_med;
    }

    const T getSb1_med() const {
        return m_S_b1_med;
    }

    const T getSb0_med() const {
        return m_S_b0_med;
    }

    const T getSmedianYsq() const {
        return m_S_med_y_square;
    }

    const T getSmedianYsq_rest() const {
        return m_S_med_y_square_rest;
    }


};

extern template bool CSolver<double>::Initialize();

extern template bool CSolver<double>::BuildCoefficients();



#endif // __LAB2_IMPL_H__
