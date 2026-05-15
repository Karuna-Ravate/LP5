#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int vertices;
    vector<vector<int>> adj;

public:
    Graph(int v) {
        vertices = v;
        adj.resize(vertices);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelBFS(int start) {
        vector<bool> visited(vertices, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS Traversal: ";

        double begin = omp_get_wtime();

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            cout << node << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++) {
                int next = adj[node][i];

                #pragma omp critical
                {
                    if (!visited[next]) {
                        visited[next] = true;
                        q.push(next);
                    }
                }
            }
        }

        double end = omp_get_wtime();

        cout << endl;
        cout << "Time Taken: " << end - begin << " seconds" << endl;
    }

    void parallelDFSUtil(int node, vector<bool>& visited) {
        bool visitNow = false;

        #pragma omp critical
        {
            if (!visited[node]) {
                visited[node] = true;
                visitNow = true;
                cout << node << " ";
            }
        }

        if (!visitNow) {
            return;
        }

        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int next = adj[node][i];

            if (!visited[next]) {
                parallelDFSUtil(next, visited);
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(vertices, false);

        cout << "Parallel DFS Traversal: ";

        double begin = omp_get_wtime();

        parallelDFSUtil(start, visited);

        double end = omp_get_wtime();

        cout << endl;
        cout << "Time Taken: " << end - begin << " seconds" << endl;
    }
};

int main() {
    int vertices, edges;
    int u, v, start;

    omp_set_num_threads(4);

    cout << "Enter number of vertices: ";
    cin >> vertices;

    cout << "Enter number of edges: ";
    cin >> edges;

    Graph g(vertices);

    cout << "Enter edges:" << endl;

    for (int i = 0; i < edges; i++) {
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "Enter starting vertex: ";
    cin >> start;

    g.parallelBFS(start);
    g.parallelDFS(start);

    return 0;
}
// Enter number of vertices: 6
// Enter number of edges: 7
// Enter edges:
// 0 1
// 0 2
// 1 3
// 1 4
// 2 4
// 3 5
// 4 5
// Enter starting vertex: 0



// o/p
// Parallel BFS Traversal: 0 1 2 3 4 5
// Time Taken: 0.0009 seconds
// Parallel DFS Traversal: 0 1 3 5 4 2
// Time Taken: 0.0012 seconds



//  how to run
// g++ filename.cpp -fopenmp -o output
// ./output
//for ubantu
// sudo apt update
// sudo apt install g++