// STL implementation of Prim's algorithm for MST
#include<bits/stdc++.h>
#include <iostream>
#include <climits>
#include <chrono>
using namespace std;
using namespace chrono;

# define INF INT_MAX
 
// iPair ==>  Integer Pair
typedef pair<int, int> iPair;
ifstream input ("input.txt");

priority_queue< iPair, vector <iPair> , greater<iPair> > pq;
vector<int> key;
 
// To store parent array which in turn store MST
vector<int> parent;

// To keep track of vertices included in MST
vector<bool> inMST;
 
// This class represents a directed graph using
// adjacency list representation
class Graph
{
    int V;    // No. of vertices
 
    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    
 
public:

    list< pair<int, int> > *adj;
    Graph(int V);  // Constructor
 
    // function to add an edge to graph
    void addEdge(int u, int v, int w);
 
    // Print MST using Prim's algorithm
    void primMST();
};
 
// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<iPair> [V];
}
 
void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}
 

void mst(int v, int u, int weight) {

    if (inMST[v] == false && key[v] > weight)
    {
        // Updating key of v
        key[v] = weight;
        pq.push(make_pair(key[v], v));
        parent[v] = u;
    }
}

// Prints shortest paths from src to all other vertices
void Graph::primMST()
{
    
    
    pq.push(make_pair(0, 0));
    key[0] = 0;
 
    /* Looping till priority queue becomes empty */
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
 
        inMST[u] = true;  // Include vertex in MST
 
        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            
            int v = (*i).first;
            int weight = (*i).second;
 
            mst(v,u,weight);
        }
    }

    // Print edges of MST using parent array
    printf("FINAL MST\n");
    for (int i = 1; i < V; ++i) {
        cout << "(" << i << "," << parent[i] << ")" << endl;
    }
}
 
int main()
{

    int V,E;
    input>>V;
    input>>E;
    Graph g(V);

    // Key is the vector of key
    key.resize(V,INF);

    // Stores parent of nodes in MST
    parent.resize(V,-1);

    // Adds the nodes to MST
    inMST.resize(V,false);
    int vertex1, vertex2, weight;

    // Initializing graph
    for(int i=0;i<E;i++) {

        input>>vertex1>>vertex2>>weight;
        g.addEdge(vertex1,vertex2,weight);

    }
    cout << "\nGraph Description==>\n\tNo. Of vertices : "<<V<<"\n\tNo. Of edges : "<<E<<"\n";
    for (int u = 0; u < V; ++u) {
        cout << "\t" << u << ": ";
        for(auto i = g.adj[u].begin(); i != g.adj[u].end(); ++i) {
            int v = (*i).first;
            int w = (*i).second;
            cout << "(" << v << "," << w << ") --> ";
        }
        cout<< "END" << endl;
    }
  
    auto start = std::chrono::high_resolution_clock::now();
    g.primMST();
    auto totaltime = std::chrono::high_resolution_clock::now() - start;
    printf("Execution time for sequential Prim's is %lld microseconds\n", totaltime);
    return 0;
}