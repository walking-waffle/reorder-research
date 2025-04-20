/*
=======================================================
./init.out uni fileName   // for konect unigraph
./init.out bi fileName    // for konect bipartite

*****.txt->.el*****
=======================================================
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

struct Edge {
    int src;
    int dst;
};

void makeStart0 ( vector<Edge> & el ) {
    int leftMinID = el.at(0).src;
    int rightMinID = el.at(0).dst;

    for ( Edge e : el ) {
        if ( e.src < leftMinID )
            leftMinID = e.src;
        if ( e.dst < rightMinID )
            rightMinID = e.dst;
    } // for
    
    for ( Edge & e : el ) {
        e.src = e.src - leftMinID;
        e.dst = e.dst - rightMinID;
    } // for

} // makeStart0

void findNumOfNodes( vector<Edge> & el, int & leftSize, int & rightSize ) {
    leftSize = 0;
    rightSize = 0;
    for ( Edge e : el ) {
        if ( e.src > leftSize )
            leftSize = e.src;
        if ( e.dst > rightSize )
            rightSize = e.dst;
    } // for

    leftSize++;
    rightSize++;
} // findNumOfNodes

// 把 edgeList 寫入檔案
void writeEdgeListFile( string & fileName, vector<Edge> & el ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + ".el" );

    for ( Edge e : el ) {
        outputFile << e.src;
        outputFile << " ";
        outputFile << e.dst;
        outputFile << " \n";
    } // for

    outputFile.close();
} // writeEdgeListFile

void initBI( string & fileName ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    // konect 資料集中，%開頭需去除
    char c;
    c = inputFile.peek();
    while ( c == '%' ) {
        inputFile.get(c);
        while ( c != '\n' )
            inputFile.get(c);

        c = inputFile.peek();
    } // while

    int node1, node2;
    vector<Edge> el;

    // 讀取邊緣列表數據
    while ( inputFile >> node1 >> node2 )
        el.push_back({node1, node2});

    inputFile.close();

    // 讓節點 ID 從0開始
    makeStart0 ( el );

    // 計算出左右集合節點各自數量
    int leftSize = 0, rightSize = 0;
    findNumOfNodes( el, leftSize, rightSize );

    // 讓左右節點只用唯一ID
    // 把二分圖的 edgeList 轉換成不重複ID 的二分圖 edgeList
    // 假設左集合有3個節點，則右集合的節點 ID 全部+3
    for ( Edge & e : el )
        e.dst = e.dst + leftSize;
    
    // 把無向圖的 edgeList 轉換成有向圖的 edgeList( 兩倍大 )
    int m = el.size();
    for ( int i = 0; i < m; i++ )
        el.push_back({el.at(i).dst, el.at(i).src});

    // 寫檔
    writeEdgeListFile( fileName, el );
} // initBI

void initUNI( string & fileName ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    // konect 資料集中，%開頭需去除
    char c;
    c = inputFile.peek();
    while ( c == '%' ) {
        inputFile.get(c);
        while ( c != '\n' )
            inputFile.get(c);

        c = inputFile.peek();
    } // while

    int node1, node2;
    int startID;

    inputFile >> node1 >> node2;

    vector<Edge> el;
    el.push_back({node1, node2});
    startID = min( node1, node2 );

    // 讀取邊緣列表數據
    while ( inputFile >> node1 >> node2 ) {
        el.push_back({node1, node2});
        startID = min( startID, node1 );
        startID = min( startID, node2 );
    } // while

    inputFile.close();

    if ( startID != 0 ) {
        for ( Edge & e : el ) {
            e.src = e.src - startID;
            e.dst = e.dst - startID;
        } // for
    } // if

    writeEdgeListFile( fileName, el );
} // initUNI

int main( int argc, char *argv[] ) {
    if ( argc != 3 )
        cout << "----------Parameter Error----------\n"; 
    else {
        string graphType(argv[1]);
        string elFile(argv[2]);
        
        if ( graphType == "uni" ) {
            initUNI( elFile );
            cout << "unigraph init success!\n";
        } // if

        else if ( graphType == "bi" ) {
            initBI( elFile );
            cout << "bigraph init success!\n";
        } // else if

        else
            cout << "----------Please select unigraph or bipartie----------\n";

    } // else
} // main()
