# Graph Reordering - BFS Ordering
Graph reordering assigns new orders to graph vertices to improve memory locality without changing the graph structure, thereby enhancing the performance of graph applications.
This project aims to compare the effectiveness of graph reordering using BFS Ordering with other methods.

## Directory Structure

```
├─convert
│    el2csr.cpp          # Convert edge list to Compressed Sparse Row (CSR) format
│    el2matrix.py        # Represent edge list graph as a matrix
│    konectINIT.cpp      # Initialize KONECT dataset
│    make_bidirected.cpp # Convert graph to bidirected graph
├─generate 
│    gg.py               # Generate random graphs using networkx
├─reordering
│    reorder.cpp         # Graph reordering algorithm
```


## Usage
#### Step 1
Download datasets from [KONECT](http://konect.cc/networks/)
and use `convert/konectINIT.cpp` to initialize the graph, outputting graph files with `.el` extension.

#### Step 2
Place the `.el` files into the `reordering` folder and run `reorder.cpp` to obtain results after reordering.
For graph reordering methods Gorder and RCM, it is recommended to use [Gorder](https://github.com/datourat/Gorder)[1].

#### Step 3
Use the reordered graphs to run graph applications and evaluate their performance.
For graph applications, it is recommended to use [Ligra](https://github.com/jshun/ligra?tab=readme-ov-file#hypergraph-applications)[2].
If you need to convert graphs into a CSR format more suitable for graph applications, use `convert/el2csr.cpp`.
If graph applications only support undirected graphs (e.g., Maximum Independent Set), use `convert/make_bidirected.cpp` to convert the graph.

## Resources
[1] H. Wei, J. X. Yu, C. Lu, and X. Lin, [Speedup graph processing by graph ordering](https://github.com/datourat/Gorder/blob/master/paper.pdf),
in Proceedings of the 2016 International Conference on Management of Data,
pp. 1813–1828, 2016.

[2] Julian Shun and Guy E. Blelloch. [Ligra: A Lightweight Graph Processing Framework for Shared Memory](https://jshun.csail.mit.edu/ligra.pdf). Proceedings of the ACM SIGPLAN Symposium on Principles and Practice of Parallel Programming (PPoPP), pp. 135-146, 2013.
