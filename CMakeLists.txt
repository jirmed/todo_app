cmake_minimum_required(VERSION 3.31)
project(todo_app)

set(CMAKE_CXX_STANDARD 26)

add_executable(todo_app
        main.cpp
        TaskManager.cpp
        TaskManager.h
        Task.h
)
