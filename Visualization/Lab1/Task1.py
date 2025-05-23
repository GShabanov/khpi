import numpy as np
import matplotlib.pyplot as plt

def Fz(x):

    z = np.zeros_like(x)

    for i, value in enumerate(x):
        if value <= 0:
            z[i] = np.cbrt(6 + value ** 2)
        else:
            z[i] = np.sin(np.pi * value) ** 3 + (2 + value) / (1 + np.cos(value) ** 2)

    return z

def Fy(x):

    divident = 1 + x
    divisor = 5 + np.sqrt(np.abs(x) * np.exp(-x) + np.abs(np.cos(np.pi * x)))

    return divident / divisor



x = np.arange(0, 2, 0.1)
y = Fy(x)
z = Fz(x)

plt.figure(1)
plt.subplot(211) 
plt.title('Варіант 9')

plt.plot(x, y)
plt.grid(True, linestyle='-.')
plt.xlabel('X')
plt.ylabel('Y')
plt.legend([r'$y = \frac{1 + x} { 5 + \sqrt{\left | x \right | * e^{-x} + \left | \cos(\pi * x) \right | } } $'])

plt.subplot(212)
plt.plot(x, z)
plt.grid(True, linestyle='-.')
plt.xlabel('X')
plt.ylabel('Z')
plt.legend([r'$\sqrt[3]{6 + x^2}, x \leq 0, \sin^3(\pi * x) + \frac{2 + x}{1 + \cos^2(x)} x < 0$'])

data = np.column_stack((x, y))
np.savetxt('Lab1_task1_1.txt', data, delimiter='\t', header='X\tY')

data = np.column_stack((x, z))
np.savetxt('Lab1_task1_2.txt', data, delimiter='\t', header='X\tZ')

plt.show()
