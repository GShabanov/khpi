from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
import numpy as np


x, y, z = np.meshgrid(np.arange(-2, 7, 0.9),
                      np.arange(-2, 7, 0.9),
                      np.arange(-2, 7, 0.9))

Fx = np.divide(x, (x**2 + y**2 + z**2))
Fy = np.divide(y, (x**2 + y**2 + z**2))
Fz = np.divide(z, (x**2 + y**2 + z**2))



fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.quiver(x, y, z, Fx, Fy, Fz, length=1, normalize=True, cmap='viridis')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title(r'$F = \left (\frac{x}{(x^{2} + y^{2} + z^{2})}; \frac{y}{(x^{2} + y^{2} + z^{2})}; \frac{z}{(x^{2} + y^{2} + z^{2})}\right )$')

plt.show()