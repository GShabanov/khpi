import numpy as np
import matplotlib.pyplot as plt



#
# Варіант 9.
#
def UFunction(x, y):
    return x**2 * y / 4 - np.sqrt(x**2 + 5 * y**2)

n = 256
x = np.linspace(-2, 7, n)  # діапазон по X
y = np.linspace(-2, 7, n)  # діапазон по Y

#
# лінійна сітка для скалярного поля
#
gridx1, gridy1 = np.meshgrid(x, y)

fig, axs = plt.subplots(2, 2, layout='constrained')
fig.suptitle('Варіант 9\nВізуалізацію функції  ' +  r'$\frac{1}{4}x^{2}y - \sqrt{x^{2} + 5y^{2}}$')


axs[0, 0].pcolormesh(gridx1, gridy1, UFunction(gridx1, gridy1))
axs[0, 0].set_title('Cкалярне поле')
axs[0, 0].set_xlabel('X')
axs[0, 0].set_ylabel('Y')

#
# лінійна сітка для векторного поля
#
gridx2, gridy2 = np.meshgrid(np.linspace(-2, 7, 18), np.linspace(-2, 7, 18))
uf = UFunction(gridx2, gridy2)
u_dx, u_dy = np.gradient(uf)

#
# векторне поле
#
axs[1, 0].quiver(gridx2, gridy2, u_dx, u_dy)
axs[1, 0].set_title('Векторне поле')
axs[1, 0].set_xlabel('X')
axs[1, 0].set_ylabel('Y')

axs[0, 1].streamplot(gridx2, gridy2, u_dx, u_dy, color = uf, cmap='viridis')
axs[0, 1].set_title('Градієнтне поле')
axs[0, 1].set_xlabel('X')
axs[0, 1].set_ylabel('Y')

axs[1, 1].remove()

plt.show()