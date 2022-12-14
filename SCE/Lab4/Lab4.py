import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math


def main():

    # Модель динамічної системи має вигляд y(t) = a1*e^(b1*t)+a2*e^(b2*t)   (1).
    #
    # 1) За даними, наведеними в таблиці А,  отримати значення функції відгуку,
    # виконавши (аналітично за формулою (1))  5 експериментів, починаючи з t0=0 з кроком h .
    # Результати записати в таблицю 1, утримуючи 3 знаки після коми.
    #
    #

    a1 = 2.5
    b1 = -0.21
    a2 = -0.5
    b2 = -0.14
    h = 2.12

    n_tests = 5

    t_k = 0
    table_1 = []

    for x in range(1, (n_tests + 1)):
        line = [x, t_k, (a1 * math.pow(math.e, (b1*t_k)) + a2*math.pow(math.e, (b2*t_k)))]
        t_k += h
        table_1.append(line)

    df1 = pd.DataFrame(table_1)
    y = 'y_k'
    print(f'table 1')
    df1.rename(columns= { 0: "n", 1: "t_k", 2: "y_k"}, inplace=True)

    # 2) Маючи значення функції відгуку (Таблиця 1),
    # побудувати математичну модель функції відгуку у вигляді
    # h(t) = c1*e^(λ1*t)+c2*e^(λ2*t) . Для цього застосувати відповідний алгоритм апроксимації.

    print(df1)
    # а) Записати вигляд системи рівнянь для визначення z1, z2.
    #                                      z1,                                                                                                       z2
    A = np.array([[(df1[y][0] * df1[y][0] + df1[y][1] * df1[y][1] + df1[y][2] * df1[y][2]),   (df1[y][0] * df1[y][1] + df1[y][1] * df1[y][2] + df1[y][2] * df1[y][3])],
                  [(df1[y][0] * df1[y][1] + df1[y][1] * df1[y][2] + df1[y][2] * df1[y][3]),   (df1[y][1] * df1[y][1] + df1[y][2] * df1[y][2] + df1[y][3] * df1[y][3])]])

    B = np.array([(-(df1[y][0] * df1[y][2]) - (df1[y][1] * df1[y][3]) - (df1[y][2] * df1[y][4])),
                  (-(df1[y][1] * df1[y][2]) - (df1[y][2] * df1[y][3]) - (df1[y][3] * df1[y][4]))])

    X = np.linalg.inv(A).dot(B)
    print(f'A = {A}')
    print(f'B = {B}')
    z1 = np.round(X[0], 3)
    z2 = np.round(X[1], 3)
    print(f'z1 = {z1}, z2 = {z2}')

    # б) Записати вигляд рівняння для визначення x1 , x2 .
    print(f'x^2 + {z2} * x + {z1} ')
    coeff = [1, z2, z1]
    x1x2 = np.roots(coeff)
    x1 = np.round(x1x2[0], 3)
    x2 = np.round(x1x2[1], 3)
    print(f'x1 = {x1}, x2 = {x2}')

    # в) Записати вигляд системи рівнянь для визначення λ1,λ2, c1, c2.
    λ1 = np.round(((1/h)*(math.log(x1))), 3)
    λ2 = np.round(((1/h)*(math.log(x2))), 3)
    print(f'λ1 = {λ1}, λ2 = {λ2}')

    C1C2 = np.array([[(1 + x1**2 + x1**4)      , (1 + x1*x2 + x1**2*x2**2)],
                     [(1 + x1*x2 + x1**2*x2**2), (1 + x2**2 + x2**4)]])

    C0 = np.array([(df1[y][0] + df1[y][1]*x1 + df1[y][2]*x1**2),
                   (df1[y][0] + df1[y][1]*x2 + df1[y][2]*x2**2)])

    c1c2_solved = np.linalg.inv(C1C2).dot(C0)
    c1 = np.round(c1c2_solved[0], 3)
    c2 = np.round(c1c2_solved[1], 3)
    print(f'c1 = {c1}, c2 = {c2}')

    # е) Записати вигляд моделі.
    e = math.e
    print(f'h(t) = {c1}*e²{λ1}t + {c2}*e²{λ2}t')

    # Перевірити значимість рівняння моделі в цілому, використовуючи F-критерій Фішера.
    # Результати обчислень записати в таблицю 2. Зробити висновок.

    t_k = 0
    h_k = []
    for x in range(1, (n_tests+1)):
        h_k.append(np.round((c1*e**(λ1*t_k) + c2*e**(λ2*t_k)), 3))
        t_k += h

    df2 = df1
    df2 = df2.merge(pd.Series(h_k, name='h_k'), left_index=True, right_index=True)

    df2['yk - hk'] = df2[y] - df2['h_k']
    df2['(yk - hk)²'] = np.power((df2[y] - df2['h_k']), 2)
    summ = np.round(df2['(yk - hk)²'].sum(), 5)

    print(df2['(yk - hk)²'])


    print(f'sum([yk - hk]²) = {summ}')



# Run main code
main()
