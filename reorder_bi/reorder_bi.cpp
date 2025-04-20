#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <numeric>
#include <iomanip>
#include <ctime>

using namespace std;

enum SetType { leftSide, rightSide };

struct Edge {
    int src;
    int dst;
};

void convertToCSR( vector<Edge> &el, int n, vector<int> &OA, vector<int> &EA) {
    OA.resize(n + 1, 0);
    for (Edge &e : el)
        OA[e.src + 1]++;

    for (int i = 1; i <= n; ++i)
        OA[i] += OA[i - 1];

    EA.resize(el.size());
    vector<int> currentIndex(n, 0);
    for (const Edge &e : el) {
        int pos = OA[e.src] + currentIndex[e.src]++;
        EA[pos] = e.dst;
    } 
}

vector<int> findIndexI(vector<int> originList) {
    vector<int> invertedList(originList.size(), 0);
    for (int i = 0; i < originList.size(); i++)
        invertedList.at(originList.at(i)) = i;

    return invertedList;
}

int findMaxDegreeIndex(vector<int> OA) {
    int max = 0, numNeighbor = 0, index = -1;

    for (int i = 1; i < OA.size(); i++) {
        numNeighbor = OA.at(i) - OA.at(i - 1);
        if (max < numNeighbor) {
            max = numNeighbor;
            index = i - 1;
        }
    }

    return index;
}

vector<int> bfs(vector<int> OA, vector<int> EA, int begin) {
    int n = OA.size() - 1;
    vector<int> result;

    vector<bool> visited(n, false);

    queue<int> q;
    q.push(begin);
    visited.at(begin) = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        result.push_back(current);

        for (int i = OA.at(current); i < OA.at(current + 1); i++) {
            int neighbor = EA.at(i);

            if (!visited.at(neighbor)) {
                q.push(neighbor);
                visited.at(neighbor) = true;
            }
        }
    }

    return result;
}

vector<Edge> mix64k(vector<Edge> el, int leftSize, int rightSize) {
    vector<int> newOrderL, newOrderR;

    bool turnL = (leftSize <= rightSize);
    bool turnR = !turnL;

    for (int newID = 0; newID < leftSize + rightSize; newID++) {
        if (newID % 64000 == 0) {
            turnL = !turnL;
            turnR = !turnR;
        }

        if (turnL)
            newOrderL.push_back(newID);
        else
            newOrderR.push_back(newID);
    }

    for (Edge &e : el) {
        if (e.src < leftSize)
            e.src = newOrderL.at(e.src);
        else
            e.src = newOrderR.at(e.src - leftSize);

        if (e.dst < leftSize)
            e.dst = newOrderL.at(e.dst);
        else
            e.dst = newOrderR.at(e.dst - leftSize);
    }

    return el;
}

vector<Edge> BFSOrder(vector<Edge> el, int leftSize, int rightSize) {
    vector<int> OA, EA;

    convertToCSR(el, leftSize + rightSize, OA, EA);

    vector<int> bfsResult = bfs(OA, EA, findMaxDegreeIndex(OA));

    if (bfsResult.size() != (leftSize + rightSize)) {
        vector<bool> visited(leftSize + rightSize, false);
        for (int n : bfsResult)
            visited.at(n) = true;

        for (int i = 0; i < visited.size(); i++) {
            if (visited.at(i) == false)
                bfsResult.push_back(i);
        }
    }

    vector<int> invertedList = findIndexI(bfsResult);

    for (Edge &e : el) {
        e.src = invertedList.at(e.src);
        e.dst = invertedList.at(e.dst);
    }

    return el;
}

vector<Edge> BFSOrderS(vector<Edge> el, int leftSize, int rightSize) {
    vector<int> OA, EA;

    convertToCSR(el, leftSize + rightSize, OA, EA);

    vector<int> result;
    vector<int> notInResult;

    vector<bool> visited(OA.size() - 1, false);

    queue<int> q;
    int begin = findMaxDegreeIndex(OA);
    q.push(begin);
    visited.at(begin) = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        result.push_back(current);

        int hit = 0;
        for (int i = OA.at(current); i < OA.at(current + 1); i++) {
            int neighbor = EA.at(i);
            if (!visited.at(neighbor)) {
                if (hit % 5 == 0)
                    q.push(neighbor);
                else
                    notInResult.push_back(neighbor);

                visited.at(neighbor) = true;
            }

            hit++;
        }
    }

    result.insert(result.end(), notInResult.begin(), notInResult.end());

    if (result.size() != (leftSize + rightSize)) {
        vector<bool> visited(leftSize + rightSize, false);
        for (int n : result)
            visited.at(n) = true;

        for (int i = 0; i < visited.size(); i++) {
            if (visited.at(i) == false)
                result.push_back(i);
        }
    }

    vector<int> invertedList = findIndexI(result);

    for (Edge &e : el) {
        e.src = invertedList.at(e.src);
        e.dst = invertedList.at(e.dst);
    }

    return el;
}

