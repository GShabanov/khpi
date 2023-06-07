import numpy as np
import matplotlib.pyplot as plt
import matplotlib.path as mpath
import matplotlib.patches as mpatches
from matplotlib import animation
from matplotlib.animation import PillowWriter

fig = plt.figure()
ax = plt.axes(xlim=(-0.5, 0.5), ylim=(-0.5, 0.5))

Path = mpath.Path
path_data = [
    (Path.MOVETO, [0.018, -0.11]),
    (Path.CURVE4, [-0.031, -0.051]),
    (Path.CURVE4, [-0.115, 0.073]),
    (Path.CURVE4, [-0.03, 0.073]),
    (Path.LINETO, [-0.011, 0.039]),
    (Path.CURVE4, [0.043, 0.121]),
    (Path.CURVE4, [0.075, -0.005]),
    (Path.CURVE4, [0.035, -0.027]),
    (Path.CLOSEPOLY, [0.018, -0.11])]
codes, verts = zip(*path_data)
path = mpath.Path(verts, codes)
patch = mpatches.PathPatch(path)

def init():
    ax.add_patch(patch)
    return patch,

def animate(frame):
    
    if frame > 20:
        frame = 40 - frame


    path_data_new = [
        (Path.MOVETO, [0.018, -0.11]),
        (Path.CURVE4, [-0.031, -0.051]),
        (Path.CURVE4, [-0.115, 0.073]),
        (Path.CURVE4, [-0.03, 0.073]),
        (Path.LINETO, [-0.011, 0.039]),
        (Path.CURVE4, [0.043, 0.121]),
        (Path.CURVE4, [0.075, -0.005]),
        (Path.CURVE4, [0.035, -0.027]),
        (Path.CLOSEPOLY, [0.018, -0.11])]
    
    codes1, verts1 = zip(*path_data_new)
    
    for i in range(0, len(verts1)):


        coords = verts1[i]

        if coords[0] < 0:
            verts1[i][0] -= (frame * 0.001)
    
        elif coords[0] > 0:
            verts1[i][0] += (frame * 0.001)
    
        if coords[1] < 0:
            verts1[i][1] -= (frame * 0.001)
        elif coords[1] > 0:
            verts1[i][1] += (frame * 0.001)
    
    path_new = mpath.Path(verts1, codes1)
    patch.set_path(path_new)

    return patch,


anim = animation.FuncAnimation(fig, animate,
                               init_func=init,
                               frames=40,
                               interval=20,
                               blit=True)

writer = PillowWriter(fps=30)
anim.save("animation.gif", writer=writer)

plt.show()