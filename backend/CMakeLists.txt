cmake_minimum_required(VERSION 3.25)
project(todo_app)

# ─────────────────────────────┤ Konstanty a proměnné ├────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_NAME todo_core)
set(EXECUTABLE_NAME todo_app_exe)
set(TEST_EXECUTABLE_NAME unit_tests)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# ────────────────────────────────┤ Optimalizace ├────────────────────────────────
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL")
    else ()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")
    endif ()
    set(BUILD_SHARED_LIBS OFF)
endif ()

# ────────────────────────────────┤ FetchContent ├───────────────────────────────
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

# ────────────────────────────────┤ Core knihovna ├──────────────────────────────
file(GLOB_RECURSE SRC_FILES
        "${SRC_DIR}/**/*.cpp"
)
list(APPEND SRC_FILES "${SRC_DIR}/presentation/http/RestServer.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")

add_library(${LIB_NAME} STATIC ${SRC_FILES})
target_precompile_headers(${LIB_NAME}
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
target_include_directories(${LIB_NAME}
        PUBLIC
        ${SRC_DIR}
        ${ASIO_INCLUDE_DIR}
)
target_link_libraries(${LIB_NAME}
        PUBLIC
        nlohmann_json::nlohmann_json
        Crow::Crow
)
target_compile_definitions(${LIB_NAME}
        PUBLIC
        ASIO_STANDALONE
)
if (WIN32)
    target_link_libraries(${LIB_NAME}
            PUBLIC
            ws2_32
            mswsock
    )
endif ()

# ────────────────────────────────┤ Hlavní aplikace ├────────────────────────────
add_executable(${EXECUTABLE_NAME} ${SRC_DIR}/main.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIB_NAME})

# Výstupní adresář pro debug executables na Windows
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(RUNTIME_DEBUG_BIN ${CMAKE_BINARY_DIR}/bin)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
endif ()

# ────────────────────────────────┤ Testy ├──────────────────────────────────────
include(GoogleTest)
enable_testing()
file(GLOB_RECURSE TEST_FILES "${TESTS_DIR}/**/*_test.cpp")

foreach (TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_include_directories(${TEST_NAME} PRIVATE ${TESTS_DIR})
    target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME} gtest_main gtest)
    if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
    endif ()
    gtest_discover_tests(${TEST_NAME}
            WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>"
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>"
            TEST_PREFIX "${TEST_NAME}."
            TEST_LIST ${TEST_NAME}_TESTS
    )
    set_property(GLOBAL PROPERTY TEST_DISCOVERY_TIMEOUT 60)
endforeach ()

add_executable(${TEST_EXECUTABLE_NAME} ${TEST_FILES})
target_include_directories(${TEST_EXECUTABLE_NAME} PRIVATE ${TESTS_DIR}/unit)
target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE ${LIB_NAME} gtest_main gtest)
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
endif ()