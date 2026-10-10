#include "TaskQueue.h"

void TaskQueue::push(const std::shared_ptr<Task>& task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (stopping_) {
            return;
        }

        queue_.push(task);
    }

    condition_.notify_one();
}

std::shared_ptr<Task> TaskQueue::waitAndPop() {
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(lock, [this] {
        return stopping_ || !queue_.empty();
    });

    if (stopping_ && queue_.empty()) {
        return nullptr;
    }

    auto task = queue_.top();
    queue_.pop();
    return task;
}

void TaskQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
    }

    condition_.notify_all();
}

bool TaskQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}
