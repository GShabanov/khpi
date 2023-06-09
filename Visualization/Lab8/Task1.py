import networkx as nx
import matplotlib.pyplot as plt

plt.figure()

graph = nx.read_gexf('sp_data_school_day_2_g.gexf')

nx.draw_networkx(graph, width = 0.1,
                 node_size = 100,
                 node_color = 'lightblue',
                 edge_color = 'grey',
                 alpha = 0.5, 
                 font_size = 5, 
                 font_color = 'k')
plt.show()
