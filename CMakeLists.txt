cmake_minimum_required(VERSION 3.25)
project(todo_app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Kód aplikace (jádro logiky)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_library(core ${SRC_FILES})
target_include_directories(core PUBLIC src)

# Spustitelná aplikace
add_executable(app src/main.cpp)
target_link_libraries(app core)

# Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

# Testy – pozor, NElinkuj gtest_main, pokud máš vlastní main()
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(tests ${TEST_FILES})
target_link_libraries(tests core gtest)

include(GoogleTest)
enable_testing()
gtest_discover_tests(tests)
