#include "graph.h"
#include "barrier.h"
#include "minheap.h"
#include <tuple>
#include <thread>
#include <atomic>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <condition_variable>

using namespace std;

std::mutex debug_lock;
const int max_threads = thread::hardware_concurrency();
atomic_int color_tracker(1);
atomic_bool connected_component_change(true);

// break loops on basis of thread id
// as for(int i = start; i < end; i++)
tuple<int, int> getLoopBounds(int threadID, int starting_index, int final_index) {
  int start, end;

  int cnt = (final_index - starting_index) / max_threads;
  start = cnt * threadID + starting_index;

  if (cnt == 0) {
    start = threadID + starting_index;
    end = start + 1;
    if (start >= final_index) //if loop won't be executed even once
      start = end;
  }
  else {
    start = threadID * cnt + starting_index;
    // handling the case when cnt is not divisible by max_threads
    if (threadID < max_threads - 1)
      end = start + cnt;
    else
      end = final_index;
  }
  return make_tuple(start, end);
}


void PRIM_MST(int threadID, Graph &graph, vector<atomic_int> &color, vector<atomic_bool> &visited, Barrier &node_barrier) {

  auto loopBounds = getLoopBounds(threadID, 0, graph.no_of_vertices);
  //initializing the vectors
  for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {
    color[i] = 0;
    visited[i] = false;
  }
  // printf("Thread %d %d %dEntering1\n", threadID, get<0>(loopBounds), get<1>(loopBounds));
  node_barrier.await();
  // printf("Thread %d Exiting1\n", threadID);

  vector<Edge> MST_edges(graph.no_of_edges / max_threads);
  int MST_edges_counter = 0;
  MinHeap heap(graph.no_of_vertices);

  int zero_color = 0;
  for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {

    int curr_color = atomic_fetch_add(&color_tracker, 1);
    printf("Thread %d *********** 0 -- %d\n", threadID, i);

    if (!atomic_compare_exchange_strong(&color[i], &zero_color, curr_color)) {
      // v is already colored and has been already processed by another processsor
      continue;
    }
    // beginning Prim algorithm from vertex i
    HeapNode e1(0, i, i);
    int pos_in_heap = heap.insert(e1);

    while (!heap.empty()) {
      HeapNode* el = heap.extract_min();

      int min_edge_vertex = el->vertex1;
      if (color[min_edge_vertex] != curr_color) {
        // node has been processed by other thread
        // the tree obtained so far is mature
        // Hence breaking out of this loop
        break;
      }
      bool breakable = false;
      for (const Node& u : graph.adj_list[min_edge_vertex]) {
        if (color[u.v] != 0 && color[u.v] != curr_color) {
          breakable = true;
          break;
        }
      }
      if (breakable) {
        break;
      }
      if (visited[min_edge_vertex]) {
        continue;
      }
      // adding this min_edge to the MST
      visited[min_edge_vertex] = true;
      MST_edges[MST_edges_counter].v1 = el->vertex1;
      MST_edges[MST_edges_counter].v2 = el->vertex2;
      MST_edges[MST_edges_counter].w = el->key;
      MST_edges_counter++;

      for (const Node& u : graph.adj_list[min_edge_vertex]) {

        if (visited[u.v] && color[u.v] == curr_color)
          continue;

        atomic_compare_exchange_strong(&color[u.v], &zero_color, curr_color);
        // reusing min_edge node
        el->vertex1 = u.v;
        el->vertex1 = min_edge_vertex;
        el->key = u.w;

        if (heap.positionArray[u.v] == -1) {
          heap.insert(*el);
        }
        else {
          heap.decrease_key(heap.positionArray[u.v], *el);
        }
      }
    }
  }
  node_barrier.await();

  // handling unvisited vertices
  for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {
    if (!visited[i]) {
      int min_weight = INT_MAX, min_index;
      for (const Node& u : graph.adj_list[i]) {
        if (min_weight > u.w) {
          min_weight = u.w;
          min_index = u.v;
        }
      }
      MST_edges[MST_edges_counter].v1 = i;
      MST_edges[MST_edges_counter].v2 = min_index;
      MST_edges[MST_edges_counter].w = min_weight;
      MST_edges_counter++;
    }
  }
  debug_lock.lock();
  printf("Thread %d 1 ++++++++++++ \n", threadID);

  for (int i = 0; i < MST_edges_counter; ++i) {
    cout << "(" << MST_edges[i].v1 << "," << MST_edges[i].v2 << "," << MST_edges[i].w << ") -> ";
  }
  cout << endl;
  debug_lock.unlock();


  // running connected components on the subgraph using union find data structure
  for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {
    color[i] = i;
  }
  node_barrier.await();
  int i, j;
  while (connected_component_change) {
    connected_component_change = false;
    for (int l = 0; l < MST_edges_counter; ++l) {
      i = MST_edges[l].v1;
      j = MST_edges[l].v2;
      if (color[i] == color[color[i]] && color[j] < color[i]) {
        color[i] = color[j];
        connected_component_change = true;
      }
      else if (color[j] == color[color[j]] && color[j] > color[i]) {
        color[j] = color[i];
        connected_component_change = true;
      }
    }
    node_barrier.await();
    for (int i = get<0>(loopBounds); i < get<1>(loopBounds); i++) {

      while (color[i] != color[color[i]]) {
        color[i] = color[color[i]];
      }
    }
    change = 
  }



  //

}

int main(int argc, char const * argv[]) {
  string file_name;
  cin >> file_name;
  Graph graph;
  Barrier node_barrier(max_threads);
  graph.readGraph(file_name);
  graph.printGraph();
  vector<thread> th(max_threads);

  vector<atomic_int> color(graph.no_of_vertices);
  vector<atomic_bool> visited(graph.no_of_vertices);

  for (int i = 0; i < max_threads; i++) {
    th[i] = thread(PRIM_MST, i, ref(graph), ref(color), ref(visited), ref(node_barrier));
  }

  printf("Joining\n");
  for (int i = 0; i < max_threads; i++) {
    th[i].join();
  }
  printf("Joining Done\n");

  return 0;
}