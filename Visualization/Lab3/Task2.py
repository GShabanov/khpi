import numpy as np
import matplotlib.pyplot as plt

min_x = -2
max_x = 7

min_y = -2
max_y = 7

def fu(x, y):
    return (x + y)

def fv(x, y):
    return (2 * x)


def create_stream_line(x0, y0, u, v, t0=0, t1=10, dt=0.1):
    t = np.arange(t0, t1, dt)
    xx_new = np.zeros_like(t)
    yy_new = np.zeros_like(t)
    #xx_new[0] = x0
    #yy_new[0] = y0

    for i in range(0, t.size):
        xx_new[i] = x0 + u(x0, y0) * dt
        yy_new[i] = y0 + v(x0, y0) * dt

        x0 = xx_new[i]
        y0 = yy_new[i]

    return xx_new, yy_new


fig, (ax1, ax2) = plt.subplots(1, 2)
fig.suptitle('Варіант 9.\nВізуалізація ліній току  ' + r'$F = (x + y, 2x);  x,y\in[-2, 7]$');


n = 15
x = np.linspace(min_x, max_x, n)
y = np.linspace(min_y, max_y, n)

xx, yy = np.meshgrid(x, y)
u_val = fu(xx, yy)
v_val = fv(xx, yy)

ax1.set_title('функція streamplot')
ax1.streamplot(xx, yy, u_val, v_val)

ax2.set_xlim(min_x, max_x)
ax2.set_ylim(min_y, max_y)
ax2.set_title('функція create_stream_line')


for i in range(min_y, max_y):
    for j in range(min_x, max_x):
        x1, y1 = create_stream_line(x[j], y[i], fu, fv, min_x, max_x)
        ax2.plot(x1, y1)

#fig.set_xlim(min_x, max_x)
#fig.set_ylim(min_y, max_y)

plt.show()