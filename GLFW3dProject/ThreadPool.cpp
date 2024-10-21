#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads)
    : jobs(6000), // Initialize lock-free queue with a capacity
    done(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::Worker, this);
    }
}

ThreadPool::~ThreadPool() {
    done = true;
    for (auto& worker : workers) {
        worker.join();
    }
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
    Job* newJob = new Job(job);
    while (!jobs.push(newJob)) { // Try to push job until successful
        std::this_thread::yield(); // Yield the CPU to other threads
    }
}

void ThreadPool::Worker() {
    Job* job;
    while (!done) {
        if (jobs.pop(job)) {
            (*job)();
            delete job; // Clean up after job execution
        }
        else {
            std::this_thread::yield(); // Yield the CPU to other threads
        }
    }
}
