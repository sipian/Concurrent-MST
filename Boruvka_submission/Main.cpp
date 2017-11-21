#include "boruvka.h"
#include <sys/time.h>

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
  struct timeval  tv1, tv2;

  cout << "calling boruvka" << endl;
  gettimeofday(&tv1, NULL);
  vector<Edge> MST_edges = bk.run();
  gettimeofday(&tv2, NULL);
  cout << "done boruvka" << endl;

  double timeTaken = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);       //get CPU time taken in seconds
 
  printf("FINAL MST\n");
  for (int i = 0; i < MST_edges.size(); i++) {
    cout << "(" << MST_edges[i].u << "," << MST_edges[i].v << "," << MST_edges[i].w<< ")" << endl;
  }

  int nTh = bk.noOfTheadsCreated;
  cout << "Grand Total No. Of threads spawned during program execution :: " << nTh << endl;
  cout << "Time Taken To Run Parallel Boruvka :: " << timeTaken << " seconds " << endl;

  return 0;
}

