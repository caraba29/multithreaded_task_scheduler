#include "Scheduler.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace {

std::string statusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::Queued:
            return "Queued";
        case TaskStatus::Running:
            return "Running";
        case TaskStatus::Completed:
            return "Completed";
        case TaskStatus::Cancelled:
            return "Cancelled";
        case TaskStatus::Failed:
            return "Failed";
    }

    return "Unknown";
}

} // namespace

int main() {
    Scheduler scheduler(3);

    const int firstTask = scheduler.submitTask(
        "High priority calculation",
        TaskPriority::High,
        [] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "High priority task finished\n";
        }
    );

    const int secondTask = scheduler.submitTask(
        "Medium priority task",
        TaskPriority::Medium,
        [] {
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            std::cout << "Medium priority task finished\n";
        }
    );

    const int failingTask = scheduler.submitTask(
        "Failing task",
        TaskPriority::Low,
        [] {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            throw std::runtime_error("Example task failure");
        }
    );

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    scheduler.cancelTask(secondTask);

    scheduler.shutdown();

    std::cout << "\nTask summary\n";
    std::cout << "------------\n";

    for (const auto& task : scheduler.getAllTasks()) {
        std::cout
            << "ID: " << task->id()
            << " | Name: " << task->name()
            << " | Status: " << statusToString(task->status())
            << " | Time: " << task->executionTime().count() << " ms";

        if (!task->errorMessage().empty()) {
            std::cout << " | Error: " << task->errorMessage();
        }

        std::cout << '\n';
    }

    return 0;
}
