#ifndef BARRIER_H_
#define BARRIER_H_

#include <iostream>
#include <mutex>
#include <condition_variable>

class Barrier {
public:
    Barrier(uint64_t parties);
    void await();

private:
    std::condition_variable cv;
    std::mutex lock;
    uint64_t total_threads;
    uint64_t current_waiting_threads;
    uint64_t resetting_thread_count;
};

#endif
