import numpy as np
import matplotlib.pyplot as plt

r = 2

fi =  np.linspace(0, 2 * np.pi, 1000)

# Кардіодіда
ro = 2 * r * (1 - np.cos(fi))




ax = plt.subplot(111, projection='polar')
ax.plot(fi, ro, color='b', linewidth = 2)
plt.title(r'$\rho = 2 * r * (1 - \cos{\phi})$')

ax.set_rmin(0)
ax.set_rmax(5)

plt.show()

theta2 = np.degrees(fi)
data = np.column_stack((theta2, ro))
np.savetxt("Lab1_Task3.txt", data)

