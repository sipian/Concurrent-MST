// STL implementation of Prim's algorithm for MST
#include <bits/stdc++.h>
#include <iostream>
#include <climits>
#include "PriorityQueue.h"
#define INF INT_MAX

using namespace std;
ifstream input ("input.txt");

void concurrentAdd(int v, int u, int weight, int ID);

// This class represents a directed graph using adjacency list representation
class Graph
{
    int V;

public:
    list< pair<int, int> > *adj;

    vector<int> key;
    vector<int> parent;
    vector<bool> inMST;

    // Constructor
    void init(int V) {
        this->V = V;
        adj = new list<pair<int, int> > [V];
    }

    // function to add an edge to graph
    void addEdge(int u, int v, int w) {

        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }

    // function to find minimum spanning tree
    void primMST(int V) {

        pq.insert(0, 0, 0);
        key[0] = 0;

        int pqsize = V;

        /* Looping till priority queue becomes empty */
        while (pqsize != 0) {
            int u = pq.removeMin().second;
            inMST[u] = true;  // Include vertex in MST
            // m = number of threads
            int m = adj[u].size();
            thread *thr = new std::thread[m];
            int id = 0;

            list< pair<int, int> >::iterator i;
            for ( i = adj[u].begin(); i != adj[u].end(); ++i) {

                int v = (*i).first;
                int weight = (*i).second;
                // concurrentAdd(v,u,weight,0);
                thr[id] = std::thread(concurrentAdd, v, u, weight, id + 1);
                id++;
            }

            for (int j = 0; j < m; j++) {
                thr[j].join();
            }

            pqsize--;

        }

        
        // Print edges of MST using parent array
        printf("FINAL MST\n");
        for (int i = 1; i < V; ++i) {
            cout << "(" << i << "," << parent[i] << ")" << endl;
        }

    }
};

Graph g;

void concurrentAdd(int v, int u, int weight, int ID) {

    if (g.inMST[v] == false && g.key[v] > weight) {
        g.key[v] = weight;
        pq.insert(v, ID, weight);
        g.parent[v] = u;
    }

}

// Main function
int main()
{
    // Creating Graph

    // V = Number of vertices
    // E = Number of edges
    int V, E;
    input >> V;
    input >> E;
    g.init(V);

    // Key is the vector of key
    g.key.resize(V, INF);

    // Stores parent of nodes in MST
    g.parent.resize(V, -1);

    // Adds the nodes to MST
    g.inMST.resize(V, false);
    pq.init(V);
    int vertex1, vertex2, weight;

    // Initializing graph
    for (int i = 0; i < E; i++) {

        input >> vertex1 >> vertex2 >> weight;
        g.addEdge(vertex1, vertex2, weight);

    }

    cout << "\nGraph Description==>\n\tNo. Of vertices : " << V << "\n\tNo. Of edges : " << E << "\n";
    for (int u = 0; u < V; ++u) {
        cout << "\t" << u << ": ";
        for (auto i = g.adj[u].begin(); i != g.adj[u].end(); ++i) {
            int v = (*i).first;
            int w = (*i).second;
            cout << "(" << v << "," << w << ") --> ";
        }
        cout << "END" << endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    g.primMST(V);
    auto totaltime = std::chrono::high_resolution_clock::now() - start;
    printf("Execution time for parallel Prim's is %lld microseconds\n", totaltime);

    return 0;
}