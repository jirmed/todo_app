cmake_minimum_required(VERSION 3.25)
project(todo_app_deps)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ────────────────┤ FetchContent - pouze závislosti ├───────────────
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/_deps" CACHE PATH "FetchContent base directory")

# nlohmann::json
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# ASIO (standalone)
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-28-1
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(asio)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)
set(ASIO_FOUND TRUE)
if (NOT TARGET asio::asio)
    add_library(asio::asio INTERFACE IMPORTED)
    target_include_directories(asio::asio INTERFACE ${ASIO_INCLUDE_DIR})
    target_compile_definitions(asio::asio INTERFACE ASIO_STANDALONE)
    find_package(Threads REQUIRED)
    target_link_libraries(asio::asio INTERFACE Threads::Threads)
endif ()

# Crow framework
FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/crow.git
        GIT_TAG v1.2.1.2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(CROW_USE_BOOST OFF CACHE BOOL "Use Boost instead of standalone asio" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "Build Crow tests" FORCE)
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "Build Crow examples" FORCE)
FetchContent_MakeAvailable(crow)

# GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "Install GoogleTest" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Build GoogleMock" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Dummy target
add_custom_target(deps_ready)
