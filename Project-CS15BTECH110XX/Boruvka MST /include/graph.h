#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
using namespace std;

typedef struct Edge {
  int u;
  int v;
  int w;
  Edge(int u, int v, int w) {
    this->u = u;
    this->v = v;
    this->w = w;
  }
  Edge():u(0), v(0), w(0) {}
} Edge;

class Graph {
public:
  int noOfVertices;
  int noOfEdges;
  vector<vector<Edge> > adjList;
  Graph() {
    noOfVertices = 0;
    noOfEdges = 0;
  }
  int readGraph(string file_name);
  void printGraph();

};

#endif