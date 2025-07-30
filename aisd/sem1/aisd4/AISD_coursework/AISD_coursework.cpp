#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <climits>
#include <algorithm>
#include <iomanip>
using namespace std;

const int MaxV = 26; // Maximum number of vertices

// Convert integer index to corresponding character
char Ch(int c) { return c + 'a'; }

class Graph {
    int num, n, m;                     // Counters for DFS numbers, vertices, and edges
    vector<list<int>> adjList;         // Adjacency list representation of the graph
    vector<int> NUM, L;                // Discovery times and lowest points
    vector<vector<int>> edgeMatrix;    // Edge weight matrix
    stack<pair<int, int>> edgeStack;   // Stack to track edges during DFS

    void DBL(int v, int p);            // Depth-first search for biconnected components
    void ManualInput();                // Manual graph creation from user input
    void Random();                     // Random graph generation

public:
    Graph();                           // Constructor
    void PrintAdjList() const;         // Print adjacency list
    void PrintMatrix() const;          // Print adjacency matrix
    void ExecuteDBL();                 // Driver to start DFS on the graph
    void CreateEdgeMatrix();           // Generate edge weight matrix
    void PrintEdgeMatrix() const;      // Print edge weight matrix
    void DijkstraShortestPath(int start, int end, vector<int>& path, int& length) const; // Find shortest path
    void FindShortestPath();           // Interactive shortest path finder
    ~Graph() = default;                // Destructor
};

Graph::Graph() : num(0), n(0), m(0) {
    int choice;
    cout << "Choose an option:\n";
    cout << "1. Manual input\n";
    cout << "2. Random generation\n";
    cin >> choice;

    if (choice == 1) {
        ManualInput();
    } else if (choice == 2) {
        Random();
    }
}

void Graph::ManualInput() {
    adjList.resize(MaxV);
    string s;

    cout << "\nEnter adjacency sets (lines with letters from 'a' to 'z'):\n";
    while (true) {
        cout << "v[" << Ch(n) << "]=";
        cin >> s;
        if (!isalpha(s[0])) break;

        for (auto c : s) {
            if (isalpha(c)) {
                int j = tolower(c) - 'a';
                if (find(adjList[n].begin(), adjList[n].end(), j) == adjList[n].end()) {
                    adjList[n].push_back(j);
                    adjList[j].push_back(n);
                    ++m;
                }
            }
        }
        ++n;
        if (n >= MaxV) break;
    }
    m /= 2; // Each edge is counted twice
}

void Graph::Random() {
    srand(time(0));
    adjList.resize(MaxV / 2);
    n = MaxV / 2;
    m = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (static_cast<double>(rand()) / RAND_MAX < 0.5) {
                adjList[i].push_back(j);
                adjList[j].push_back(i);
                ++m;
            }
        }
    }
}

void Graph::PrintAdjList() const {
    cout << "Graph adjacency list:\n";
    for (int i = 0; i < n; ++i) {
        cout << Ch(i) << ": ";
        if (adjList[i].empty()) {
            cout << "-";
        } else {
            for (int j : adjList[i]) {
                cout << Ch(j) << ' ';
            }
        }
        cout << '\n';
    }
    cout << "\n|V|=" << n << " |E|=" << m << '\n';
}

void Graph::PrintMatrix() const {
    cout << "  ";
    for (int i = 0; i < n; ++i) {
        cout << Ch(i) << " ";
    }
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << Ch(i) << " ";
        vector<int> row(n, 0);

        for (int neighbor : adjList[i]) {
            row[neighbor] = 1;
        }

        for (int value : row) {
            cout << value << " ";
        }
        cout << '\n';
    }
}

void Graph::ExecuteDBL() {
    NUM.resize(n, 0);
    L.resize(n, 0);
    for (int i = 0; i < n; ++i) {
        if (!NUM[i]) {
            DBL(i, -1);
        }
    }
}

