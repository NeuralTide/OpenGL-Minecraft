#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <boost/lockfree/queue.hpp>

#include <thread>
#include <vector>
#include <atomic>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void QueueJob(const std::function<void()>& job);

private:
    void Worker();

    struct Job {
        std::function<void()> func;
        Job() = default;
        Job(const std::function<void()>& f) : func(f) {}
        void operator()() {
            if (func) func();
        }
    };

    boost::lockfree::queue<Job*> jobs;
    std::vector<std::thread> workers;
    std::atomic<bool> done;
};

#endif // THREADPOOL_H
