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
    # Ponecháme statické linkování pro konzistenci
    set(BUILD_SHARED_LIBS OFF)
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

# 📦 GoogleTest - explicitně nastavíme statické linkování
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "Install GoogleTest" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Build GoogleMock" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 📚 Core knihovna - změníme na statickou
# Rekurzivně najdeme všechny zdrojové soubory v adresáři src
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
)

# Přidáme explicitně chybějící soubor
list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/http/RestServer.cpp")

# Vyloučíme main.cpp ze zdrojových souborů knihovny
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")

add_library(core STATIC ${SRC_FILES})

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src
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
include(GoogleTest)
enable_testing()

# Rekurzivně najdeme všechny testovací soubory
file(GLOB_RECURSE TEST_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/**/*_test.cpp"
)

# Pro každý testovací soubor vytvoříme samostatný spustitelný soubor
foreach(TEST_FILE ${TEST_FILES})
    # Získáme název testu ze jména souboru (např. "InMemoryTaskRepository_test")
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Vytvoříme spustitelný soubor pro daný test
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME}
        PRIVATE
        core
        gtest_main
        gtest
    )

    # Zajištění správného runtime pro Windows
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
        
        # Kopírování potřebných DLL souborů do výstupního adresáře
        add_custom_command(TARGET ${TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:gtest>
            $<TARGET_FILE_DIR:${TEST_NAME}>
            COMMENT "Copying gtest DLL to test directory"
        )
        
        add_custom_command(TARGET ${TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:gtest_main>
            $<TARGET_FILE_DIR:${TEST_NAME}>
            COMMENT "Copying gtest_main DLL to test directory"
        )
    endif()

    # Registrace testů pomocí discover_tests - vráceno na původní
    gtest_discover_tests(${TEST_NAME})
endforeach()

# Vypíše všechny zdrojové soubory - užitečné pro debugging
message(STATUS "Nalezené zdrojové soubory:")
foreach(SRC_FILE ${SRC_FILES})
    message(STATUS "  ${SRC_FILE}")
endforeach()

message(STATUS "Nalezené testovací soubory:")
foreach(TEST_FILE ${TEST_FILES})
    message(STATUS "  ${TEST_FILE}")
endforeach()