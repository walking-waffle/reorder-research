import networkx as nx
import matplotlib.pyplot as plt

# input your file
f = "file.el"

# read edge list
g = nx.read_edgelist(f, create_using=nx.Graph(), nodetype=int)

# 確保節點順序固定（按節點 ID 排序）
sorted_nodes = sorted(g.nodes())
adj_matrix = nx.adjacency_matrix(g, nodelist=sorted_nodes)

# 使用 spy 來可視化稀疏矩陣結構
plt.figure(figsize=(10, 10))
plt.spy(adj_matrix, markersize=0.1)  # 調小 markersize 以顯示更多元素
plt.title("Matrix Structure of Graph")
plt.show()

