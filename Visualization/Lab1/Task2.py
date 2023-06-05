import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import numpy as np

fig = plt.figure()
ax = fig.add_subplot(projection='3d')
X = np.arange(0, 8, 0.25)
Y = np.arange(0, 8, 0.25)
X, Y = np.meshgrid(X, Y)

U = np.sin(X + 1) ** 2 * np.cos(Y) - 10 * Y ** (0.5 * X) * np.exp(-X)

surf = ax.plot_surface(X, Y, U, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('U')
ax.set_title(r'$\sin^2(x-1)*\cos(y) - 10 * y^{0.5 * x} * e^{-x}$')

ax.zaxis.set_major_locator(LinearLocator(10))
fig.colorbar(surf, shrink = 0.5, aspect = 5)

plt.show()

np.savetxt('Lab1_Task2.txt', np.c_[X.ravel(), Y.ravel(), U.ravel()], header='X Y Z')
