cmake_minimum_required(VERSION 3.16)

project(MultithreadedTaskScheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(task_scheduler
    src/Task.cpp
    src/TaskQueue.cpp
    src/ThreadPool.cpp
    src/Scheduler.cpp
)

target_include_directories(task_scheduler
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(task_scheduler
    PUBLIC
        Threads::Threads
)

add_executable(task_scheduler_demo
    src/main.cpp
)

target_link_libraries(task_scheduler_demo
    PRIVATE
        task_scheduler
)

enable_testing()

add_executable(task_scheduler_tests
    tests/test_scheduler.cpp
)

target_link_libraries(task_scheduler_tests
    PRIVATE
        task_scheduler
)

add_test(
    NAME task_scheduler_tests
    COMMAND task_scheduler_tests
)
