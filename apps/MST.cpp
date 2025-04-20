#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <climits>
#include <tuple>

using namespace std;

class MinHeapNode {
public:
    int v;
    int key;
    MinHeapNode(int vertex, int k) : v(vertex), key(k) {}
};

class MinHeap {
private:
    void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);

public:
    int size;
    int capacity;
    vector<int> pos;
    vector<MinHeapNode*> array;
    MinHeap(int capacity);
    void minHeapify(int idx);
    bool isEmpty() const;
    MinHeapNode* extractMin();
    void decreaseKey(int v, int key);
    bool isInMinHeap(int v) const;
};

MinHeap::MinHeap(int capacity) : size(0), capacity(capacity), pos(capacity), array(capacity) {}

void MinHeap::swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void MinHeap::minHeapify(int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < size && array[left]->key < array[smallest]->key)
        smallest = left;

    if (right < size && array[right]->key < array[smallest]->key)
        smallest = right;

    if (smallest != idx) {
        MinHeapNode* smallestNode = array[smallest];
        MinHeapNode* idxNode = array[idx];

        pos[smallestNode->v] = idx;
        pos[idxNode->v] = smallest;

        swapMinHeapNode(&array[smallest], &array[idx]);
        minHeapify(smallest);
    }
}

bool MinHeap::isEmpty() const {
    return size == 0;
}

MinHeapNode* MinHeap::extractMin() {
    if (isEmpty())
        return nullptr;

    MinHeapNode* root = array[0];
    MinHeapNode* lastNode = array[size - 1];
    array[0] = lastNode;

    pos[root->v] = size - 1;
    pos[lastNode->v] = 0;

    --size;
    minHeapify(0);

    return root;
}

