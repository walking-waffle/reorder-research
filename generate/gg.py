import networkx as nx
import matplotlib.pyplot as plt

# https://networkx.org/documentation/stable/reference/generated/networkx.generators.random_graphs.watts_strogatz_graph.html
#g = nx.watts_strogatz_graph(n, k, p, seed=None)

# https://networkx.org/documentation/stable/reference/generated/networkx.generators.random_graphs.powerlaw_cluster_graph.html
#g = nx.powerlaw_cluster_graph(n, m, p, seed=None)

# https://networkx.org/documentation/stable/reference/generated/networkx.generators.random_graphs.barabasi_albert_graph.html#
#g = nx.barabasi_albert_graph(n, m, seed=None, initial_graph=None)

print("is_connected_graph: " + str(nx.is_connected(g)))

# default is undirected, so turn it into directed
g = g.to_directed()

# output
nx.write_edgelist(g, "input_your_graph_name.el", data=False)
print("V: " + str(g.number_of_nodes()))
print("E: " + str(g.number_of_edges()))
print("finish")

# show graph (optional)
#nx.draw(g)
#plt.show()