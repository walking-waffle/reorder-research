/*
=======================================================
Before execution, make sure the input file has been initialized 
(using konectINIT.cpp).

This program converts an edge list (.el) into CSR format.

Usage:
./a.out file.el

Conversion:
graph.el -> graphCSR
=======================================================
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

struct Edge{
    int src;
    int dst;
};

void readEdgeList( string & fileName, vector<Edge> & el, int & n ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    int node1, node2;

    // 讀取邊緣列表數據
    while ( inputFile >> node1 >> node2 ) {
        el.push_back({node1, node2});
        n = max( n, node1 );
        n = max( n, node2 );
    } // while

    n++;
    inputFile.close();
    cout << "N: " << n << "\n";
    cout << "M: " << el.size() << "\n";
} // readEdgeList

// 將圖的edge list格式轉換為CSR格式
void el2CSR( vector<Edge> el, int n, vector<int> & OA, vector<int> & EA ) {

    OA.resize( n + 1, 0 );

    // 計算每個節點的鄰居數量
    for ( Edge e : el )
        OA.at(e.src + 1)++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= n; i++ )
        OA.at(i) += OA.at(i - 1);
    // ---------------------------------------- OA 完成
    EA.resize( el.size() );

    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( n, 0 );

    for ( Edge e : el ) {
        int node1 = e.src;
        int node2 = e.dst;
        int idx = OA.at(node1) + nextIndex.at(node1);
        EA.at(idx) = node2;
        nextIndex.at(node1)++;
    } // for
    // ---------------------------------------- EA 完成

} // el2CSR

// 把 CSR 寫入檔案
void writeCSRFile( string fileName, vector<int> OA, vector<int> EA ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + "CSR" );

    outputFile << "AdjacencyGraph\n";
    outputFile << OA.size() << "\n";
    outputFile << EA.size() << "\n";

    for ( int val : OA )
        outputFile << val << "\n";

    for ( int val : EA )
        outputFile << val << "\n";

    outputFile.close();
} // writeCSRFile


int main( int argc, char *argv[] ) {

    int n = 0;
    vector<Edge> el;
    vector<int> OA, EA;

    if ( argc != 2 )
        cout << "----------Parameter Error----------\n";
    else {
        string elFile(argv[1]);
        readEdgeList( elFile, el, n );
        el2CSR( el, n, OA, EA );
        writeCSRFile( elFile, OA, EA );
    } // else
} // main()
