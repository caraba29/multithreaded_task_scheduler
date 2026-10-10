#include "ThreadPool.h"

#include <stdexcept>

ThreadPool::ThreadPool(std::size_t workerCount) {
    if (workerCount == 0) {
        throw std::invalid_argument("workerCount must be greater than 0");
    }

    workers_.reserve(workerCount);

    for (std::size_t i = 0; i < workerCount; ++i) {
        workers_.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::submit(const std::shared_ptr<Task>& task) {
    if (shuttingDown_.load()) {
        throw std::runtime_error("Cannot submit tasks after shutdown");
    }

    taskQueue_.push(task);
}

void ThreadPool::shutdown() {
    bool expected = false;

    if (!shuttingDown_.compare_exchange_strong(expected, true)) {
        return;
    }

    taskQueue_.stop();

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::workerLoop() {
    while (true) {
        auto task = taskQueue_.waitAndPop();

        if (!task) {
            break;
        }

        if (task->status() == TaskStatus::Cancelled) {
            continue;
        }

        task->execute();
    }
}