void MinHeap::decreaseKey(int v, int key) {
    int i = pos[v];
    array[i]->key = key;

    while (i && array[i]->key < array[(i - 1) / 2]->key) {
        pos[array[i]->v] = (i - 1) / 2;
        pos[array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&array[i], &array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

bool MinHeap::isInMinHeap(int v) const {
    return pos[v] < size;
}

vector<int> prim(const vector<int>& OA, const vector<int>& EA, const vector<int>& data, int begin) {
    int V = OA.size() - 1;
    vector<int> parent(V, -1);
    vector<int> key(V, INT_MAX);
    MinHeap minHeap(V);

    for (int v = 0; v < V; ++v) {
        if ( v == begin )
            continue;
        minHeap.array[v] = new MinHeapNode(v, key[v]);
        minHeap.pos[v] = v;
    } // for

    key[begin] = 0;
    minHeap.array[begin] = new MinHeapNode(begin, key[begin]);
    minHeap.pos[begin] = 0;

    minHeap.size = V;

    while (!minHeap.isEmpty()) {
        MinHeapNode* minHeapNode = minHeap.extractMin();
        int u = minHeapNode->v;

        for (int i = OA[u]; i < OA[u + 1]; ++i) {
            int v = EA[i];
            int weight = data[i]; // Using weight from the data vector

            if (minHeap.isInMinHeap(v) && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                minHeap.decreaseKey(v, key[v]);
            } // if
        } // for
    } // while

    return key;
}

struct Edge {
    int u, v, weight;
    bool operator<(const Edge &other) const {
        return weight < other.weight;
    }
};

// 輸入 CSR 或 CSC 圖的 offset array，輸出最大 out/in-degree 的 id
int findMaxDegreeID( vector<int> OA ) {
    int max = 0, id = -1;

    for ( int i = 1; i < OA.size(); i++ ) {
        int numNeighbor = OA.at(i) - OA.at(i-1);
        if ( max < numNeighbor ) {
            max = numNeighbor;
            id = i-1;
        } // if
    } // for

    cout << "maximum degree id: " << id << endl;
    cout << "maximum degree: " << max << endl;
    return id;
} // findMaxOutDegreeID

int findSet(int u, vector<int> &parent) {
    if (u != parent[u])
        parent[u] = findSet(parent[u], parent);

    return parent[u];
}

void unite(int u, int v, vector<int> &parent, vector<int> &rank) {
    int root_u = findSet(u, parent);
    int root_v = findSet(v, parent);
    if (root_u != root_v) {
        if (rank[root_u] > rank[root_v]) {
            parent[root_v] = root_u;
        } else if (rank[root_u] < rank[root_v]) {
            parent[root_u] = root_v;
        } else {
            parent[root_v] = root_u;
            rank[root_u]++;
        }
    }
}

vector<Edge> getEdges( vector<int> & OA, vector<int> & EA, vector<int> & data ) {
    vector<Edge> edges;
    int n = OA.size() - 1; // number of vertices
    for ( int u = 0; u < n; u++ ) {
        for ( int i = OA[u]; i < OA[u + 1]; i++ ) {
            int v = EA[i];
            int weight = data[i];
            edges.push_back({u, v, weight});
        } // for
    } // for

    return edges;
} // getEdges

vector<Edge> kruskal( vector<int> & OA, vector<int> & EA, vector<int> & data) {
    vector<Edge> edges = getEdges( OA, EA, data );
    sort(edges.begin(), edges.end());

    int n = OA.size() - 1;
    vector<int> parent(n), rank(n, 0);
    for (int i = 0; i < n; i++) 
        parent[i] = i;

    vector<Edge> mst;
    for (const auto & edge : edges) {
        if ( findSet(edge.u, parent) != findSet(edge.v, parent) ) {
            unite( edge.u, edge.v, parent, rank );
            mst.push_back(edge);
        } // if
    } // for

    return mst;
} // kruskal

// 廣度優先搜索 (BFS)
vector <int> bfs( vector<int> OA, vector<int> EA, int begin ) {
    int numOfNodes = OA.size() - 1;
    vector <int> result;

    // 記錄節點是否被訪問過
    vector<bool> visited( numOfNodes, false );

    // 起始節點先 push 進 queue
    queue<int> q;
    q.push(begin);
    visited.at(begin) = true;

    while ( !q.empty() ) {
        int current = q.front();
        q.pop();

        // 找到目標
        result.push_back(current);

        // 將當前節點的鄰居節點加入 queue
        for ( int i = OA.at(current); i < OA.at(current + 1); i++ ) {
            int neighbor = EA.at(i);

            // 如果鄰居節點尚未被訪問過
            if ( !visited.at(neighbor) ) {
                q.push(neighbor);
                visited.at(neighbor) = true;
            } // if
        } // for
    } // while

    return result;
} // bfs

// 深度優先搜索 (DFS)
vector<int> dfs( vector<int> OA, vector<int> EA, int begin ) {
    int numOfNodes = OA.size() - 1;
    vector<bool> visited( numOfNodes, false );
    vector <int> result;;

    stack<int> s;
    s.push(begin);
    visited.at(begin) = true;

    while ( !s.empty() ) {
        int current = s.top();
        s.pop();

        // 找到目標
        result.push_back(current);

        // 將當前節點的鄰接節點加入堆疊（反向加入）
        for ( int i = OA.at(current + 1) - 1; i >= OA.at(current); i-- ) {
            int neighbor = EA.at(i);
            if ( !visited.at(neighbor) ) {
                s.push(neighbor);
                visited.at(neighbor) = true;
            } // if
        } // for
    } // while

    return result;
} // dfs

int minKey(vector<int>& key, vector<bool>& mstSet) {
    int min = INT_MAX, min_index = -1;
    int V = mstSet.size();

    for (int v = 0; v < V; v++) {
        if (!mstSet.at(v) && key.at(v) < min) {
            min = key.at(v);
            min_index = v;
        }
    }

    return min_index;
}

// 讀 CSR 
void readCSRFile( string fileName, vector<int> & OA, vector<int> & EA, vector<int> & data ) {
    ifstream inputFile( fileName );
    if (!inputFile) {
        cerr << "Cannot open file: " << fileName << endl;
        exit(1);
    } // if

    string graphType = "";
    int OAsize = 0, EAsize = 0;

    inputFile >> graphType;
    cout << graphType << endl;
    if ( graphType != "AdjacencyGraph" && graphType != "WeightedAdjacencyGraph" ) {
        cout << "file error";
        exit(0);
    } // if

    inputFile >> OAsize;
    inputFile >> EAsize;

    int val = 0;
    for ( ; OAsize > 0; OAsize-- ) {
        inputFile >> val;
        OA.push_back(val);
    } // for

    for ( ; EAsize > 0; EAsize-- ) {
        inputFile >> val;
        EA.push_back(val);
    } // for

    if ( graphType == "WeightedAdjacencyGraph" ) {    
        while ( inputFile >> val )
            data.push_back(val);

    } // if

    inputFile.close();
} // readCSRFile

void processApp( string fileName, string s ) {
    time_t start, end;

    vector<int> OA, EA, data;
    readCSRFile( fileName, OA, EA, data );

    int begin = findMaxDegreeID( OA );
    begin = stoi(s);
/*
    start = clock();
    vector<int> bfsResult = bfs( OA, EA, begin );
    cout << "BFS Size: " << bfsResult.size() << endl;
    end = clock();
    cout << "BFS Running Time: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << endl;

    start = clock();
    vector<int> dfsResult = dfs( OA, EA, begin );
    end = clock();
    cout << "DFS Running Time: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << endl;
*/
    start = clock();
    vector<Edge> mstKruskal = kruskal( OA, EA, data );
    end = clock();
    cout << "Kruskal Running Time: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << endl;
    int ans = 0;
    for (const auto &edge : mstKruskal)
        ans = ans + edge.weight;
    cout << "Kruskal: " << ans << endl;

    vector<int> ansK;
    start = clock();
    ansK = prim( OA, EA, data, begin);
    end = clock();
    cout << "Prim Running Time: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << endl;
    ans = 0;
    for ( int k : ansK )
        ans = ans + k;
    cout << "Prim: " << ans << endl;

} // processApp

int main( int argc, char *argv[] ) {
    if ( argc != 3 )
        cout << "----------Parameter Error----------\n";
    else {
        string argv1(argv[1]); // filename
        string argv2(argv[2]); // begin vertex
        processApp( argv1, argv2 );
    } // else
} // main()
