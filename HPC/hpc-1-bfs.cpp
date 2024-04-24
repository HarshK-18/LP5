#include <iostream>
#include <queue>
#include <ctime>
#include <omp.h>

using namespace std;

const int MAX_V = 10000; // Maximum number of vertices

class Graph {
    int V;
    int** adj;

public:
    Graph(int V) : V(V) {
        adj = new int*[V];
        for (int i = 0; i < V; ++i) {
            adj[i] = new int[V];
            for (int j = 0; j < V; ++j) {
                adj[i][j] = 0;
            }
        }
    }

    void addEdge(int v, int w) {
        adj[v][w] = 1;
    }

    void BFSSequential(int s) {
        bool visited[MAX_V] = {false};
        queue<int> q;

        visited[s] = true;
        q.push(s);

        while (!q.empty()) {
            int current = q.front();
            q.pop();
//            cout << current << " ";

            for (int i = 0; i < V; ++i) {
                if (adj[current][i] && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
    }

    void BFSParallel(int s) {
        bool visited[MAX_V] = {false};
        queue<int> q;

        visited[s] = true;
        q.push(s);

        while (!q.empty()) {
            #pragma omp parallel
            {
                #pragma omp single nowait
                {
                    while (!q.empty()) {
                        int current = q.front();
                        q.pop();
//                        cout << current << " ";
                    }
                }

                #pragma omp for
                for (int i = 0; i < V; ++i) {
                    if (!q.empty()) {
                        int current;
                        #pragma omp critical
                        {
                            current = q.front();
                            q.pop();
                        }

                        if (!visited[current]) {
                            visited[current] = true;
                            #pragma omp critical
                            {
                                for (int j = 0; j < V; ++j) {
                                    if (adj[current][j] && !visited[j]) {
                                        q.push(j);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

int main() {
    const int numVertices = 4000;
    Graph g(numVertices);

    // Create a fully connected graph
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (i != j) {
                g.addEdge(i, j);
            }
        }
    }

    // Measure execution time for sequential BFS
    clock_t startSeq = clock();
    g.BFSSequential(0);
    clock_t stopSeq = clock();
    double durationSeq = (double)(stopSeq - startSeq) / CLOCKS_PER_SEC;
    cout << "Sequential BFS Time: " << durationSeq * 1000 << " milliseconds" << endl;

    // Measure execution time for parallel BFS
    clock_t startPar = clock();
    g.BFSParallel(0);
    clock_t stopPar = clock();
    double durationPar = (double)(stopPar - startPar) / CLOCKS_PER_SEC;
    cout << "Parallel BFS Time: " << durationPar * 1000 << " milliseconds" << endl;

    return 0;
}
