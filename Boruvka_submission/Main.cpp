#include "boruvka.h"
using namespace std;

int main(int argc, char const * argv[]) {
  string file_name;
  const int max_threads = thread::hardware_concurrency();
  cin >> file_name;
  Graph graph;
  graph.readGraph(file_name);
  graph.printGraph();
  Boruvka bk(graph, 300, max_threads);
  vector<Edge> MST_edges = bk.run();

  printf("FINAL MST\n");
  for (int i = 0; i < MST_edges.size(); i++) {
    cout << "(" << MST_edges[i].u << "," << MST_edges[i].v << "," << MST_edges[i].w<< ")" << endl;
  }

  int nTh = bk.noOfTheadsCreated;
  cout << "Grand Total No Of threads spawned :: " << nTh << endl;

  return 0;
}