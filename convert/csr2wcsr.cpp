#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Edge {
    int u, v, weight;
};

vector<Edge> csr2el( vector<int> OA, vector<int> EA ) {
    vector<Edge> el;
    for ( int i = 0; i < OA.size()-1; i++ ) {
        for ( int j = OA.at(i); j < OA.at(i+1); j++ )
            el.push_back( { i, EA.at(j), 0 } );
    } // for

    return el;
} // csr2el

vector<Edge> addWeight( vector<Edge> el ) {
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(1, 50);

    for ( Edge & e : el ) 
        e.weight = distr(eng);

    return el;
} // addWeight

vector<Edge> d2nd( vector<Edge> el ) {
    int m = el.size();
    for ( int i = 0; i < m; i++ )
        el.push_back( {el.at(i).v, el.at(i).u, el.at(i).weight} );
    return el;
} // d2nd

/// 將圖的edge list格式轉換為CSR格式
void el2CSR( vector<Edge> el, int numOfNodes, vector<int> & OA, vector<int> & EA, vector<int> & data ) {
    OA.clear();
    EA.clear();
    data.clear();

    OA.resize( numOfNodes + 1, 0 );
    // 計算每個節點的鄰居數量
    for ( Edge e : el )
        OA.at(e.u + 1)++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= numOfNodes; i++ )
        OA.at(i) += OA.at(i - 1);
    // ---------------------------------------- OA 完成

    EA.resize( el.size() );
    data.resize( el.size() );

    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( numOfNodes, 0 );

    for ( Edge e : el ) {
        int idx = OA.at(e.u) + nextIndex.at(e.u);
        EA.at(idx) = e.v;
        data.at(idx) = e.weight;
        nextIndex.at(e.u)++;
    } // for
    // ---------------------------------------- EA 完成

} // el2CSR

void writeFile( string fileName, vector<int> OA, vector<int> EA, vector<int> data ) {
    ofstream outputFile;

    outputFile.open(fileName.substr(0, fileName.find("C")) + "WCSR");

    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return;
    } // if

    outputFile << "WeightedAdjacencyGraph\n" << OA.size() << "\n" << EA.size() << "\n";

    for ( const int num : OA )
        outputFile << num << "\n";

    for ( const int num : EA )
        outputFile << num << "\n";

    for ( const int num : data )
        outputFile << num << "\n";

    outputFile.close();

} // writeFile

void readFile( string fileName, vector<int> & OA, vector<int> & EA ) {
    ifstream inputFile( fileName );
    if ( !inputFile.is_open() ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(EXIT_FAILURE);
    } // if

    string format = "";
    inputFile >> format;

    int offsetSize, edgeSize;
    inputFile >> offsetSize >> edgeSize;

    OA.resize(offsetSize);
    EA.resize(edgeSize);

    for ( int i = 0; i < offsetSize; i++ )
        inputFile >> OA.at(i);

    for ( int i = 0; i < edgeSize; i++ )
        inputFile >> EA.at(i);

    inputFile.close();
} // readFile

int main( int argc, char *argv[] ) {
    // 輸入為有向無加權圖，可以轉成有向or無向加權圖
    // 想要轉成有向加權圖--> ./a.out -d filename
    // 想要轉成無向加權圖--> ./a.out -nd filename
    srand(time(NULL));

    if (argc != 3) {
        cout << "-----------------paremeter error-----------------\n";
        return 1;
    } // if

    string fileName(argv[1]);
    string graphType(argv[2]);
    vector<int> OA, EA, data;

    readFile( fileName, OA, EA );

    vector<Edge> el;
    el = csr2el( OA, EA );
    el = addWeight( el );

    if ( graphType == "-nd" )
        el = d2nd( el );

    el2CSR( el, OA.size()-1, OA, EA, data );
    writeFile( fileName, OA, EA, data );

} // main()
