#include "boruvka.h"

/* 
 * This function computes a partition of the bound of loop a thread should be dedicated to
 * Assume loops are of form for(int i = starting_index; i < final_index; i++)
*/
tuple<int, int> Boruvka::getLoopBounds(int threadID, int starting_index, int final_index) {
  int start, end;

  int cnt = (final_index - starting_index) / hardwareConcurrency;
  start = cnt * threadID + starting_index;

  if (cnt == 0) {
    start = threadID + starting_index;
    end = start + 1;
    if (start >= final_index) //if loop won't be executed even once
      start = end;
  }
  else {
    start = threadID * cnt + starting_index;

    // handling the case when cnt is not divisible by hardwareConcurrency
    if (threadID < hardwareConcurrency - 1)
      end = start + cnt;
    else
      end = final_index;
  }
  return make_tuple(start, end);
}

/* 
 * Constructor for Boruvka
 * 
*/
Boruvka::Boruvka(Graph &graph, int no_of_threads): hardwareConcurrency(no_of_threads) {
  this->graph = graph;
  noOfVertices = graph.noOfVertices;
  minimumSizeOfArrayForSequentialOperation = 500;
  noOfTheadsCreated = 0;

  root = vector<int> (noOfVertices);
  groups = map<int, vector<int> > ();
  outEdges = vector<vector<Edge> >(noOfVertices);
  inEdges = map<int, vector<Edge> >();
  cheapest = vector<Edge> (noOfVertices);
  newGroups = map<int, vector<int> > ();
  newOutEdges = vector<vector<Edge> >(noOfVertices);

  // initially #noOfVertices components
  for (int i = 0; i < noOfVertices; i++)
  {
    groups[i] = vector<int>();
    groups[i].push_back(i);
    outEdges[i] = graph.adjList[i];   // outgoing edge for a vertex
    inEdges[i] = vector<Edge> ();
    root[i] = i;    // make every component parent of itself
  }
}
/* 
 * Constructor for Boruvka
 * 
*/
vector<Edge> Boruvka::run() {

  while (groups.size() > 1) {
    // preparing the data structures for the next iteration
    // on the new components computed previously
    newOutEdges = vector<vector<Edge> > (noOfVertices);
    newGroups = groups;
    keyset = getKeyset();

    findMinEdges();   // parallelly find the minimum edge
    setNewGroups();   // parallelly compute the connected components

    if (newGroups.size() == 1) {
      // MST has been made
      break;
    }
    setNewOutEdges();   //merge the connected components
  }
  return inEdges[0];
}

//returns a vector of the component id-s of the components present currently in group i.e. input in the current iteration
vector<int> Boruvka::getKeyset() {
  vector<int> keys;
  for (auto it = groups.begin(); it != groups.end(); it++) {
    keys.insert(keys.end(), it->first);
  }
  return keys;
}
//returns a vector of the component id-s of the components currently in newgroup i.e. output after the current iteration
vector<int> Boruvka::getNewKeyset() {
  vector<int> keys;
  for (auto it = newGroups.begin(); it != newGroups.end(); it++) {
    keys.insert(keys.end(), it->first);
  }
  return keys;
}

// concurrent implementation of finding minEdge on subset of vertices 
void Boruvka::findMinEdges() {

  std::vector<thread> th(hardwareConcurrency);
  for (int i = 0; i < hardwareConcurrency; i++) {
    th[i] = thread(&Boruvka::findMinEdgesPerThread, this, i);
  }
  noOfTheadsCreated += hardwareConcurrency;

  for (int i = 0; i < hardwareConcurrency; i++) {
    th[i].join();
  }
}

Edge Boruvka::minElementSequential(vector<Edge> &array, int start, int end) {
  auto begin_iterator = array.begin();
  auto end_iterator = array.begin();
  advance(begin_iterator, start);
  advance(end_iterator, end);
  return *min_element(begin_iterator, end_iterator, Boruvka::edgeCompare);
}

Edge Boruvka::minElementGraph(vector<Edge> &array) {
  // if size is small enough no need to parallelize
  // Better if done sequentially
  if (array.size() < minimumSizeOfArrayForSequentialOperation ) {
    return *min_element(array.begin(), array.end(), Boruvka::edgeCompare);
  }
  else {
    // Parallel implementation of finding the minimum element
    int sz = array.size(), rl;
    int limit = sz / minimumSizeOfArrayForSequentialOperation;
    std::vector<std::future<Edge> > th(limit);
    for (int i = 0; i < limit; i++) {

      if (i == limit - 1) {
        rl = sz;
      }
      else {
        rl = (i + 1) * minimumSizeOfArrayForSequentialOperation;
      }
      th[i] = std::async(std::launch::async, &Boruvka::minElementSequential, this, ref(array), i * minimumSizeOfArrayForSequentialOperation, rl);
    }

    noOfTheadsCreated += limit;
    Edge result(-1, -1, INT_MAX);

    for (int i = 0; i < limit; i++) {

      Edge tmp = th[i].get();
      if (tmp.w < result.w) {
        result = tmp;
      }
    }
    return result;
  }
}

