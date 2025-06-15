cmake_minimum_required(VERSION 3.31)
project(todo_app)

set(CMAKE_CXX_STANDARD 23)

add_executable(todo_app
        main.cpp
        Task.cpp
        TaskManager.cpp
)
