#include <iostream>
#include <stack>
#include <vector>
#include <ctime> // for timing
#include <omp.h>

using namespace std;

const int MAX_V = 10000; 

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V, vector<int>(V, 0)) {}

    void addEdge(int v, int w) {
        adj[v][w] = 1;
    }

    void DFSSequential(int s) {
        vector<bool> visited(V, false);
        stack<int> stack;

        visited[s] = true;
        stack.push(s);

        while (!stack.empty()) {
            s = stack.top();
            stack.pop();

            // cout << s << " ";

            for (int i = 0; i < V; ++i) {
                if (adj[s][i] && !visited[i]) {
                    visited[i] = true;
                    stack.push(i);
                }
            }
        }
    }

    void DFSParallel(int s) {
        vector<bool> visited(V, false);
        stack<int> stack;

        visited[s] = true;
        stack.push(s);

        while (!stack.empty()) {
            int localStack[MAX_V];
            int localStackSize = 0;

            #pragma omp parallel
            {
                #pragma omp single nowait
                {
                    s = stack.top();
                    stack.pop();

                    localStack[localStackSize++] = s;
                }

                #pragma omp for
                for (int i = 0; i < localStackSize; ++i) {
                    int node = localStack[i];
                    // Process node and its neighbors without synchronization
                    for (int j = 0; j < V; ++j) {
                        if (adj[node][j] && !visited[j]) {
                            visited[j] = true;
                            #pragma omp critical
                            {
                                stack.push(j);
                            }
                        }
                    }
                }
            }
        }
    }
};

int main() {
    int numVertices = 5000; // Reduced number of vertices for testing
    Graph g(numVertices);

    // Create a fully connected graph
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (i != j) {
                g.addEdge(i, j);
            }
        }
    }

    // Measure execution time for sequential DFS
    clock_t startSeq = clock();
    g.DFSSequential(0);
    clock_t stopSeq = clock();
    double durationSeq = (double)(stopSeq - startSeq) / CLOCKS_PER_SEC;
    cout << "Sequential DFS Time: " << durationSeq * 1000 << " milliseconds" << endl;

    // Measure execution time for parallel DFS
    clock_t startPar = clock();
    g.DFSParallel(0);
    clock_t stopPar = clock();
    double durationPar = (double)(stopPar - startPar) / CLOCKS_PER_SEC;
    cout << "Parallel DFS Time: " << durationPar * 1000 << " milliseconds" << endl;

    return 0;
}
