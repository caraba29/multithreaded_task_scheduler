#pragma once

#include "ThreadPool.h"

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class Scheduler {
public:
    explicit Scheduler(std::size_t workerCount);

    int submitTask(
        const std::string& name,
        TaskPriority priority,
        Task::Work work
    );

    bool cancelTask(int taskId);
    TaskStatus getTaskStatus(int taskId) const;
    std::shared_ptr<Task> getTask(int taskId) const;
    std::vector<std::shared_ptr<Task>> getAllTasks() const;

    void shutdown();

private:
    mutable std::mutex tasksMutex_;
    std::unordered_map<int, std::shared_ptr<Task>> tasks_;

    std::atomic<int> nextTaskId_{1};
    ThreadPool threadPool_;
};
