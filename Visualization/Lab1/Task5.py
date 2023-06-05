import numpy as np
import matplotlib.pyplot as plt

n_groups = 10
de = (76.4, 97.6, 122.2, 130.5, 153,   176,   200.5, 227,   247,   277)
fr = (45.7, 54.7, 58.7,  62.3,  67,    72,    77,    78.5,  79,    82)
uk = (40.8, 41.8, 42,    42,    42,    46,    50.5,  54,    56.5,  59)
jp = (44,   51.6, 63.2,  71.8,  83,    93,    104,   116.8, 123.5, 127)
ussr = (123,  158,  171.5, 186.5, 205.5, 226.5, 247,   258.5, 290,   290)


fig, ax = plt.subplots()
index = np.arange(n_groups)
bar_width = 0.2
opacity = 1

br1 = np.arange(len(de))
br2 = [x + bar_width for x in br1]
br3 = [x + bar_width for x in br2]
br4 = [x + bar_width for x in br3]
br5 = [x + bar_width for x in br4]


colors = ['red', 'gray', '#00008b', '#CCCC00', '#CC0000']
rects1 = ax.bar(br1, de, bar_width,
                alpha=opacity, color=colors[0],
                label='Німеччина')

rects2 = ax.bar(br2, fr, bar_width,
                alpha=opacity, color=colors[1],
                label='Франція')

rects3 = ax.bar(br3, uk, bar_width,
                alpha=opacity, color=colors[2],
                label='Великобританія')

rects4 = ax.bar(br4, jp, bar_width,
                alpha=opacity, color=colors[3],
                label='Японія')

rects5 = ax.bar(br5, ussr, bar_width,
                alpha=opacity, color=colors[4],
                label='СРСР')


ax.grid(True, linestyle ='-.')
ax.set_xlabel('Роки')
ax.set_ylabel('Млн. чол')
ax.set_title('Чисельність населення, млн. чол.')
ax.set_xticks(index + 1.5*bar_width)
ax.set_xticklabels(('1900', '1913', '1929', '1938', '1950', '1960', '1970', '1980', '1990', '2000'))
ax.legend(loc='upper left')

plt.show()
