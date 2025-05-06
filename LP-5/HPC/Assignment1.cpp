// Title - Parallel Searching Algorithms
// Problem Statement - Design and implement Parallel Breadth First Search and Depth First Search based on existing algorithms using OpenMP. Use a Tree or an undirected graph for BFS and DFS.

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
    int v;
    vector<vector<int>> adj;

    public: 
        Graph (int v){
            this->v = v;
            adj.resize(v);
        }

        void addEdge(int v1, int v2){
            adj[v1].push_back(v2);
            adj[v2].push_back(v1);
        }

        void printGraph(){
            for(int i=0; i<v; i++){
                cout << i << " -> [ ";
                for(int j=0; j<adj[i].size(); j++){
                    cout << adj[i][j] << " ";
                }
                cout << "]" << endl;
            }
        }

        void sequentialBFS(int start){
            vector<bool> visited(v, false);
            queue<int> q;

            visited[start] = true;
            q.push(start);

            while (!q.empty()){
                int vertex = q.front();
                q.pop();

                for(int neighbour : adj[vertex]){
                    if(!visited[neighbour]){
                        visited[neighbour] = true;
                        q.push(neighbour);
                    }
                }
            }
        }

        void parallelBFS(int start){
            vector<bool> visited(v, false);
            queue<int> q;

            visited[start] = true;
            q.push(start);

            while (!q.empty()){
                int n = q.size();
               
                #pragma omp parallel for default(none) shared(q, visited, n)
                for(int i=0; i<n; i++){
                    int vertex;

                    #pragma omp critical
                    {
                        vertex = q.front();
                        q.pop();
                    }

                    for(int neighbour : adj[vertex]){
                        #pragma omp critical
                        {
                            if(!visited[neighbour]){
                                visited[neighbour] = true;
                                q.push(neighbour);
                            }
                        }
                    }
                }
            }
        }

        void sequentialDFS(int start){
            vector<bool> visited(v, false);

            sequentialDFSUtil(start, visited);
        }

        void sequentialDFSUtil(int start, vector<bool>& visited){
            visited[start] = true;

            for(int neighbour : adj[start]){
                if(!visited[neighbour]){
                    sequentialDFSUtil(neighbour, visited);
                }
            }
        }

        void parallelDFS(int start){
            vector<bool> visited(v, false);

            #pragma omp parallel
            {
                #pragma omp single
                {
                    parallelDFSUtil(start, visited);
                }
            }
        }

        void parallelDFSUtil(int start, vector<bool>& visited){
            visited[start] = true;

            for(int neighbour : adj[start]){
                #pragma omp task 
                {
                    if(!visited[neighbour]){
                        parallelDFSUtil(neighbour, visited);
                    }
                }
            }

            #pragma omp taskwait
        }
};

int main() {
    Graph g(7);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    g.printGraph();

    auto start = omp_get_wtime();
    g.sequentialBFS(0);
    auto stop = omp_get_wtime();
    auto duration = stop - start;
    cout << "Time Taken By Sequential BFS : " << duration << endl;
    
    start = omp_get_wtime();
    g.parallelBFS(0);
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken By Parallel BFS : " << duration << endl;

    start = omp_get_wtime();
    g.sequentialDFS(0);
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken By Sequential DFS : " << duration << endl;
    
    start = omp_get_wtime();
    g.parallelDFS(0);
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken By Parallel DFS : " << duration << endl;

    return 0;
}

// Run Program -
// Compile Command - g++ -fopenmp Assignment1.cpp -o assignment1
// Execute Command - ./assignment1