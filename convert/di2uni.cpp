/*
=======================================================
只要任兩個節點有邊，將它改成兩個節點都有兩個有向邊

原本的邊列表
0 1
1 2
2 1

改動後的邊列表
0 1
1 0
1 2
2 1

./di2undi.out file.el
=======================================================
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

struct Edge {
    int src;
    int dst;
};

// 把 edgeList 寫入檔案
void writeEdgeListFile( string & fileName, vector<Edge> & el ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + ".bel" );

    for ( Edge e : el ) {
        outputFile << e.src;
        outputFile << " ";
        outputFile << e.dst;
        outputFile << " \n";
    } // for

    outputFile.close();
} // writeEdgeListFile

void readEdgeListFile( string & fileName, vector<Edge> & el, int & n ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    int node1, node2;

    // 讀取邊緣列表數據
    while ( inputFile >> node1 >> node2 ) {
        n = max( n, node1 );
        n = max( n, node2 );
        el.push_back({node1, node2});
    } // for
    
    n++;
    inputFile.close();
} // readEdgeListFile

void el2adj( vector<Edge> & el, int & n, vector<vector<int>> & adj ) {
    adj.resize(n);
    for( Edge e : el )
        adj[e.src].push_back(e.dst);
} // el2adj

// input vertex u and vertex v, return if the direct edge uv exist.
bool uvExist( vector<vector<int>> & adj, int & u, int & v ) {
    for ( int j = 0; j < adj[u].size(); j++ ) {
        if ( adj[u][j] == v )
            return true;
    } // for

    return false;
} // uvExist

void di2undi( vector<vector<int>> & adj ) {
    for ( int i = 0; i < adj.size(); i++ ) {
        for ( int j = 0; j < adj[i].size(); j++ ) {
            int u = i;
            int v = adj[i][j];
            // uv exist, and check vu. 
            // if vu not exist, then add to adj.
            if ( !uvExist( adj, v, u ) )
               adj[v].push_back(u);
        } // for
    } // for
} // el2adj

void adj2el( vector<Edge> & el, int & n, vector<vector<int>> & adj ) {
    el.clear();
    for ( int i = 0; i < adj.size(); i++ ) {
        for ( int j = 0; j < adj[i].size(); j++ ) {
            el.push_back({i, adj[i][j]});
        } // for
    } // for
} // el2adj

int main( int argc, char *argv[] ) {
    // 記錄開始時間
    auto start = chrono::high_resolution_clock::now();

    if ( argc != 2 ) {
        cout << "----------Parameter Error----------\n"; 
    } else {
        string elFile(argv[1]);
        int n = 0;
        vector<Edge> el;
        vector<vector<int>> adj; 

        readEdgeListFile( elFile, el, n );
        el2adj( el, n, adj );
        di2undi( adj );
        adj2el( el, n, adj );
        writeEdgeListFile( elFile, el );
    } // else

    // 記錄結束時間並計算執行時間
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Total Execution Time: " << duration.count() << " seconds" << endl;

    return 0;
} // main()