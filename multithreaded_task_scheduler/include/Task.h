#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

enum class TaskPriority {
    Low = 0,
    Medium = 1,
    High = 2
};

enum class TaskStatus {
    Queued,
    Running,
    Completed,
    Cancelled,
    Failed
};

class Task {
public:
    using Work = std::function<void()>;

    Task(int id, std::string name, TaskPriority priority, Work work);

    int id() const;
    const std::string& name() const;
    TaskPriority priority() const;
    TaskStatus status() const;

    bool cancel();
    bool isCancelled() const;

    void execute();

    std::chrono::milliseconds executionTime() const;
    std::string errorMessage() const;

private:
    int id_;
    std::string name_;
    TaskPriority priority_;
    Work work_;

    std::atomic<TaskStatus> status_{TaskStatus::Queued};
    std::atomic<bool> cancelled_{false};

    std::chrono::milliseconds executionTime_{0};
    std::string errorMessage_;
};
