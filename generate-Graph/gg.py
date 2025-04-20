import sys
import networkx as nx
import matplotlib.pyplot as plt

# 確保至少有兩個參數（a 和 b）
if len(sys.argv) != 3:
    print("Usage: python3 gg.py a b")
    sys.exit(1)

# 讀取命令行參數
try:
    a = int(sys.argv[1])
    b = int(sys.argv[2])
except ValueError:
    print("Error: Both a and b should be integers.")
    sys.exit(1)

# 建立 Barabási-Albert 圖
baGraph = nx.barabasi_albert_graph(a, b, seed=None, initial_graph=None)

print("isConnected: " + str(nx.is_connected(baGraph)))
print("isDirected: " + str(nx.is_directed(baGraph)))

# 轉換為有向圖並寫入邊列表
g = baGraph.to_directed()
nx.write_edgelist(g, "ba" + str(b) + ".txt", data=False)
print("finish")

# 顯示圖形（可選）
#nx.draw(baGraph)
#plt.show()
