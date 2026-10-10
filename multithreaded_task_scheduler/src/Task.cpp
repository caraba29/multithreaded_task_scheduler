#include "Task.h"

#include <exception>
#include <utility>

Task::Task(int id, std::string name, TaskPriority priority, Work work)
    : id_(id),
      name_(std::move(name)),
      priority_(priority),
      work_(std::move(work)) {}

int Task::id() const {
    return id_;
}

const std::string& Task::name() const {
    return name_;
}

TaskPriority Task::priority() const {
    return priority_;
}

TaskStatus Task::status() const {
    return status_.load();
}

bool Task::cancel() {
    TaskStatus expected = TaskStatus::Queued;

    if (status_.compare_exchange_strong(expected, TaskStatus::Cancelled)) {
        cancelled_.store(true);
        return true;
    }

    return false;
}

bool Task::isCancelled() const {
    return cancelled_.load();
}

void Task::execute() {
    if (isCancelled()) {
        status_.store(TaskStatus::Cancelled);
        return;
    }

    status_.store(TaskStatus::Running);
    const auto start = std::chrono::steady_clock::now();

    try {
        work_();

        if (isCancelled()) {
            status_.store(TaskStatus::Cancelled);
        } else {
            status_.store(TaskStatus::Completed);
        }
    } catch (const std::exception& ex) {
        errorMessage_ = ex.what();
        status_.store(TaskStatus::Failed);
    } catch (...) {
        errorMessage_ = "Unknown error";
        status_.store(TaskStatus::Failed);
    }

    const auto end = std::chrono::steady_clock::now();
    executionTime_ =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::chrono::milliseconds Task::executionTime() const {
    return executionTime_;
}

std::string Task::errorMessage() const {
    return errorMessage_;
}
