#include "boruvka.h"

using namespace std;

int main(int argc, char const * argv[]) {
  string file_name;
  const int max_threads = thread::hardware_concurrency();
  cout << "Enter test file name :: ";
  // cin >> file_name;
  file_name = string(argv[1]);
  Graph graph;
  cout << "inputting graph" << endl;
  graph.readGraph(file_name);
  cout << "inputed graph" << endl;
  //graph.printGraph();
  cout << "boruvka constructor" << endl;
  Boruvka bk(graph, 500, max_threads);

  cout << "calling boruvka" << endl;

  auto start = std::chrono::high_resolution_clock::now();
  vector<Edge> MST_edges = bk.run();
  auto totaltime = std::chrono::high_resolution_clock::now() - start;
  cout << "done boruvka" << endl;

  printf("FINAL MST\n");
  for (int i = 0; i < MST_edges.size(); i++) {
    cout << "(" << MST_edges[i].u << "," << MST_edges[i].v << "," << MST_edges[i].w << ")" << endl;
  }

  int nTh = bk.noOfTheadsCreated;
  cout << "Grand Total No. Of threads spawned during program execution :: " << nTh << endl;
  printf(" Time Taken To Run Parallel Boruvka :: %lld microseconds\n", totaltime);

  return 0;
}

