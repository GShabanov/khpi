import numpy as np
import matplotlib.pyplot as plt

# параболічний циліндр

phi = 10

x = np.linspace(-10, 10, 100)
y = np.linspace(0, 10, 100)
x, y = np.meshgrid(x, y)
#u = np.sqrt(2 * phi * x) 
u = x ** 2 / (2 * phi)


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x, y, u)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.title(r'$y^2 = 2*\phi * y$')
plt.show()

# об'єднати x, y, u в один масив
combined = np.array([x.flatten(), y.flatten(), u.flatten()])

# зберегти combined в txt формат
np.savetxt('Lab1_Task4.txt', combined.T)