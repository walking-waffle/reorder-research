/*
=======================================================
If edge (u, v) exists and edge (v, u) does not, then add edge (v, u).
This ensures all edges are bidirectional.

Both input and output are edge lists.
Input file extension: .el
Output file extension: .bel

Example:

Before (file.el):
0 1
1 2
2 1

After (file.bel):
0 1
1 0
1 2
2 1

Usage:
./a.out file.el
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

void readEdgeListFile( string & fileName, vector<Edge> & el, int & n ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    int node1, node2;

    while ( inputFile >> node1 >> node2 ) {
        n = max( n, node1 );
        n = max( n, node2 );
        el.push_back({node1, node2});
    } // for
    
    n++;
    inputFile.close();
} // readEdgeListFile

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


void el2adjList( vector<Edge> & el, int & n, vector<vector<int>> & adjList ) {
    adjList.resize(n);
    for( Edge e : el )
        adjList[e.src].push_back(e.dst);
} // el2adjList

// input vertex u and vertex v, return if the direct edge uv exist.
bool uvExist( vector<vector<int>> & adjList, int & u, int & v ) {
    for ( int j = 0; j < adjList[u].size(); j++ ) {
        if ( adjList[u][j] == v )
            return true;
    } // for

    return false;
} // uvExist

void make_bidirected( vector<vector<int>> & adjList ) {
    for ( int i = 0; i < adjList.size(); i++ ) {
        for ( int j = 0; j < adjList[i].size(); j++ ) {
            int u = i;
            int v = adjList[i][j];
            // uv exist, and check vu. 
            // if vu not exist, then add to adjList.
            if ( !uvExist( adjList, v, u ) )
               adjList[v].push_back(u);
        } // for
    } // for
} // make_bidirected

void adjList2el( vector<Edge> & el, int & n, vector<vector<int>> & adjList ) {
    el.clear();
    for ( int i = 0; i < adjList.size(); i++ ) {
        for ( int j = 0; j < adjList[i].size(); j++ ) {
            el.push_back({i, adjList[i][j]});
        } // for
    } // for
} // adjList2el

int main( int argc, char *argv[] ) {
    auto start = chrono::high_resolution_clock::now();

    if ( argc != 2 ) {
        cout << "----------Parameter Error----------\n"; 
    } else {
        string elFile(argv[1]);
        int n = 0;
        vector<Edge> el;
        vector<vector<int>> adjList; 

        readEdgeListFile( elFile, el, n );
        el2adjList( el, n, adjList );
        make_bidirected( adjList );
        adjList2el( el, n, adjList );
        writeEdgeListFile( elFile, el );
    } // else

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Total Execution Time: " << duration.count() << " seconds" << endl;

    return 0;
} // main()