#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
#include <climits>
#include <unordered_set>

using namespace std;

struct Node {
    int id;
    int numOfDeg;
};

struct Edge {
    Node src;
    Node dst;
};

void readEdgeList( string & fileName, vector<Edge> & el, int & n ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    Node node1, node2;

    // 讀取邊緣列表數據
    while ( inputFile >> node1.id >> node2.id ) {
        el.push_back({node1, node2});
        n = max( n, node1.id );
        n = max( n, node2.id );
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
        OA.at(e.src.id + 1)++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= n; i++ )
        OA.at(i) += OA.at(i - 1);
    // ---------------------------------------- OA 完成
    EA.resize( el.size() );

    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( n, 0 );

    for ( Edge e : el ) {
        int node1 = e.src.id;
        int node2 = e.dst.id;
        int idx = OA.at(node1) + nextIndex.at(node1);
        EA.at(idx) = node2;
        nextIndex.at(node1)++;
    } // for
    // ---------------------------------------- EA 完成

} // el2CSR

// 將圖的edge list格式轉換為CSC格式
void el2CSC( vector<Edge> el, int n, vector<int> & OA, vector<int> & EA ) {

    OA.resize( n + 1, 0 );

    // 計算每個節點的鄰居數量
    for ( Edge e : el )
        OA.at(e.dst.id + 1)++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= n; i++ )
        OA.at(i) += OA.at(i - 1);
    // ---------------------------------------- OA 完成
    EA.resize( el.size() );

    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( n, 0 );

    for ( Edge e : el ) {
        int node1 = e.dst.id;
        int node2 = e.src.id;
        int idx = OA.at(node1) + nextIndex.at(node1);
        EA.at(idx) = node2;
        nextIndex.at(node1)++;
    } // for
    // ---------------------------------------- EA 完成

} // el2CSC

void sortEdgeList( vector<Edge> & el ) {
    sort(el.begin(), el.end(), [](const Edge & a, const Edge & b)->bool{
		if ( a.src.id < b.src.id )
			return true;
		else if ( a.src.id > b.src.id )
			return false;
		else {
			if( a.dst.id <= b.dst.id )
				return true;
			else
				return false;
		} // else
	});
} // sortEdgeList

vector<int> randomOrder( vector<Edge> & el, int n ) {
    vector <int> randomList(n);
    for ( int i = 0; i < n; i++ )
        randomList.at(i) = i;

    unsigned seed = 0;
    shuffle( randomList.begin(), randomList.end(), default_random_engine(seed));

    return randomList;
} // randomOrder

// 輸入 CSR 或 CSC 圖的 offset array，輸出最大 out/in-degree 的 id
int findMaxDegreeID( vector<int> OA ) {
    int max = 0, numNeighbor = 0, id = -1;

    for ( int i = 1; i < OA.size(); i++ ) {
        numNeighbor = OA.at(i) - OA.at(i-1);
        if ( max < numNeighbor ) {
            max = numNeighbor;
            id = i-1;
        } // if
    } // for

    cout << "maximum degree id: " << id << endl;
    cout << "maximum degree: " << max << endl;
    return id;
} // findMaxDegreeID

// 廣度優先搜索 (BFS)
vector<int> bfs( vector<int> & OA, vector<int> & EA, int & begin, bool* & visited ) {
    int numOfNodes = OA.size() - 1;
    vector<int> result;

    // 起始節點先 push 進 queue
    queue<int> q;
    q.push(begin);
    visited[begin] = true;

    while ( !q.empty() ) {
        int current = q.front();
        q.pop();

        // 找到目標
        result.push_back(current);

        // 將當前節點的鄰居節點加入 queue
        for ( int i = OA[current]; i < OA[current + 1]; i++ ) {
            int neighbor = EA[i];

            // 如果鄰居節點尚未被訪問過
            if ( !visited[neighbor] ) {
                q.push(neighbor);
                visited[neighbor] = true;
            } // if
        } // for
    } // while

    cout << "maximum components:" << result.size() << "\n";
    return result;
} // bfs