void Boruvka::findMinEdgesPerThread (int threadID) {
  auto loopBounds = getLoopBounds(threadID, 0, keyset.size());
  for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {
    int component = keyset[i];
    cheapest[component] = minElementGraph(outEdges[component]);
  }
}

// sequential implementation
void Boruvka::setNewGroups() {
  for (int i = 0; i < keyset.size(); i++) {
    int group_id = keyset[i];
    Edge minEdge = cheapest[group_id];

    int root1 = getRootOfSubTree(minEdge.u);
    int root2 = getRootOfSubTree(minEdge.v);

    // if both trees are of the same component
    if (root1 == root2) {
      continue;
    }
    if (root1 < root2) {
      mergeNodeGroups(root1, root2);
      mergeNodeEdges(root1, root2, minEdge);
    }
    else {
      mergeNodeGroups(root2, root1);
      mergeNodeEdges(root2, root1, minEdge);
    }
    mergeSubTrees(root1, root2);
  }
  // for (int i = 0; i < noOfVertices; ++i)
  // {
  //   printf("%d - %d\n", i,  root[i]);
  // }
  // exit(0);
}

void Boruvka::mergeNodeGroups(int root1, int root2) {
  newGroups[root1].insert(newGroups[root1].end(), newGroups[root2].begin(), newGroups[root2].end());
  newGroups.erase(root2);
}

void Boruvka::mergeNodeEdges(int root1, int root2, Edge &picked) {
  inEdges[root1].insert(inEdges[root1].end(), inEdges[root2].begin(), inEdges[root2].end());
  // adding the MST edge in inEdges
  inEdges[root1].push_back(picked);
  inEdges.erase(root2);
}

// union find data structure find algorithm
int Boruvka::getRootOfSubTree(int i) {
  while (root[i] != i) {
    i = root[i];
  }
  return i;
}

// union find data structure union algorithm
void Boruvka::mergeSubTrees(int i, int j) {
  // performing path compression for smaller trees
  if (i < j)
    root[j] = i;
  else
    root[i] = j;
}

void Boruvka::setNewOutEdges() {

  std::vector<thread> th(hardwareConcurrency);
  // get the merged components of the merged groups
  keyset = getNewKeyset();
  for (int i = 0; i < hardwareConcurrency; i++) {
    th[i] = thread(&Boruvka::setNewOutEdgesPerThread, this, i);
  }
  noOfTheadsCreated += hardwareConcurrency;

  for (int i = 0; i < hardwareConcurrency; i++) {
    th[i].join();
  }
  // save the updated edges and groups
  outEdges = newOutEdges;
  groups = newGroups;
}

void Boruvka::setNewOutEdgesPerThread(int threadID) {
  auto loopBounds = getLoopBounds(threadID, 0, keyset.size());
  for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {

    int newcomponentID = keyset[i];
    // getting all nodes that have been merged into 1 node
    std::vector<int> group_subcomponents = newGroups[newcomponentID];
    newOutEdges[newcomponentID] = vector <Edge> ();

    for (auto it : group_subcomponents) {
      if (groups.find(it) == groups.end()) {
        // the 2 nodes have already been merged
        continue;
      }
      newOutEdges[newcomponentID].insert(newOutEdges[newcomponentID].end(), outEdges[it].begin(), outEdges[it].end());
      outEdges[it].clear();  //edges not required as they have already been merged into a new edges
    }

    for (auto it : inEdges[newcomponentID]) {
      deleteDuplicateOfEdgeInList(newOutEdges[newcomponentID], it);
    }
    deleteAllCycles(newcomponentID);
  }
}
//deletes all duplicates of edge 'e' in the vector 'v'
void Boruvka::deleteDuplicateOfEdgeInList(vector<Edge> &list, Edge &e) {
  auto it = list.begin();
  while (it != list.end()) {
    if ((it->u == e.u && it->v == e.v) || (it->u == e.v && it->v == e.u)) {
      *it = list.back();   //storing the edge e at the end
      list.pop_back();
      continue;
    }
    it++;
  }
}

//iterates through all edges of component 'id' and deletes all edges that have two endpoints in the same component
void Boruvka::deleteAllCycles(int id) {
  vector<Edge> *v = &newOutEdges[id];
  vector<int> subs = groups[id];

  auto it = v->begin();
  while (it != v->end()) {
    int x = (*it).u;
    int y = (*it).v;
    if ( (find(subs.begin(), subs.end(), x) != subs.end()) && (find(subs.begin(), subs.end(), y) != subs.end())) {
      *it = v->back();   //storing the edge e at the end
      v->pop_back();     //deleting the last edge
      continue;
    }
    it++;
  }
}