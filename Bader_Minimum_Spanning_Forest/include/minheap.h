#ifndef MINHEAP_H_
#define MINHEAP_H_

#include "graph.h"

typedef struct HeapNode {
  int key;
  int vertex1;  //the characteristic node
  int vertex2;  //the other node on the edge
  HeapNode(int key, int vertex1, int vertex2) {
    this->key = key;
    this->vertex1 = vertex1;
    this->vertex2 = vertex2;
  }
  HeapNode():key(0), vertex1(-1), vertex2(-1) {}

} HeapNode;

class MinHeap {
  int capacity;
  int no_elements;
  vector<HeapNode> heapArray;
  inline int parent(int i);
  inline int left(int i);
  inline int right(int i);
  void printHeap();

public:

  vector<int> positionArray;
  MinHeap(int cap);
  void heapify(int i);
  HeapNode* extract_min();
  int insert(HeapNode key);
  bool empty();
  void clean();
  int decrease_key(int position, HeapNode new_key);
  void swap(HeapNode &a, HeapNode &b);

};

#endif /*_MINHEAP_H_*/
