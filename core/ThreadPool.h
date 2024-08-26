#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueueTask(const std::function<void()> &task);

private:
    std::vector<std::thread> workers;

    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

#endif // THREADPOOL_H