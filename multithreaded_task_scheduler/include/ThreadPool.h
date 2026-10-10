#pragma once

#include "TaskQueue.h"

#include <atomic>
#include <cstddef>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t workerCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(const std::shared_ptr<Task>& task);
    void shutdown();

private:
    void workerLoop();

    TaskQueue taskQueue_;
    std::vector<std::thread> workers_;
    std::atomic<bool> shuttingDown_{false};
};