// 深度優先搜索 (DFS)
vector<int> dfs( vector<int> OA, vector<int> EA, int begin ) {
    int n = OA.size() - 1;
    vector<bool> visited( n, false );
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

vector<int> degSort( vector<Edge> & el, int & n ) {
    vector<int> OA, EA;
    el2CSR(el, n, OA, EA);

    // node info, include id and numOfDeg
    vector<Node> nodeList(n);
    
    // complete nodeList
    for ( int i = 0; i < OA.size()-1; i++ ) {
        nodeList[i].id = i;
        nodeList[i].numOfDeg = OA[i+1]-OA[i];
    } // for

    sort(nodeList.begin(), nodeList.end(), [](const Node& a, const Node& b) {
        return a.numOfDeg > b.numOfDeg;
    });
    
    vector<int> invertedList(n);
    for( int i = 0; i < n; i++ )
        invertedList[nodeList[i].id]=i;

    return invertedList;
} // degSort

vector<int> hubSort( vector<Edge> & el, int & n ) {
    int avgDeg = el.size()/n;

    vector<int> OA, EA;
    el2CSR(el, n, OA, EA);

    // node info, include id and numOfDeg
    vector<Node> nodeList(n);
    
    // complete nodeList
    for ( int i = 0; i < OA.size()-1; i++ ) {
        nodeList[i].id = i;
        nodeList[i].numOfDeg = OA[i+1]-OA[i];
    } // for

    vector<Node> hot, cold;
    for ( Node v : nodeList ) {
        if ( v.numOfDeg > avgDeg )
            hot.push_back( v );
        else
            cold.push_back( v );
    } // for

    sort(hot.begin(), hot.end(), [](const Node& a, const Node& b) {
        return a.numOfDeg > b.numOfDeg;
    });

    hot.insert( hot.end(), cold.begin(), cold.end() );
    
    vector<int> invertedList(n);
    for( int i = 0; i < n; i++ )
        invertedList[hot[i].id]=i;

    return invertedList;
} // hubSort

vector<int> hubCluster( vector<Edge> & el, int & n ) {
    int avgDeg = el.size()/n;

    vector<int> OA, EA;
    el2CSR(el, n, OA, EA);

    // node info, include id and numOfDeg
    vector<Node> nodeList(n);
    
    // complete nodeList
    for ( int i = 0; i < OA.size()-1; i++ ) {
        nodeList[i].id = i;
        nodeList[i].numOfDeg = OA[i+1]-OA[i];
    } // for

    vector<Node> hot, cold;
    for ( Node v : nodeList ) {
        if ( v.numOfDeg > avgDeg )
            hot.push_back( v );
        else
            cold.push_back( v );
    } // for

    hot.insert( hot.end(), cold.begin(), cold.end() );
    
    vector<int> invertedList(n);
    for( int i = 0; i < n; i++ )
        invertedList[hot[i].id]=i;

    return invertedList;
} // HubCluster

vector<int> dbg( vector<Edge> & el, int & n ) {
    int avgDeg = el.size()/n;

    vector<int> OA, EA;
    el2CSR(el, n, OA, EA);

    // node info, include id and numOfDeg
    vector<Node> nodeList(n);
    
    // complete nodeList
    for ( int i = 0; i < OA.size()-1; i++ ) {
        nodeList[i].id = i;
        nodeList[i].numOfDeg = OA[i+1]-OA[i];
    } // for

    int threshold[8] = {avgDeg/2, avgDeg, avgDeg*2, avgDeg*4, avgDeg*8, avgDeg*16, avgDeg*32, INT_MAX};
    vector<vector<int>> groups(8);

    // Classify nodes into 8 positions of groups according to degree
    for ( int i = 0 ; i < n ; i++ ) {
        for ( int j = 0 ; j < 8 ; j++ ) {
            if ( nodeList[i].numOfDeg <= threshold[j] ) {
                groups[j].push_back(i);
                break;
            } // if
        } // for
    } // for

    int k = 0;
    vector<int> result(n);
    for ( int i = 7; i >= 0; i-- ) {
        for ( int j = 0; j < groups[i].size(); j++ ) {
            result[k] = groups[i][j];
            k++;
        } // for
    } // for

    vector<int> invertedList(n);
    for( int i = 0; i < n; i++ )
        invertedList[result[i]]=i;

    return invertedList;
} // dbg

vector<int> bfsOrder( vector<Edge> & el, int & n ) {
    vector<int> OA, EA;
    el2CSR( el, n, OA, EA );

	random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, n);
    int begin = distrib(gen);
    
    bool* visited=new bool[n];
	memset(visited, 0, sizeof(bool)*n);
    vector<int> bfsResult = bfs( OA, EA, begin, visited );

    if ( bfsResult.size() != n ) {
        for ( int i = 0; i < n; i++ ) {
            if ( visited[i] == false )
                bfsResult.push_back(i);
        } // for
    } // if

    vector<int> invertedList(n);
	for( int i = 0; i < n; i++ )
		invertedList[bfsResult[i]]=i;

    return invertedList;
} // bfsOrder

