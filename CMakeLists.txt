# CONFIG
cmake_minimum_required(VERSION 3.24...4.1.2)
project(KHUZDOOR
    VERSION 0.0.3
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

find_package(ImageMagick COMPONENTS Magick++)

##

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
    GIT_REPOSITORY https://github.com/TrynaThinkOf1/termcolor-cmake.git
    GIT_BRANCH     master
)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.3.1
)

FetchContent_MakeAvailable(sodium json termcolor cxxopts)

##

include(ExternalProject)

ExternalProject_Add(
    libmagic_ep
    GIT_REPOSITORY https://github.com/file/file.git
    GIT_TAG FILE5_47
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --disable-shared --enable-static --prefix=<INSTALL_DIR>
    BUILD_IN_SOURCE 1
    UPDATE_COMMAND ""
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    PATCH_COMMAND autoreconf -fiv
)

ExternalProject_Get_Property(libmagic_ep INSTALL_DIR)

set(LIBMAGIC_INCLUDE_DIR ${INSTALL_DIR}/include)
set(LIBMAGIC_LIBRARY ${INSTALL_DIR}/lib/libmagic.a)
#

# EXECUTABLE
add_executable(khuzdoor ${KHUZDOOR_SRC})

add_dependencies(khuzdoor libmagic_ep)

target_include_directories(khuzdoor
    BEFORE PRIVATE
        ${ZLIB_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${LIBMAGIC_INCLUDE_DIR}
        ${ImageMagick_INCLUDE_DIRS}
)

target_link_libraries(khuzdoor
    PRIVATE
        ZLIB::ZLIB
        sodium
        nlohmann_json::nlohmann_json
        termcolor
        cxxopts
        ${LIBMAGIC_LIBRARY}
        ${ImageMagick_LIBRARIES}
        bz2
        lzma
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
    HAVE_LIBMAGIC
    MAGIC_DB_PATH="${MAGIC_DB_PATH}"
)
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
#

# POST-CREATE
add_custom_command(TARGET khuzdoor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/_build/compile_commands.json
    COMMENT "Copying compile_commands.json to _build/"
)
#