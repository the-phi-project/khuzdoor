# CONFIG
cmake_minimum_required(VERSION 3.24...4.1.2)
project(KHUZDOOR
    VERSION 0.0.1
    LANGUAGES C CXX
)

if(APPLE)
    execute_process(
        COMMAND xcrun --sdk macosx --show-sdk-path
        OUTPUT_VARIABLE MACOS_SDK_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CMAKE_OSX_SYSROOT "${MACOS_SDK_PATH}")
    message(STATUS "macOS SDK Path: ${CMAKE_OSX_SYSROOT}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
#

# VARIABLES
set(KHUZDOOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)

file(GLOB_RECURSE KHUZDOOR_SRC CONFIGURE_DEPENDS ${KHUZDOOR_SRC_DIR}/*.cpp ${KHUZDOOR_SRC_DIR}/*.c)
#

# EXTERNAL LIBRARIES
find_package(ZLIB REQUIRED)

include(FetchContent)


FetchContent_Declare(sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
    GIT_BRANCH     master
)
set(SODIUM_DISABLE_TESTS ON CACHE BOOL "Disable tests")

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(termcolor
    GIT_REPOSITORY https://github.com/TrynaThinkOf1/termcolor-cmake
    GIT_BRANCH     master
)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG        v3.3.1
)

FetchContent_MakeAvailable(sodium json termcolor cxxopts)
#

# EXECUTABLE
add_executable(khuzdoor ${KHUZDOOR_SRC})

target_include_directories(khuzdoor
    BEFORE PRIVATE
        ${ZLIB_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(khuzdoor
    PRIVATE
        ZLIB::ZLIB
        sodium
        nlohmann_json::nlohmann_json
        termcolor
        cxxopts
)

target_compile_options(khuzdoor
    PRIVATE
        -Wall
        -Wextra
        -Wno-unused-function
        -Wno-unused-parameter
        -Wno-return-type
        -Wno-comment
        -Wno-backslash-newline-escape
        $<$<CONFIG:Release>:-O1>
)

add_compile_definitions(
    KHUZDOOR_VERSION="${CMAKE_PROJECT_VERSION}"
)
#

# POST-CREATE
add_custom_command(TARGET khuzdoor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/_build/compile_commands.json
    COMMENT "Copying compile_commands.json to _build/"
)
#