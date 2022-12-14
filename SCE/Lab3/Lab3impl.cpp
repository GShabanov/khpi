#include "pch.h"

#include "Lab3impl.h"

bool
CSolver<double>::Initialize()
{
    typedef double T;

    static double inputValues[10][2] = {
        { 0.40, 4.95 },
        { 1.00, 7.15 },
        { 1.60, 8.16 },
        { 2.20, 8.24 },
        { 2.80, 7.65 },
        { 3.40, 6.65 },
        { 4.00, 5.50 },
        { 4.60, 4.46 },
        { 5.20, 3.78 },
        { 5.80, 3.73 }
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

    for (unsigned i = 0; i < m_inputData.rows(); i++)
    {
        //
        // X
        //
        m_SecondMatrix(i, 0) = m_inputData(i, 0);

        //
        // Y
        //
        m_SecondMatrix(i, 1) = m_inputData(i, 1);

        //
        // X * Y
        //
        m_SecondMatrix(i, 2) = m_inputData(i, 0) * m_inputData(i, 1);
        //
        // X * X
        //
        m_SecondMatrix(i, 3) = m_inputData(i, 0) * m_inputData(i, 0);

        //
        // X * X * Y
        //
        m_SecondMatrix(i, 4) = m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 1);

        //
        // X * X * X
        //
        m_SecondMatrix(i, 5) = m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0);

        //
        // X * X * X * X
        //
        m_SecondMatrix(i, 6) = m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0);

        //
        // X^5
        //
        m_SecondMatrix(i, 7) = m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0);
        //
        // X^6
        //
        m_SecondMatrix(i, 8) = m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0);
        //
        // X^3 * Y
        //
        m_SecondMatrix(i, 9) = m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 0) * m_inputData(i, 1);
    }

    //    [df[x].sum(), df['xx'].sum(), df['xxx'].sum()] ,
    //    [df['xx'].sum(), df['xxx'].sum(), df['xxxx'].sum()]])
    // 
    //    B = np.array([df[y].sum(), df['xy'].sum(), df['xxy'].sum()])
    //    X = np.linalg.inv(A).dot(B)


    //
    // col 1
    //
    m_AMatrix(0, 0) = m_SecondMatrix.rows();                // n
    m_AMatrix(0, 1) = m_SecondMatrix.columnSumm(0);         // Σ (X)
    m_AMatrix(0, 2) = m_SecondMatrix.columnSumm(3);         // Σ (X * X)


    //
    // col 2
    //
    m_AMatrix(1, 0) = m_SecondMatrix.columnSumm(0);         // Σ (X)
    m_AMatrix(1, 1) = m_SecondMatrix.columnSumm(3);         // Σ (X * X)
    m_AMatrix(1, 2) = m_SecondMatrix.columnSumm(5);         // Σ (X * X * X)

    //
    // col 3
    //
    m_AMatrix(2, 0) = m_SecondMatrix.columnSumm(3);         // Σ (X * X)
    m_AMatrix(2, 1) = m_SecondMatrix.columnSumm(5);         // Σ (X * X * X)
    m_AMatrix(2, 2) = m_SecondMatrix.columnSumm(6);         // Σ (X * X * X * X)



    //
    // B matrix
    //
    m_BMatrix(0, 0) = m_SecondMatrix.columnSumm(1);         // Σ (Y)
    m_BMatrix(1, 0) = m_SecondMatrix.columnSumm(2);         // Σ (X * Y)
    m_BMatrix(2, 0) = m_SecondMatrix.columnSumm(4);         // Σ (X * X * Y)

    CMatrix<double>  inversedA = m_AMatrix.inverse();

    m_XMatrix = inversedA * m_BMatrix;

    double A = m_XMatrix(0, 0);
    double B = m_XMatrix(1, 0);
    double C = m_XMatrix(2, 0);

    for (unsigned i = 0; i < m_SecondMatrix.rows(); i++)
    {
        //
        // X
        //
        m_ThirdMatrix(i, 0) = m_SecondMatrix(i, 0);
        //
        // Y
        //
        m_ThirdMatrix(i, 1) = m_SecondMatrix(i, 1);

        //
        // X^2
        //
        m_ThirdMatrix(i, 2) = m_SecondMatrix(i, 3);

        //
        // ŷ
        //
        m_ThirdMatrix(i, 3) = m_SecondMatrix(i, 3) * C + m_SecondMatrix(i, 0) * B + A;       ////////  

        //
        // | y - ŷ |
        //
        m_ThirdMatrix(i, 4) = abs(m_SecondMatrix(i, 1) - m_ThirdMatrix(i, 3));

        //
        // (y - ŷ)^2
        //
        m_ThirdMatrix(i, 5) = (m_SecondMatrix(i, 1) - m_ThirdMatrix(i, 3)) * (m_SecondMatrix(i, 1) - m_ThirdMatrix(i, 3));

        //
        // y-ȳ
        //
        m_ThirdMatrix(i, 6) = (m_SecondMatrix(i, 1) - (m_SecondMatrix.columnSumm(1) / m_SecondMatrix.rows()));

        //
        // (y-ȳ)^2
        //
        m_ThirdMatrix(i, 7) = m_ThirdMatrix(i, 6) * m_ThirdMatrix(i, 6);
    }


    //
    //  CUBIC
    // 
    
    //
    // col 1
    //
    m_AMatrix2(0, 0) = m_SecondMatrix.rows();                // n
    m_AMatrix2(0, 1) = m_SecondMatrix.columnSumm(0);         // Σ (X)
    m_AMatrix2(0, 2) = m_SecondMatrix.columnSumm(3);         // Σ (X * X)
    m_AMatrix2(0, 3) = m_SecondMatrix.columnSumm(5);         // Σ (X * X * X)


    //
    // col 2
    //
    m_AMatrix2(1, 0) = m_SecondMatrix.columnSumm(0);         // Σ (X)
    m_AMatrix2(1, 1) = m_SecondMatrix.columnSumm(3);         // Σ (X * X)
    m_AMatrix2(1, 2) = m_SecondMatrix.columnSumm(5);         // Σ (X * X * X)
    m_AMatrix2(1, 3) = m_SecondMatrix.columnSumm(6);         // Σ (X * X * X * X)

    //
    // col 3
    //
    m_AMatrix2(2, 0) = m_SecondMatrix.columnSumm(3);         // Σ (X * X)
    m_AMatrix2(2, 1) = m_SecondMatrix.columnSumm(5);         // Σ (X * X * X)
    m_AMatrix2(2, 2) = m_SecondMatrix.columnSumm(6);         // Σ (X * X * X * X)
    m_AMatrix2(2, 3) = m_SecondMatrix.columnSumm(7);         // Σ (X * X * X * X * X)

    //
    // col 4
    //
    m_AMatrix2(3, 0) = m_SecondMatrix.columnSumm(5);         // Σ (X * X * X)
    m_AMatrix2(3, 1) = m_SecondMatrix.columnSumm(6);         // Σ (X * X * X * X)
    m_AMatrix2(3, 2) = m_SecondMatrix.columnSumm(7);         // Σ (X * X * X * X * X)
    m_AMatrix2(3, 3) = m_SecondMatrix.columnSumm(8);         // Σ (X * X * X * X * X * X)


    //
    // B matrix
    //
    m_BMatrix2(0, 0) = m_SecondMatrix.columnSumm(1);         // Σ (Y)
    m_BMatrix2(1, 0) = m_SecondMatrix.columnSumm(2);         // Σ (X * Y)
    m_BMatrix2(2, 0) = m_SecondMatrix.columnSumm(4);         // Σ (X * X * Y)
    m_BMatrix2(3, 0) = m_SecondMatrix.columnSumm(9);         // Σ (X * X * X * * Y)


    CMatrix<double>  inversedA_2 = m_AMatrix2.inverse();

    m_XMatrix2 = inversedA_2 * m_BMatrix2;

    double A2 = m_XMatrix2(0, 0);
    double B2 = m_XMatrix2(1, 0);
    double C2 = m_XMatrix2(2, 0);
    double D2 = m_XMatrix2(3, 0);


    for (unsigned i = 0; i < m_SecondMatrix.rows(); i++)
    {
        //
        // X
        //
        m_ThirdMatrix2(i, 0) = m_SecondMatrix(i, 0);
        //
        // Y
        //
        m_ThirdMatrix2(i, 1) = m_SecondMatrix(i, 1);

        //
        // X^2
        //
        m_ThirdMatrix2(i, 2) = m_SecondMatrix(i, 3);

        //
        // ŷ
        //
        m_ThirdMatrix2(i, 3) = m_SecondMatrix(i, 5) * D2 + m_SecondMatrix(i, 3) * C2 + m_SecondMatrix(i, 0) * B2 + A2;       ////////  

        //
        // | y - ŷ |
        //
        m_ThirdMatrix2(i, 4) = abs(m_SecondMatrix(i, 1) - m_ThirdMatrix2(i, 3));

        //
        // (y - ŷ)^2
        //
        m_ThirdMatrix2(i, 5) = (m_SecondMatrix(i, 1) - m_ThirdMatrix2(i, 3)) * (m_SecondMatrix(i, 1) - m_ThirdMatrix2(i, 3));

        //
        // y-ȳ
        //
        m_ThirdMatrix2(i, 6) = (m_SecondMatrix(i, 1) - (m_SecondMatrix.columnSumm(1) / m_SecondMatrix.rows()));

        //
        // (y-ȳ)^2
        //
        m_ThirdMatrix2(i, 7) = m_ThirdMatrix2(i, 6) * m_ThirdMatrix2(i, 6);
    }

    /*for (unsigned i = 0; i < m_XMatrix.rows(); i++)
    {

        for (unsigned j = 0; j < m_XMatrix.cols(); j++)
        {

            CString num;
            num.Format(_T(" %4.4f "), m_XMatrix.operator()(i, j));

            OutputDebugString(num);

        }

        OutputDebugString(L"\n");
    }*/


        //df[x] = df_old[x]
        //df['xx'] = df_old['xx']
        //df[y] = df_old[y]

        //df['ŷ'] = (df['xx'].mul(c) + df[x].mul(b) + a)
        //df['|y -ŷ|'] = np.abs(df[y] - df['ŷ'])
        //df['(y -ŷ)^2'] = np.power((df[y] - df['ŷ']), 2)
        //df['y-ȳ'] = df[y] - df[y].mean()
        //df['(y-ȳ)^2'] = np.power((df[y] - df['y-ȳ']), 2)

        //s_mid2_rest = (df['(y -ŷ)^2'].sum()) / (n - 2)
        //s_mid2_y = (df['(y-ȳ)^2'].sum()) / (n - 1)
        //F = s_mid2_y / s_mid2_rest
        //print(f'S̄^2_зал = {s_mid2_rest}, S̄^2_y = {s_mid2_y}, F = {F}')
        //print(f' F = {F} >> F(9,8,1%)=5,9106')
        //# коефіцієнт кореляції

        //R = math.sqrt(1 - (df['(y -ŷ)^2'].sum()) / (df['(y-ȳ)^2'].sum()))
        //print(f'R = {R}')
        //print(f' R > r(крит, 2, 1%) => {round(R, 6)} > 0.9800')

        //t_roof = (R * math.sqrt(n - 2)) / (math.sqrt(1 - R * R))
        //print(f't^ = {round(t_roof, 4)} >> t(8, 0.05%) = 5,0413')


    return true;
}

