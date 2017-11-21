#include "graph.h"
#include <fstream>

int Graph::readGraph(string file_name) {

  int u, v, w;
  ifstream fin;
  fin.open(file_name.c_str());
  if(!fin) {
    cerr << file_name << " not found." << endl;
    return 0;
  }
  fin >> noOfVertices;
  adjList = vector<vector<Edge> > (noOfVertices);
  int edges = 0;
  
  //creating an undirected graph
  while(!fin.eof()) {
    fin >> u >> v >> w;
    Edge Edge1(u,v,w);
    Edge Edge2(v,u,w);
    adjList[u].push_back(Edge1);
    adjList[v].push_back(Edge2);
    edges += 2;
  }

  noOfEdges = edges;
  fin.close();
  return 1; 
}

void Graph::printGraph() {
  cout << "\nGraph Description==>\n\tNo. Of vertices : "<<noOfVertices<<"\n\tNo. Of edges : "<<noOfEdges<<"\n";
  for (int i = 0; i < noOfVertices; ++i) {
    cout << "\t" << i << ": ";
    for(const Edge &Edge : adjList[i]) {
      cout << "(" << Edge.v << "," << Edge.w << ") --> ";
    }
    cout << endl;
  }
}