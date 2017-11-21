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
  fin >> no_of_vertices;
  adj_list = vector<vector<Node> > (no_of_vertices);
  int edges = 0;
  //creating an undirected graph
  while(!fin.eof()) {
    fin >> u >> v >> w;
    Node node1(v,w);
    Node node2(u,w);
    adj_list[u].push_back(node1);
    adj_list[v].push_back(node2);
    edges += 2;
  }
  no_of_edges = edges;
  fin.close();
  return 1; 
}

void Graph::printGraph() {
  cout << "\nGraph Description==>\n\tNo. Of vertices : "<<no_of_vertices<<"\n\tNo. Of edges : "<<no_of_edges<<"\n";
  for (int i = 0; i < no_of_vertices; ++i) {
    cout << "\t" << i << ": ";
    for(const Node &node : adj_list[i]) {
      cout << "(" << node.v << "," << node.w << ") --> ";
    }
    cout << endl;
  }
}