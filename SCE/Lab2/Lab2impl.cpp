#include "pch.h"

#include "Lab2impl.h"

bool
CSolver<double>::Initialize()
{
    typedef double T;

    static double inputValues[10][2] = {
        { 0.12,  -3.9 },
        { 2.74,   7.0 },
        { 5.34,  17.4 },
        { 7.95,  28.0 },
        { 10.55, 38.8 },
        { 13.18, 49.0 },
        { 15.75, 59.1 },
        { 18.40, 70.1 },
        { 21.02, 80.4 },
        { 23.60, 91.0 }

    };

    m_inputData = CMatrix<T>();

    for (unsigned i = 0; i < ARRAYSIZE(inputValues); i++)
    {
        for (unsigned j = 0; j < ARRAYSIZE(inputValues[0]); j++)
        {
            m_inputData(i, j) = (T)inputValues[i][j];
        }
    }

    return true;
}

bool
CSolver<double>::BuildCoefficients()
{
    typedef double T;

    //=======================================================================================
    //   inputData  matrix
    //      -----------------------------------
    //      | index  0      |   index  1      |
    //      -----------------------------------
    //      |     X         |      Y          |
    //      -----------------------------------
    //
    //    SecondMatrix  matrix
    //      -------------------------------------------------------------------------------
    //      |   index  0  |   index  1    |   index  2   |   index  3   |   index  4      |
    //      -------------------------------------------------------------------------------
    //      |     X^2     |      Y^2      |     X * Y    |     X + Y    |   (X + Y)^2     |
    //      -------------------------------------------------------------------------------

    m_SecondMatrix = CMatrix<T>();

    for (unsigned i = 0; i < m_inputData.rows(); i++)
    {
        //
        // X * X
        //
        m_SecondMatrix(i, 0) = m_inputData(i, 0) * m_inputData(i, 0);
        //
        // Y * Y
        //
        m_SecondMatrix(i, 1) = m_inputData(i, 1) * m_inputData(i, 1);

        //
        // X * Y
        //
        m_SecondMatrix(i, 2) = m_inputData(i, 0) * m_inputData(i, 1);
        //
        // X + Y
        //
        m_SecondMatrix(i, 3) = m_inputData(i, 0) + m_inputData(i, 1);

        //
        // (X + Y) ^ 2
        //
        m_SecondMatrix(i, 4) = m_SecondMatrix(i, 3) * m_SecondMatrix(i, 3);
    }


    //
    // X*X  column summ
    //
    m_sum_xx = m_SecondMatrix.columnSumm(0);

    //
    // X*Y  column summ
    //
    m_sum_xy = m_SecondMatrix.columnSumm(2);

    //
    // Y*Y  column summ
    //
    m_sum_yy = m_SecondMatrix.columnSumm(1);

    //
    // (X + Y) ^ 2  column summ
    //

    m_sum_sq_x_y = m_SecondMatrix.columnSumm(4);

    unsigned n = m_SecondMatrix.rows();

    T  sum_x = m_inputData.columnSumm(0);
    T  sum_y = m_inputData.columnSumm(1);

    //
    //  Sum(Xi) * Sum(Yi)
    //

    T  sum_x_sum_y = sum_x * sum_y;
    T  sum_x_sum_x = sum_x * sum_x;
    T  sum_y_sum_y = sum_y * sum_y;


    // direct
    m_b0_yx = (sum_y * m_sum_xx - m_sum_xy * sum_x) / (n * m_sum_xx - sum_x_sum_x);  // 2.11
    m_b1_yx = (n * m_sum_xy - sum_x_sum_y) / (n * m_sum_xx - sum_x_sum_x);  // 2.12

    m_y_hat1 = m_b0_yx;
    m_y_hat2 = m_b1_yx;
    // y ^ = b0_yx + b1_yx * x

    // indirect
    // x ^ = b0_xy + b1_xy * x
    T b0_xy = (sum_x * m_sum_yy - m_sum_xy * sum_y) / (n * m_sum_yy - sum_y_sum_y);  // 2.15
    m_b1_xy = (n * m_sum_xy - sum_x * sum_y) / (n * m_sum_yy - sum_y_sum_y);  // 2.16

    m_x_hat1 = b0_xy;
    m_x_hat2 = m_b1_xy;

    m_thetta = n * m_sum_xx - sum_x_sum_x;  // 2.8
    m_thetta1 = sum_y * m_sum_xx - m_sum_xy * sum_x;  // 2.9
    m_thetta2 = n * m_sum_xy - sum_x * sum_y;  // 2.10
    T thetta_shtrich = n * m_sum_yy - sum_y_sum_y;  // 2.17
    T thetta_shtrich_1 = sum_x * m_sum_yy - sum_y * m_sum_xy;  // 2.18

    m_Xmed = sum_x / n;
    m_Ymed = sum_y / n;

    m_Corr = (n * m_sum_xy - sum_x * sum_y) / (sqrt((n * m_sum_xx - sum_x_sum_x) * (n * m_sum_yy - sum_y_sum_y)));


    m_Qx = m_thetta / n;          // # 2.26
    m_Qy = thetta_shtrich / n;    // # 2.27
    m_Qxy = m_thetta2 / n;        // # 2.28

    m_Sx_med = sqrt(m_Qx / (n - 1));   // # 2.38
    m_Sy_med = sqrt(m_Qy / (n - 1));   //  # 2.39
    m_Sxy_med = m_Qxy / (n - 1);       //  # 2.40
    m_Syx_med = sqrt(m_Qxy / (n - 2)); //  # 2.41
    m_S_b1_med = m_Syx_med / sqrt(m_Qx); //  # 2.42

    //T s_mid_b0_yx = s_mid_yx * sqrt(1 / n + ((sum_x / n) * (sum_x / n) /*df[x].mean() * df[x].mean()*/) / Qx); //  # 2.43

    m_S_b0_med = m_Syx_med * sqrt((T)1.0 / n + ((sum_x / n) * (sum_x / n)) / m_Qx); //  # 2.43


    for (unsigned i = 0; i < m_inputData.rows(); i++)
    {
        //
        // y^ = b0_yx + b1_yx * x
        //
        m_SecondMatrix(i, 5) = m_inputData(i, 0) * m_b1_yx + m_b0_yx;

        //
        // y-y^ = y - y^
        //

        m_SecondMatrix(i, 6) = m_inputData(i, 1) - m_SecondMatrix(i, 5);

        //
        // (y-y^)^2 = (y-y^)*(y-y^)
        //
        m_SecondMatrix(i, 7) = m_SecondMatrix(i, 6) * m_SecondMatrix(i, 6);
        
    }

    T sum_square_y_y_roof = m_SecondMatrix.columnSumm(7);

    m_S_med_y_square = m_Qy / (n - 1);
    m_S_med_y_square_rest = sum_square_y_y_roof / (n - 2);   // # 2.48


    return true;
}

