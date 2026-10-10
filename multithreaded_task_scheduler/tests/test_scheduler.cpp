#include "Scheduler.h"

#include <cassert>
#include <chrono>
#include <stdexcept>
#include <thread>

int main() {
    {
        Scheduler scheduler(2);

        const int id = scheduler.submitTask(
            "successful task",
            TaskPriority::Medium,
            [] {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        );

        scheduler.shutdown();

        assert(scheduler.getTaskStatus(id) == TaskStatus::Completed);
    }

    {
        Scheduler scheduler(1);

        const int id = scheduler.submitTask(
            "failing task",
            TaskPriority::High,
            [] {
                throw std::runtime_error("failure");
            }
        );

        scheduler.shutdown();

        assert(scheduler.getTaskStatus(id) == TaskStatus::Failed);
        assert(!scheduler.getTask(id)->errorMessage().empty());
    }

    {
        Scheduler scheduler(1);

        const int first = scheduler.submitTask(
            "slow task",
            TaskPriority::High,
            [] {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        );

        const int second = scheduler.submitTask(
            "task to cancel",
            TaskPriority::Low,
            [] {}
        );

        const bool cancelled = scheduler.cancelTask(second);

        scheduler.shutdown();

        assert(cancelled);
        assert(scheduler.getTaskStatus(second) == TaskStatus::Cancelled);
        assert(
            scheduler.getTaskStatus(first) == TaskStatus::Completed ||
            scheduler.getTaskStatus(first) == TaskStatus::Running
        );
    }

    return 0;
}
