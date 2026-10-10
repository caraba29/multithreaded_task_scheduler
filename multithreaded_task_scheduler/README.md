# Multithreaded Task Scheduler

A small C++17 project that executes tasks using a pool of worker threads.

I built it to practice concurrency, synchronization and resource management in C++.

## Features

- submit tasks with low, medium or high priority
- execute multiple tasks concurrently
- thread-safe priority queue
- worker thread pool
- cancel tasks that have not started yet
- track task status
- record task execution time
- store error messages for failed tasks
- graceful shutdown
- basic tests

## Concepts Used

- `std::thread`
- `std::mutex`
- `std::condition_variable`
- `std::atomic`
- `std::priority_queue`
- smart pointers
- RAII
- exception handling
- CMake

## Project Structure

```text
include/
├── Task.h
├── TaskQueue.h
├── ThreadPool.h
└── Scheduler.h

src/
├── Task.cpp
├── TaskQueue.cpp
├── ThreadPool.cpp
├── Scheduler.cpp
└── main.cpp

tests/
└── test_scheduler.cpp
```

## How It Works

Tasks are added to a thread-safe priority queue. A fixed number of worker threads wait for available work and execute tasks as they are received.

Higher-priority tasks are selected before lower-priority tasks when multiple tasks are waiting in the queue.

Each task keeps track of its own status:

```text
Queued
Running
Completed
Cancelled
Failed
```

A task can be cancelled while it is still waiting in the queue. If a task throws an exception while running, it is marked as failed and the error message is stored.

## Build

From the project directory:

```bash
cmake -S . -B build
cmake --build build
```

## Run

On Windows with a typical CMake build:

```powershell
.\build\Debug\task_scheduler_demo.exe
```

Depending on the generator you use, the executable may instead be directly inside the `build` folder.

## Run Tests

```bash
ctest --test-dir build --output-on-failure
```

## Example

The demo program creates tasks with different priorities, runs them across multiple worker threads, cancels one queued task and prints a summary when execution finishes.
