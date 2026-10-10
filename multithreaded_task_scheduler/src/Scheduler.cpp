#include "Scheduler.h"

#include <stdexcept>

Scheduler::Scheduler(std::size_t workerCount)
    : threadPool_(workerCount) {}

int Scheduler::submitTask(
    const std::string& name,
    TaskPriority priority,
    Task::Work work
) {
    const int id = nextTaskId_.fetch_add(1);
    auto task = std::make_shared<Task>(id, name, priority, std::move(work));

    {
        std::lock_guard<std::mutex> lock(tasksMutex_);
        tasks_[id] = task;
    }

    threadPool_.submit(task);
    return id;
}

bool Scheduler::cancelTask(int taskId) {
    auto task = getTask(taskId);

    if (!task) {
        return false;
    }

    return task->cancel();
}

TaskStatus Scheduler::getTaskStatus(int taskId) const {
    auto task = getTask(taskId);

    if (!task) {
        throw std::out_of_range("Task not found");
    }

    return task->status();
}

std::shared_ptr<Task> Scheduler::getTask(int taskId) const {
    std::lock_guard<std::mutex> lock(tasksMutex_);
    const auto it = tasks_.find(taskId);

    if (it == tasks_.end()) {
        return nullptr;
    }

    return it->second;
}

std::vector<std::shared_ptr<Task>> Scheduler::getAllTasks() const {
    std::lock_guard<std::mutex> lock(tasksMutex_);

    std::vector<std::shared_ptr<Task>> result;
    result.reserve(tasks_.size());

    for (const auto& [id, task] : tasks_) {
        result.push_back(task);
    }

    return result;
}

void Scheduler::shutdown() {
    threadPool_.shutdown();
}
