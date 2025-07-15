cmake_minimum_required(VERSION 3.25)
project(todo_app)

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
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})

# ────────────────┤ Konfigurace optimalizací ├───────────────
function(optimize_build_settings)
    if (${BUILD_TYPE} STREQUAL "Debug")
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

# ────────────────┤ Načti závislosti ├───────────────
# Závislosti už jsou stažené z předchozí fáze
include(cmake/dependencies.cmake)

# ────────────────┤ Soubory projektu ├───────────────
# Teď můžeme bezpečně hledat soubory
if(EXISTS "${SRC_DIR}")
    file(GLOB_RECURSE ALL_SRC_FILES "${SRC_DIR}/**/*.cpp")
    list(FILTER ALL_SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")
endif()

# ────────────────┤ Core knihovna ├───────────────
if(ALL_SRC_FILES)
    add_library(${LIB_NAME} STATIC ${ALL_SRC_FILES})
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
endif()

# ────────────────┤ Hlavní aplikace ├───────────────
if(EXISTS "${SRC_DIR}/main.cpp")
    add_executable(${EXECUTABLE_NAME} ${SRC_DIR}/main.cpp)
    if(TARGET ${LIB_NAME})
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIB_NAME})
    endif()
endif()

# Výstupní adresář pro debug executables na Windows
if (WIN32 AND ${BUILD_TYPE} STREQUAL "Debug")
    set(RUNTIME_DEBUG_BIN ${CMAKE_BINARY_DIR}/bin)
    if(TARGET ${EXECUTABLE_NAME})
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
    endif()
endif ()

# ────────────────┤ Testy ├───────────────
if(EXISTS "${TESTS_DIR}")
    include(GoogleTest)
    enable_testing()
    file(GLOB_RECURSE ALL_TEST_FILES "${TESTS_DIR}/**/*_test.cpp")
    
    if(ALL_TEST_FILES)
        add_executable(${TEST_EXECUTABLE_NAME} ${ALL_TEST_FILES})
        target_include_directories(${TEST_EXECUTABLE_NAME} PRIVATE ${TESTS_DIR})
        target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE ${LIB_NAME} gtest_main gtest)

        if (WIN32 AND ${BUILD_TYPE} STREQUAL "Debug")
            set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DEBUG_BIN})
        endif ()

        gtest_discover_tests(${TEST_EXECUTABLE_NAME}
                WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_EXECUTABLE_NAME}>"
                PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_EXECUTABLE_NAME}>"
        )
    endif()
endif()