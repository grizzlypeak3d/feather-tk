cmake_minimum_required(VERSION 3.31)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0114 NEW)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

file(READ "lib/ftk/Core/Version.h" VERSION_H)
string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${VERSION_H})
set(ftk_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${VERSION_H})
set(ftk_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${VERSION_H})
set(ftk_VERSION_PATCH ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_DEV \"([0-9a-z.-]*)\"" _ ${VERSION_H})
set(ftk_VERSION_DEV ${CMAKE_MATCH_1})
# Composed here rather than read: the header builds it from the macros
# above, so there is no literal in the file to match. Keep this in step
# with the way the header spells it.
set(ftk_VERSION_FULL
    "${ftk_VERSION_MAJOR}.${ftk_VERSION_MINOR}.${ftk_VERSION_PATCH}${ftk_VERSION_DEV}")

project(
    feather-tk
    VERSION ${ftk_VERSION_MAJOR}.${ftk_VERSION_MINOR}.${ftk_VERSION_PATCH}
    LANGUAGES CXX C)

#-------------------------------------------------------------------------------
# Options

set(ftk_API "GL_4_1" CACHE STRING "Graphics API (GL_4_1, GL_4_1_Debug, GLES_3)")
# The build runs ftk-resource to embed the resources, so a cross build --
# Emscripten, say -- cannot use the one it is compiling. Point this at a
# native build's executable instead.
set(ftk_RESOURCE "" CACHE FILEPATH "A native ftk-resource executable, for cross-compiling")
if(ftk_RESOURCE)
    set(ftk_RESOURCE_COMMAND ${ftk_RESOURCE})
else()
    set(ftk_RESOURCE_COMMAND ftk-resource)
endif()
# The pan-CJK font is 16MB, most of the size of a web build, and the
# web build cannot compose CJK input anyway.
set(ftk_CJK_DEFAULT ON)
if(EMSCRIPTEN)
    set(ftk_CJK_DEFAULT OFF)
endif()
set(ftk_CJK ${ftk_CJK_DEFAULT} CACHE BOOL "Bundle the pan-CJK fallback font")
set(ftk_SDL2 OFF CACHE BOOL "Enable support for SDL2")
set(ftk_SDL3 ON CACHE BOOL "Enable support for SDL3")
set(ftk_nfd_DEFAULT OFF)
if(WIN32 OR APPLE)
    set(ftk_nfd_DEFAULT ON)
endif()
set(ftk_nfd ${ftk_nfd_DEFAULT} CACHE BOOL "Enable support for native file dialogs")
set(ftk_PYTHON OFF CACHE BOOL "Enable support for Python")
set(ftk_TESTS ON CACHE BOOL "Enable tests")
set(ftk_TESTS_NO_GL OFF CACHE BOOL "Run only the tests that do not need OpenGL")
set(ftk_EXAMPLES ON CACHE BOOL "Enable examples")
if(APPLE)
    set(ftk_IGNORE_PREFIX_PATH_DEFAULT /opt/homebrew)
endif()
set(ftk_IGNORE_PREFIX_PATH ${ftk_IGNORE_PREFIX_PATH_DEFAULT} CACHE STRING "Ignore the given prefix path")
set(ftk_GCOV OFF CACHE BOOL "Enable gcov code coverage")
set(ftk_GPROF OFF CACHE BOOL "Enable gprof code profiling")
set(ftk_SANITIZE "" CACHE STRING "Enable sanitizers (comma-separated, e.g. address,undefined)")

#-------------------------------------------------------------------------------
# Internal configuration

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif()

if(NOT CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(ftk_IGNORE_PREFIX_PATH)
    set(CMAKE_IGNORE_PREFIX_PATH ${ftk_IGNORE_PREFIX_PATH})
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
endif()

if(APPLE)
    set(CMAKE_FIND_FRAMEWORK LAST)
endif()

if(WIN32)
elseif(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    # The freshly built libraries have to come before the installed
    # copies, or a binary run from the build tree quietly loads the
    # stale install -- so they are gathered into one directory that
    # leads the search path. The install prefix entry cannot simply be
    # dropped: CMake adds it back for the prebuilt dependencies, and
    # after the build tree's own directories only because they are
    # listed here first.
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_BUILD_RPATH
        "${CMAKE_BINARY_DIR}/lib"
        "${CMAKE_INSTALL_PREFIX}/lib")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

if(ftk_TESTS)
    set(CTEST_OUTPUT_ON_FAILURE ON)
    enable_testing()
endif()

if(ftk_GCOV)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-update=atomic")# -fkeep-inline-functions")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-update=atomic")# -fkeep-inline-functions")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -fprofile-update=atomic")
endif()
if(ftk_GPROF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()
if(ftk_SANITIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${ftk_SANITIZE} -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=${ftk_SANITIZE} -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${ftk_SANITIZE}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=${ftk_SANITIZE}")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-------------------------------------------------------------------------------
# Dependencies

find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)
find_package(lunasvg REQUIRED)

if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_3")
    set(ftk_GLAD_DIR ${PROJECT_SOURCE_DIR}/deps/glad_${ftk_API})
    add_subdirectory(deps/glad_${ftk_API})
    if(EMSCRIPTEN)
        # SDL and GL come from Emscripten itself: the SDL port supplies the
        # headers and the implementation, and the GL calls land on WebGL.
        # The compile option is what adds the port's include path, so it is
        # global rather than a link detail.
        # Exceptions are part of how this code base reports errors, and
        # Emscripten turns them off unless asked: without this the first
        # throw aborts the program.
        add_compile_options("-sUSE_SDL=2" "-fwasm-exceptions")
        add_link_options(
            "-sUSE_SDL=2"
            "-fwasm-exceptions"
            "-sSUPPORT_LONGJMP=wasm"
            "-sFULL_ES3=1"
            "-sMIN_WEBGL_VERSION=2"
            "-sMAX_WEBGL_VERSION=2"
            "-sALLOW_MEMORY_GROWTH=1")
    else()
        find_package(OpenGL REQUIRED)
        if(ftk_SDL2)
            find_package(SDL2 REQUIRED)
        elseif(ftk_SDL3)
            find_package(SDL3 REQUIRED)
        endif()
    endif()
endif()

if(ftk_nfd)
    find_package(nfd)
endif()

if(ftk_PYTHON)
    find_package(Python3 REQUIRED Interpreter Development)
    find_package(pybind11 REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Subdirectories

include(cmake/ftkEmbedText.cmake)
include(cmake/ftkIconResources.cmake)

add_subdirectory(bin/ftk-resource)
add_subdirectory(lib/ftk)
if(ftk_TESTS)
    add_subdirectory(tests)
endif()
if(ftk_EXAMPLES)
    add_subdirectory(examples)
endif()
add_subdirectory(etc/Legal)

include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR include)
configure_package_config_file(
    ftkConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ftkConfig.cmake
    INSTALL_DESTINATION lib/cmake/ftk
    PATH_VARS INCLUDE_INSTALL_DIR)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ftkConfig.cmake
        cmake/ftkEmbedText.cmake
        cmake/ftkIconResources.cmake
        cmake/ftkIconResources.h.in
        cmake/ftkIconResources.cpp.in
    DESTINATION lib/cmake/ftk
    COMPONENT dev)
