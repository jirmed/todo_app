
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

include(FetchContent)

# JSON knihovna
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# HTTP server (httplib)
FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.15.3
)
FetchContent_MakeAvailable(httplib)

target_include_directories(core PUBLIC
        ${json_SOURCE_DIR}/include
        ${httplib_SOURCE_DIR}
)

target_link_libraries(app PRIVATE core)

# Google Test
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(tests ${TEST_FILES})
target_link_libraries(tests PRIVATE core gtest)

include(GoogleTest)
enable_testing()
gtest_discover_tests(tests)
