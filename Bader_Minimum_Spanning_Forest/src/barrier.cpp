#include "barrier.h"

Barrier::Barrier(uint64_t total_threads) {
    this->total_threads = total_threads;
    resetting_thread_count = 0;
    current_waiting_threads = 0;
}

void Barrier::await() {

    std::unique_lock<std::mutex> lk(lock);
    uint64_t const current_wait_cycle = resetting_thread_count;
    ++current_waiting_threads;
    if (current_waiting_threads < total_threads) {
        cv.wait(lk,
        // handle spurious notification calls
        [this, current_wait_cycle]() {
            return resetting_thread_count != current_wait_cycle;
        });
    }
    else {
        //checking for overflows
        if(resetting_thread_count == std::numeric_limits<uint64_t>::max()) {
            std::cerr << "resetting_thread_count has reached UINT64_T MAX" << std::endl;
            exit(0);
        }
        // increasing the to allow waiting threads to exit
        ++resetting_thread_count;
        current_waiting_threads = 0;
        cv.notify_all();
    }
}
