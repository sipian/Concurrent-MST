#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
using namespace std;

typedef struct Node {
  int v;
  int w;
  Node(int vertex, int weight) {
    v = vertex;
    w = weight;
  }
} Node;

typedef struct Edge {
  int v1;
  int v2;
  int w;
} Edge;

class Graph {
public:
  int no_of_vertices;
  int no_of_edges;
  vector<vector<Node>> adj_list;
  Graph() {
    no_of_vertices = 0;
    no_of_edges = 0;
  }
  int readGraph(string file_name);
  void printGraph();

};

#endif
