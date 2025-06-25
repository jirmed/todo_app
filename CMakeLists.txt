cmake_minimum_required(VERSION 3.25)

project(todo_app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimalizace podle kompileru a build typu
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")  # Paralelní build pro MSVC
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")
    endif()
    set(BUILD_SHARED_LIBS ON)
endif()

include(FetchContent)

# Cachování závislostí
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/_deps" CACHE PATH "FetchContent base directory")

# 📦 nlohmann::json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# 📦 Asio (standalone)
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-28-1
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(asio)

# Nastavení ASIO_INCLUDE_DIR pro FindAsio.cmake modul v Crow
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)
set(ASIO_FOUND TRUE)

# Vytvoření asio::asio targetu pro případ, že ho Crow nenalezne
if(NOT TARGET asio::asio)
    add_library(asio::asio INTERFACE IMPORTED)
    target_include_directories(asio::asio INTERFACE ${ASIO_INCLUDE_DIR})
    target_compile_definitions(asio::asio INTERFACE ASIO_STANDALONE)
    find_package(Threads REQUIRED)
    target_link_libraries(asio::asio INTERFACE Threads::Threads)
endif()

# 📦 Crow framework
FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/crow.git
        GIT_TAG v1.2.1.2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Nastavení proměnných pro Crow před jeho načtením
set(CROW_USE_BOOST OFF CACHE BOOL "Use Boost instead of standalone asio" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "Build Crow tests" FORCE)
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "Build Crow examples" FORCE)

FetchContent_MakeAvailable(crow)

# 📦 GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "Install GoogleTest" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Build GoogleMock" FORCE)
FetchContent_MakeAvailable(googletest)

# 📚 Core knihovna
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h)
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")
add_library(core ${SRC_FILES})

# Precompiled headers pro rychlejší kompilaci
target_precompile_headers(core
    PUBLIC
    <iostream>
    <string>
    <vector>
    <memory>
    <unordered_map>
    <map>
    <algorithm>
    <nlohmann/json.hpp>
)

target_include_directories(core
        PUBLIC
        src
        ${ASIO_INCLUDE_DIR}
)

target_link_libraries(core
        PUBLIC
        nlohmann_json::nlohmann_json
        Crow::Crow
)

target_compile_definitions(core
        PUBLIC
        ASIO_STANDALONE
)

# Linkování Windows specifických knihoven
if(WIN32)
    target_link_libraries(core
            PUBLIC
            ws2_32
            mswsock
    )
endif()

# 🔧 Hlavní aplikace
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE core)

# 🧪 Testy
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(tests ${TEST_FILES})
target_link_libraries(tests
        PRIVATE
        core
        gtest_main
)

# Povolení testování
include(GoogleTest)
enable_testing()
gtest_discover_tests(tests)