void Graph::DBL(int v, int p) {
    using edge = pair<int, int>;
    NUM[v] = L[v] = ++num;

    for (int u : adjList[v]) {
        if (!NUM[u]) {
            edge currentEdge(u, v), poppedEdge(0, 0);
            edgeStack.push(currentEdge);
            DBL(u, v);

            L[v] = min(L[v], L[u]);

            if (L[u] >= NUM[v]) {
                cout << "Edge <" << Ch(v) << '-' << Ch(u)
                     << "> closes a component [";
                do {
                    poppedEdge = edgeStack.top();
                    edgeStack.pop();
                    cout << Ch(poppedEdge.first) << '-' << Ch(poppedEdge.second) << "; ";
                } while (poppedEdge != currentEdge && !edgeStack.empty());
                cout << "]\n";
            }
        } else if (u != p && NUM[u] < NUM[v]) {
            edgeStack.push(make_pair(u, v));
            L[v] = min(L[v], NUM[u]);
        }
    }
}

void Graph::CreateEdgeMatrix() {
    edgeMatrix.assign(n, vector<int>(n, INT_MAX));
    for (int i = 0; i < n; ++i) {
        edgeMatrix[i][i] = 0;
        for (int j : adjList[i]) {
            if (edgeMatrix[i][j] == INT_MAX) {
                edgeMatrix[i][j] = edgeMatrix[j][i] = rand() % 10 + 1;
            }
        }
    }
}

void Graph::PrintEdgeMatrix() const {
    cout << "Edge matrix:\n";

    if (edgeMatrix.empty()) {
        PrintMatrix();
        return;
    }

    cout << "   ";
    for (int i = 0; i < n; ++i) {
        cout << setw(3) << Ch(i) << " ";
    }
    cout << '\n';

    for (int i = 0; i < n; ++i) {
        cout << setw(3) << Ch(i) << " "; // Row label
        for (int weight : edgeMatrix[i]) {
            if (weight == INT_MAX) {
                cout << setw(3) << "INF";
            } else {
                cout << setw(3) << weight;
            }
            cout << " ";
        }
        cout << '\n';
    }
}


void Graph::DijkstraShortestPath(int start, int end, vector<int>& path, int& length) const {
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    vector<bool> visited(n, false);
    dist[start] = 0;

    for (int i = 0; i < n; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (v == -1 || dist[j] < dist[v])) {
                v = j;
            }
        }

        if (dist[v] == INT_MAX) break;
        visited[v] = true;

        for (int u : adjList[v]) {
            int weight = edgeMatrix.empty() ? 1 : edgeMatrix[v][u];
            if (dist[v] + weight < dist[u]) {
                dist[u] = dist[v] + weight;
                prev[u] = v;
            }
        }
    }

    length = dist[end];
    for (int v = end; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
}

void Graph::FindShortestPath() {
    char start, end;
    cout << "Enter the start vertex: ";
    cin >> start;
    cout << "Enter the end vertex: ";
    cin >> end;

    if (start < 'a' || start >= 'a' + n || end < 'a' || end >= 'a' + n) {
        cout << "Invalid vertices. Please use vertices from 'a' to '" << Ch(n - 1) << "'.\n";
        return;
    }

    int startIdx = start - 'a';
    int endIdx = end - 'a';
    vector<int> path;
    int length = 0;

    DijkstraShortestPath(startIdx, endIdx, path, length);

    cout << "\nShortest path from " << start << " to " << end << ":\n";
    if (length == INT_MAX) {
        cout << "No path exists.\n";
    } else {
        cout << "Length: " << length << "\nPath: ";
        for (int v : path) {
            cout << Ch(v) << ' ';
        }
        cout << '\n';
    }
}

int main() {
    int answer;
    cout << "===== Start =====\n";
    Graph graph;
    do {
        cout << "\nMake a choice:\n";
        cout << "1. Print adjacency list\n";
        cout << "2. Print adjacency matrix\n";
        cout << "3. Randomize edge matrix\n";
        cout << "4. Print edge matrix\n";
        cout << "5. Find shortest path between 2 points\n";
        cout << "6. Find biconnected components\n";
        cout << "0. Exit\n";
        cin >> answer;
        switch (answer) {
        case 1:
            graph.PrintAdjList();
            break;
        case 2:
            cout << "\nGraph adjacency matrix:\n";
            graph.PrintMatrix();
            break;
        case 3:
            graph.CreateEdgeMatrix();
            break;
        case 4:
            graph.PrintEdgeMatrix();
            break;
        case 5:
            graph.FindShortestPath();
            break;
        case 6:
            graph.ExecuteDBL();
            break;
        default:
            break;
        }
    } while (answer);
    cout << "===== End =====\n";
    cout << "Press any key to continue...";
    cin.get();
    cin.get();
}