vector<int> rbfsOrder( vector<Edge> & el, int & n ) {
    vector<int> OA, EA;
    el2CSR( el, n, OA, EA );

	random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, n);
    int begin = distrib(gen);

	bool* visited=new bool[n];
	memset(visited, 0, sizeof(bool)*n);
    vector<int> bfsResult = bfs( OA, EA, begin, visited );

    if ( bfsResult.size() != n ) {
        for ( int i = 0; i < n; i++ ) {
            if ( visited[i] == false )
                bfsResult.push_back(i);
        } // for
    } // if

    vector<int> invertedList(n);
	for( int i = 0; i < n; i++ )
		invertedList[bfsResult[i]]=n-1-i;

    return invertedList;
} // rbfsOrder

// 把 reordering 後的 edgeList 寫入檔案
void writeEdgeListFile( string fileName, vector<Edge> el, vector<int> & newOrder, string oper ) {
    for ( Edge & e : el ) {
        e.src.id = newOrder.at(e.src.id);
        e.dst.id = newOrder.at(e.dst.id);
    } // for

    //sortEdgeList( el );
    ofstream outputFile(fileName.substr(0, fileName.find(".")) + oper + ".el");
    for ( Edge e : el ) {
        outputFile << e.src.id;
        outputFile << " ";
        outputFile << e.dst.id;
        outputFile << " \n";
    } // for

    outputFile.close();
} // writeEdgeListFile

void recordReorder( vector<string> reorderNameList, vector<double> timeLogList ) {
    ofstream outputFile( "log.txt" );
    outputFile << "                Reorder " << "|" << "         Time\n";
    outputFile << "---------------------------------------\n";

    for ( int i = 0; i < timeLogList.size(); i++ ) {
        outputFile << setw(25) << reorderNameList.at(i) << setw(10) << timeLogList.at(i) << " ms\n";
        outputFile << "---------------------------------------\n";
    } // for

    outputFile.close();
} // recordReorder

void processReorder( string & fileName ) {
    clock_t start, end;
    // store reorder time cost
    vector<double> timeLogList;
    // store reorder name
    vector<string> reorderNameList;

    // number of vertices
    int n = 0;
    // edgeList before and after reorder
    vector<Edge> el;
    vector<int> newOrder;
    readEdgeList( fileName, el, n );
    cout << "Read edgeList file finish.\n";
/*
    start = clock();
    newOrder = randomOrder( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "DBG |" );
    writeEdgeListFile( fileName, el, newOrder, "_RO" );
*/
    start = clock();
    newOrder = degSort( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Deg Sort |" );
    writeEdgeListFile( fileName, el, newOrder, "_D1" );

    start = clock();
    newOrder = hubSort( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Hub Sort |" );
    writeEdgeListFile( fileName, el, newOrder, "_D2" );

    start = clock();
    newOrder = hubCluster( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Hub Cluster |" );
    writeEdgeListFile( fileName, el, newOrder, "_D3" );

    start = clock();
    newOrder = dbg( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "DBG |" );
    writeEdgeListFile( fileName, el, newOrder, "_D4" );

    start = clock();
    newOrder = bfsOrder( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "bfs Order |" );
    writeEdgeListFile( fileName, el, newOrder, "_BO" );

    start = clock();
    newOrder = rbfsOrder( el, n );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "rbfs Order |" );
    writeEdgeListFile( fileName, el, newOrder, "_RBO" );

    recordReorder( reorderNameList, timeLogList );
} // processReorder

int main( int argc, char *argv[] ) {
    if ( argc != 2 )
        cout << "----------Parameter Error----------\n";
    else {
        string elFile( argv[1] );
        processReorder( elFile );
    } // else
} // main()
