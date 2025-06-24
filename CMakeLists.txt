cmake_minimum_required(VERSION 3.25)

# Nastavení politiky kvůli Boost
cmake_policy(SET CMP0167 NEW)

project(todo_app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# 📦 nlohmann::json
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# 📦 Asio (nutné pro Crow)
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-28-1
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(asio)
# 📦 Crow v1.2.1.2


FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/crow.git
        GIT_TAG v1.2.1.2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(crow)

# 📦 GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

# 📦 Boost (předpokládá se nainstalovaný v systému)
find_package(Boost REQUIRED COMPONENTS system)

# 📦 Základní logika aplikace jako knihovna
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_library(core ${SRC_FILES})
target_include_directories(core PUBLIC src)

# 🔧 Aplikační binárka
add_executable(app src/main.cpp)

target_link_libraries(app
        PRIVATE
        core
        Boost::system
        nlohmann_json::nlohmann_json
)

# Link only on Windows
if(WIN32)
    target_link_libraries(app
            PRIVATE
            ws2_32
            mswsock
    )
endif()

# Link only on Windows

target_include_directories(core
        PUBLIC
        src
        ${crow_SOURCE_DIR}/include
        ${asio_SOURCE_DIR}/asio/include
)

# 🧪 Testy (s tvým vlastním main)
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(tests ${TEST_FILES})
target_link_libraries(tests
        PRIVATE
        core
        gtest
)

include(GoogleTest)
enable_testing()
gtest_discover_tests(tests)