vector<Edge> BFSOrderD(vector<Edge> el, int leftSize, int rightSize) {
    vector<int> OA, EA;

    convertToCSR(el, leftSize + rightSize, OA, EA);

    vector<int> bfsResult = bfs(OA, EA, findMaxDegreeIndex(OA));

    if (bfsResult.size() != (leftSize + rightSize)) {
        vector<bool> visited(leftSize + rightSize, false);
        for (int n : bfsResult)
            visited.at(n) = true;

        for (int i = 0; i < visited.size(); i++) {
            if (visited.at(i) == false)
                bfsResult.push_back(i);
        }
    }

    vector<int> l, r;
    for (int i : bfsResult) {
        if (i < leftSize)
            l.push_back(i);
        else
            r.push_back(i);
    }

    l.insert(l.end(), r.begin(), r.end());

    vector<int> invertedList = findIndexI(l);

    for (Edge &e : el) {
        e.src = invertedList.at(e.src);
        e.dst = invertedList.at(e.dst);
    }

    return el;
}

vector<Edge> BFSOrderRS(vector<Edge> el, int leftSize, int rightSize) {
    vector<int> OA, EA;

    convertToCSR(el, leftSize + rightSize, OA, EA);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, leftSize + rightSize);
    int start = dis(gen);

    vector<int> bfsResult = bfs(OA, EA, start);
    cout << "=========================" << endl;
    cout << "start ID: " << start << endl;
    cout << "start degree: " << OA.at(start + 1) - OA.at(start) << endl;
    cout << "component: " << bfsResult.size() << endl;

    if (bfsResult.size() != (leftSize + rightSize)) {
        vector<bool> visited(leftSize + rightSize, false);
        for (int n : bfsResult)
            visited.at(n) = true;

        for (int i = 0; i < visited.size(); i++) {
            if (visited.at(i) == false)
                bfsResult.push_back(i);
        }
    }

    vector<int> invertedList = findIndexI(bfsResult);

    for (Edge &e : el) {
        e.src = invertedList.at(e.src);
        e.dst = invertedList.at(e.dst);
    }

    return el;
}

// Main processing and file I/O functions

void readEdgeList( string & fileName, vector<Edge> & el, int & n ) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        exit(EXIT_FAILURE);
    } // if

    int n = 0;
    int node1, node2;
    inputFile >> node1 >> node2;

    while (inputFile >> node1 >> node2) {
        el.push_back({node1, node2});
        if ( node )
    } // while

    cout << "vertex size: " << rightSize << endl;
    cout << "Edge size: " << n << endl;

    inputFile.close();
}

void writeEdgeListFile(string fileName, vector<Edge> el, string oper) {
    ofstream outputFile(fileName.substr(0, fileName.find(".")) + oper + ".el");

    for (Edge e : el)
        outputFile << e.src << " " << e.dst << "\n";

    outputFile.close();
} // writeEdgeListFile

void sortEdgeList(vector<Edge> &el) {
    sort(el.begin(), el.end(), [](const Edge &a, const Edge &b)->bool {
        if (a.src < b.src)
            return true;
        else if (a.src > b.src)
            return false;
        else {
            return a.dst <= b.dst;
        }
    });
}

void recordReorder(vector<string> reorderNameList, vector<double> timeLogList) {
    ofstream outputFile("log.txt");
    outputFile << "                Reorder " << "|" << "         Time\n";
    outputFile << "---------------------------------------\n";

    for (int i = 0; i < timeLogList.size(); i++) {
        outputFile << setw(25) << reorderNameList.at(i) << setw(10) << timeLogList.at(i) << " ms\n";
        outputFile << "---------------------------------------\n";
    }

    outputFile.close();
}

void processReorder(string fileName) {
    clock_t start, end;
    vector<double> timeLogList;
    vector<string> reorderNameList;

    // 
    int n = 0;
    vector<Edge> el, newEL;
    readEdgeList(fileName, el, n);
    cout << "Read edgeList file finish.\n";

    start = clock();
    newEL = BFSOrder(el, n);
    end = clock();
    timeLogList.push_back((1000.0) * (double)(end - start) / CLOCKS_PER_SEC);
    reorderNameList.push_back("BFS Order |");
    sortEdgeList(newEL);
    writeEdgeListFile(fileName, newEL, "_BO");

    start = clock();
    newEL = BFSOrderD(el, leftSize, rightSize);
    end = clock();
    timeLogList.push_back((1000.0) * (double)(end - start) / CLOCKS_PER_SEC);
    reorderNameList.push_back("BFS OrderD |");
    sortEdgeList(newEL);
    writeEdgeListFile(fileName, newEL, "_BOD");

    start = clock();
    newEL = BFSOrderS(el, leftSize, rightSize);
    end = clock();
    timeLogList.push_back((1000.0) * (double)(end - start) / CLOCKS_PER_SEC);
    reorderNameList.push_back("BFS OrderS |");
    sortEdgeList(newEL);
    writeEdgeListFile(fileName, newEL, "_BOS");

    for (int i = 0; i < 10; i++) {
        start = clock();
        newEL = BFSOrderRS(el, leftSize, rightSize);
        end = clock();
        timeLogList.push_back((1000.0) * (double)(end - start) / CLOCKS_PER_SEC);
        reorderNameList.push_back("BFS OrderRS |");
        sortEdgeList(newEL);
        writeEdgeListFile(fileName, newEL, "_BORS" + to_string(i));
    }

    recordReorder(reorderNameList, timeLogList);
}

int main(int argc, char *argv[]) {
    if (argc != 3)
        cout << "----------Parameter Error----------\n";
    else {
        string elFile(argv[1]);
        processReorder(elFile);
    } // else
} // main
