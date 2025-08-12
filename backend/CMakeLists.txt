cmake_minimum_required(VERSION 3.25)
project(todo_app VERSION 1.0.6 LANGUAGES CXX)

option(BUILD_TESTING "Build unit tests" OFF)

# ────────────────┤ Verze aplikace (single source of truth) ├───────────────
# If APP_VERSION is not provided (CI), default to PROJECT_VERSION-SNAPSHOT for local/dev.
if (DEFINED APP_VERSION)
    # keep as provided by CLI/CI
else ()
    set(APP_VERSION "${PROJECT_VERSION}-SNAPSHOT")
endif ()

# ────────────────┤ Globální konstanty ├───────────────
set(CPP_STANDARD 23)
set(CMAKE_CXX_STANDARD ${CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_NAME todo_core)
set(EXECUTABLE_NAME todo-server)
set(TEST_EXECUTABLE_NAME unit-tests)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# ────────────────┤ Optimalizace ├───────────────
function(optimize_build_settings)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        if (MSVC)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od" PARENT_SCOPE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" PARENT_SCOPE)
            set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL" PARENT_SCOPE)
        else ()
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1" PARENT_SCOPE)
        endif ()
        set(BUILD_SHARED_LIBS OFF PARENT_SCOPE)
    endif ()
endfunction()
optimize_build_settings()

# ────────────────┤ Git metadata (best-effort) ├───────────────
find_package(Git QUIET)

# Accept -DGIT_SHA / -DGIT_TAG (or env vars) before falling back to git commands
if (DEFINED GIT_SHA AND NOT GIT_SHA STREQUAL "")
    # use provided GIT_SHA
elseif (DEFINED ENV{VCS_REF} AND NOT "$ENV{VCS_REF}" STREQUAL "")
    set(GIT_SHA "$ENV{VCS_REF}")
elseif (Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_SHA
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
else ()
    set(GIT_SHA "")
endif ()

if (DEFINED GIT_TAG AND NOT GIT_TAG STREQUAL "")
    # use provided GIT_TAG
elseif (DEFINED ENV{GIT_TAG} AND NOT "$ENV{GIT_TAG}" STREQUAL "")
    set(GIT_TAG "$ENV{GIT_TAG}")
elseif (Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_TAG
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
else ()
    set(GIT_TAG "")
endif ()

# Build-time facts
string(TIMESTAMP BUILD_TIME "%Y-%m-%dT%H:%M:%SZ" UTC)
set(CXX_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
set(CXX_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")

# ────────────────┤ Generate: build_info.hpp ├───────────────
# Template: include/todo/build/build_info.hpp.in
# Placeholders: @APP_VERSION@, @GIT_TAG@, @GIT_SHA@, @BUILD_TIME@,
# @CMAKE_BUILD_TYPE@, @CXX_COMPILER_ID@, @CXX_COMPILER_VERSION@, @CMAKE_SYSTEM_NAME@, @CMAKE_SYSTEM_PROCESSOR@
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/todo/build)
configure_file(
        ${CMAKE_SOURCE_DIR}/include/todo/build/build_info.hpp.in
        ${CMAKE_BINARY_DIR}/generated/todo/build/build_info.hpp
        @ONLY
)

# ────────────────┤ FetchContent - závislosti ├───────────────
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake)

# ────────────────┤ Globální fallback include paths ├───────────────
# (Safety net to ensure generated and public include/ are always visible)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/generated)

# ────────────────┤ Soubory projektu ├───────────────
if (EXISTS "${SRC_DIR}")
    file(GLOB_RECURSE ALL_SRC_FILES "${SRC_DIR}/**/*.cpp")
    list(FILTER ALL_SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")
endif ()

# Dummy target to force FetchContent dependencies (for Docker deps layer)
if (NOT EXISTS "${SRC_DIR}/CMakeLists.txt" AND NOT ALL_SRC_FILES)
    add_library(dummy_dep STATIC EXCLUDE_FROM_ALL src/dummy.cpp)
    target_link_libraries(dummy_dep
            PUBLIC
            nlohmann_json::nlohmann_json
            Crow::Crow
            asio::asio
    )
endif ()

# ────────────────┤ Core knihovna ├───────────────
if (ALL_SRC_FILES)
    add_library(${LIB_NAME} STATIC ${ALL_SRC_FILES})

    # Include paths FIRST (so PCH sees them)
    target_include_directories(${LIB_NAME}
            PUBLIC
            ${SRC_DIR}
            ${ASIO_INCLUDE_DIR}
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_BINARY_DIR}/generated   # <- build_info.hpp
    )

    # PCH after includes
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
endif ()

# ────────────────┤ Hlavní aplikace ├───────────────
if (EXISTS "${SRC_DIR}/main.cpp")
    add_executable(${EXECUTABLE_NAME} ${SRC_DIR}/main.cpp)
    if (TARGET ${LIB_NAME})
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIB_NAME})
    endif ()
    # App may include build_info.hpp or public headers directly
    target_include_directories(${EXECUTABLE_NAME}
            PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_BINARY_DIR}/generated
    )
endif ()

# Výstupní adresář pro debug executables na Windows
if (WIN32 AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(RUNTIME_DEBUG_BIN ${CMAKE_BINARY_DIR}/bin)
    if (TARGET ${EXECUTABLE_NAME})
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
    endif ()
endif ()

# ────────────────┤ Testy ├───────────────
if (BUILD_TESTING AND EXISTS "${TESTS_DIR}")
    include(GoogleTest)
    enable_testing()
    file(GLOB_RECURSE ALL_TEST_FILES "${TESTS_DIR}/**/*_test.cpp")

    if (ALL_TEST_FILES AND TARGET ${LIB_NAME})
        add_executable(${TEST_EXECUTABLE_NAME} ${ALL_TEST_FILES})
        target_include_directories(${TEST_EXECUTABLE_NAME} PRIVATE ${TESTS_DIR} ${CMAKE_BINARY_DIR}/generated)
        target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE ${LIB_NAME} gtest_main gtest)

        if (WIN32 AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
        endif ()

        gtest_discover_tests(${TEST_EXECUTABLE_NAME}
                WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_EXECUTABLE_NAME}>"
                PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_EXECUTABLE_NAME}>"
        )
    endif ()
endif ()

# ────────────────┤ Souhrn (diagnostika) ├───────────────
message(STATUS "========================================")
message(STATUS " Project:         ${PROJECT_NAME}")
message(STATUS " Version:         ${PROJECT_VERSION}")
message(STATUS " APP_VERSION:     ${APP_VERSION}")
message(STATUS " Git tag:         ${GIT_TAG}")
message(STATUS " Git SHA:         ${GIT_SHA}")
message(STATUS " Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS " Compiler:        ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " System:          ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS " Generated hdr:   ${CMAKE_BINARY_DIR}/generated/todo/build/build_info.hpp")
message(STATUS "========================================")
