#include "graph.h"
#include <map>
#include <limits.h>
#include <vector>
#include <future> 
#include <atomic>
#include <thread>
#include<algorithm>
using namespace std;

class Boruvka {
  Graph graph;
  int noOfVertices;
  int hardwareConcurrency;
  int minimumSizeOfArrayForSequentialOperation;

  vector<int> root;   // array for the union find data-structure used in finding connected components during boruvka
  map<int, vector<int> > groups;  //vertices belonging to which component which is input before any iteration
  vector<vector<Edge> > outEdges;  // all outgoing edge for each component
  map<int, vector<Edge> > inEdges; // inner edges (mst edges) for each component through one iteration
  vector<Edge> cheapest;           // the cheapest outgoing edge for each component through one iteration
  map<int, vector<int> > newGroups; //vertices belonging to which component obtained after iteration
  vector<vector<Edge> > newOutEdges; //outgoingg edges for each loop obtained after iteration

  vector<int> keyset;   //stores the indices of the components between iteration
  vector<int> getKeyset();
  vector<int> getNewKeyset();
  static bool edgeCompare (const Edge &e1, const Edge &e2) {
    return (e1.w < e2.w);
  };
  tuple<int, int> getLoopBounds(int threadID, int starting_index, int final_index);

  //functions for finding the minimum weighted edge
  void findMinEdges();
  void findMinEdgesPerThread (int i);
  Edge minElementSequential(vector<Edge> &array, int start, int end);
  Edge minElementGraph(vector<Edge> &array);

  void setNewGroups();


  void mergeNodeGroups(int root1, int root2);
  void mergeNodeEdges(int root1, int root2, Edge &picked);
  int getRootOfSubTree(int i);
  void mergeSubTrees(int i, int j);
  void setNewOutEdges();
  void setNewOutEdgesPerThread(int i);
  void deleteDuplicateOfEdgeInList(vector<Edge> &list, Edge &e);
  void deleteAllCycles(int id);
public:
  atomic_int noOfTheadsCreated;
  Boruvka(Graph &graph, int min_array_size, int no_of_threads);
  vector<Edge> run();

};