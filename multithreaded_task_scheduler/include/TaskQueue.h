#pragma once

#include "Task.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

class TaskQueue {
public:
    TaskQueue() = default;

    void push(const std::shared_ptr<Task>& task);
    std::shared_ptr<Task> waitAndPop();

    void stop();
    bool empty() const;

private:
    struct TaskCompare {
        bool operator()(
            const std::shared_ptr<Task>& left,
            const std::shared_ptr<Task>& right
        ) const {
            return static_cast<int>(left->priority()) <
                   static_cast<int>(right->priority());
        }
    };

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::priority_queue<
        std::shared_ptr<Task>,
        std::vector<std::shared_ptr<Task>>,
        TaskCompare
    > queue_;

    bool stopping_{false};
};
