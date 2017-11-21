#include "minheap.h"

MinHeap::MinHeap(int cap) {
	capacity = cap;
	no_elements = 0;
	heapArray = vector<HeapNode>(capacity);
	positionArray = vector<int>(capacity);
}

inline int MinHeap::parent(int i) {
	return (i <= 0) ? -1 : (i - 1) / 2;
}

inline int MinHeap::left(int i) {
	return 2 * i + 1;
}

inline int MinHeap::right(int i) {
	return 2 * i + 2;
}

void MinHeap::swap(HeapNode &a, HeapNode &b) {
	HeapNode c = a;
	a = b;
	b = c;
}

void MinHeap::printHeap() {
	for (int i = 0; i < no_elements; ++i)
	{
		cout<<"("<<heapArray[i].key<<","<<heapArray[i].vertex1<<","<<heapArray[i].vertex2<<") -> ";
	}
	cout << endl;

	for (int i = 0; i < no_elements; ++i)
	{
		cout<<"("<<positionArray[i]<<") -> ";
	}
	cout << endl;

}
void MinHeap::heapify(int i) {
	int lf = left(i), rf = right(i), smallest;

	if (lf < no_elements && heapArray[lf].key < heapArray[i].key)
		smallest = lf;
	else
		smallest = i;

	if (rf < no_elements && heapArray[rf].key < heapArray[smallest].key)
		smallest = rf;

	if (smallest != i) {
		positionArray[heapArray[smallest].vertex1] = i;
		positionArray[heapArray[i].vertex1] = smallest;
		swap(heapArray[smallest], heapArray[i]);
		heapify(smallest);
	}
}

HeapNode* MinHeap::extract_min() {
	if (no_elements < 1) {
		cerr<<"heap underflow\n";
		return NULL;
	}

	HeapNode* a = &heapArray[0];	
	positionArray[heapArray[0].vertex1] = -1;
	swap(heapArray[0], heapArray[no_elements - 1]);
	no_elements--;
	heapify(0);
	return a;
}

int MinHeap::insert(HeapNode new_key) {

	if (no_elements >= capacity) {
		cerr<<"HEAP overflow\n";
		return -1;
	}
	no_elements++;
	heapArray[no_elements - 1] = new_key;
	int i = no_elements - 1;

	while (i > 0 && heapArray[parent(i)].key > new_key.key) {
		heapArray[i] = heapArray[parent(i)];
		positionArray[heapArray[i].vertex1] = i;
		i = parent(i);
	}
	heapArray[i] = new_key;
	positionArray[new_key.vertex1] = i;
	return i;
}

int MinHeap::decrease_key(int position, HeapNode new_key) {

	if (heapArray[position].key <= new_key.key)
		return position;

	int i = position;
	while (i > 0 && heapArray[parent(i)].key > new_key.key) {
		heapArray[i] = heapArray[parent(i)];
		positionArray[heapArray[i].vertex1] = i;
		i = parent(i);
	}
	heapArray[i] = new_key;
	positionArray[new_key.vertex1] = i;
	return i;
}

bool MinHeap::empty()
{
	if (no_elements == 0)
		return true;
	else
		return false;
}

void MinHeap::clean()
{
	no_elements = 0;
}