// Source : http://www.geeksforgeeks.org/prims-algorithm-using-priority_queue-stl/


#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>
#include <mutex>
#include <thread>
#include <atomic>
#include <atomic>
#include <mutex>
#include <iostream>
#include <vector>
#include <assert.h>


using namespace std;


// HeapNode struct is used to store each node in the queue 
struct HeapNode {

	int value;
	int threadID;
	int weight;

};


// Function to implement fine-grained priority queue
class FineGrainedHeap {

private:
	
	int ROOT = 1; // ROOT Node
	int next; // Next value 
	std::vector<HeapNode> heap;	// Vector of heap nodes
	int length; // length of heap node
	std::mutex heapLock; // Heap lock
	std::mutex *mtx; // Locks for nodes


public:

	// Function to initialize values
	void init(int capacity) {

		next = ROOT;
		length=capacity+1;
		heap.resize(capacity+1);
		mtx = new std::mutex[capacity+1];

	}

	// Function to insert in the queue
	void insert(int value, int ID, long long int wt) {


		heapLock.lock();
				
		int child = next++;
		mtx[child].lock();
		heap[child].value = value;
		heap[child].threadID = ID;
		heap[child].weight = wt;

		heapLock.unlock();

		mtx[child].unlock();


		while(child > ROOT) {

			int parent = child/2;
			mtx[parent].lock();
			mtx[child].lock();
			int oldChild = child;

			// If parent is not assigned and current node is child node
			if (heap[parent].threadID == -1 && heap[child].threadID==ID) {
				
				if(heap[child].value < heap[parent].value) {

					std::swap(heap[parent].value, heap[child].value);
					std::swap(heap[parent].threadID, heap[child].threadID);
					std::swap(heap[parent].weight, heap[child].weight);

					child = parent;

				} else {
					heap[child].threadID = -1;
					mtx[parent].unlock();
					mtx[child].unlock();
					return;
				}

			} else if (heap[child].threadID != ID) {
				child = parent;
			}


			mtx[oldChild].unlock();
			mtx[parent].unlock();

		}

		// If it's the first node to be added 
		if(child == ROOT) {
			mtx[ROOT].lock();
			if(heap[ROOT].threadID == ID) {
				heap[ROOT].threadID = -1;
			}
			mtx[ROOT].unlock();
		}


	}
	

	// Function to delete minimum value from queue
	pair<int,int> removeMin()
	{
		int bottom=--next;
		pair<int,int> min;
		min.first = heap[ROOT].weight;
		min.second = heap[ROOT].value;
		heap[ROOT]=heap[bottom];
		if(bottom==ROOT)
		{
		 return min;
		}
		int child=0;
		int parent=ROOT;

		// Sequentially checking
		while(parent<length/2)
		{
			int left=parent*2;
			int right=parent*2+1;
			if(left>=next) {
				return min;
			} else if(right>=next || heap[left].value < heap[right].value) {
				child=left;
			} else {
				child=right;
			}

			if(heap[child].value<heap[parent].value)
			{
				std::swap(heap[child].value, heap[parent].value);
				std::swap(heap[child].threadID, heap[parent].threadID);
				std::swap(heap[child].weight, heap[parent].weight);
				parent=child;
			}
			else { 
				return min; 
			}
		} 
	return min;
	}

};

FineGrainedHeap pq;








