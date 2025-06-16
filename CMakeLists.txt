cmake_minimum_required(VERSION 3.31)
project(todo_app)

set(CMAKE_CXX_STANDARD 26)

# Zdrojový soubor aplikace
add_executable(todo_app
        main.cpp
        TaskManager.cpp
)

# GTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

# Testy
add_executable(tests
        tests/TaskManager_test.cpp
        TaskManager.cpp
)

target_link_libraries(tests
        gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
