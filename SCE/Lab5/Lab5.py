import numpy as np
import math


def main():

    #
    # Лабораторна робота № 5
    # Визначити граничне значення величини, що змінюється по експоненті на основі вимірювань сигналу
    #  y(t) = y0 + b0*(1-e^(-b1*t))  на виході динамічної системи
    #

   
    # 1. З тактом   сформувати необхідну кількість вимірювань  
    # 2. Сформувати необхідні суми та різниці вимірювань t0 = 0
    # 3. Записати необхідне рівняння відносно Y. Розв’язати це рівняння, знайти Y.
    # 4. Обчислити рівноважне положення R.
    # 5. Зробити висновок.

    y0 = 7.44
    b0 = 4.0
    b1 = 0.99
    dt = 1.32
    
    print(f"y0 = {y0}, b0 = {b0}, b1 = {b1}, Δt = {dt}\n")

    R = y0 + b0
    A = -b0
    h = b1

    print(f"R = {R}, A = {A}, h = {b1}\n")

    table = []
    t = 0.0


    for i in range(0, 13):
        
        table.append( (t ,  (R + A * math.exp(-h*t))) )
        t = t + dt


    print("Data table")

    print(table)

    a0 = table[0][1]
    a1 = table[1][1]
    a2 = table[2][1]
    a3 = table[3][1]
    a4 = table[4][1]

    sigm00 = 2 * a2
    sigm10 = a3 + a1
    etta00  = a3 - a1
    etta10  = a4 - a0

    print(f"Ϭ00 = {sigm00}")
    print(f"Ϭ10 = {sigm10}")
    print(f"η00 = {etta00}")
    print(f"η10 = {etta10}")

    y = etta10 / (2 * etta00)
    R_hat = (sigm10 - sigm00*y)/(2*(1-y))

    print(f"y = {y}")
    print(f"R = {R_hat}")

    t = 0.0
    l = 0

    for i in range(0, 100000):
        
        if ((R + A * math.exp(-h*t))) == 11.440:
            print(f't = {t}')
            l = i
            break

        t = t + dt

    

# Run main code
main()
