cmake_minimum_required(VERSION 3.31)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0114 NEW)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(ftk_VERSION_MAJOR "0")
set(ftk_VERSION_MINOR "6")
set(ftk_VERSION_PATCH "0")
set(ftk_VERSION ${ftk_VERSION_MAJOR}.${ftk_VERSION_MINOR}.${ftk_VERSION_PATCH})
add_definitions(-Dftk_VERSION_MAJOR=${ftk_VERSION_MAJOR})
add_definitions(-Dftk_VERSION_MINOR=${ftk_VERSION_MINOR})
add_definitions(-Dftk_VERSION_PATCH=${ftk_VERSION_PATCH})
add_definitions(-Dftk_VERSION="${ftk_VERSION}")

project(
    feather-tk
    VERSION ${ftk_VERSION}
    LANGUAGES CXX C)

#-------------------------------------------------------------------------------
# Options

set(ftk_UI_LIB ON CACHE BOOL "Build the user interface library")
set(ftk_API "GL_4_1" CACHE STRING "Graphics API (GL_4_1, GL_4_1_Debug, GLES_2)")
set(ftk_nfd_DEFAULT OFF)
if(WIN32 OR APPLE)
    set(ftk_nfd_DEFAULT ON)
endif()
set(ftk_nfd ${ftk_nfd_DEFAULT} CACHE BOOL "Enable support for native file dialogs")
set(ftk_PYTHON OFF CACHE BOOL "Enable support for Python")
set(ftk_TESTS ON CACHE BOOL "Enable tests")
set(ftk_EXAMPLES ON CACHE BOOL "Enable examples")
if(APPLE)
    set(ftk_IGNORE_PREFIX_PATH_DEFAULT /opt/homebrew)
endif()
set(ftk_IGNORE_PREFIX_PATH ${ftk_IGNORE_PREFIX_PATH_DEFAULT} CACHE STRING "Ignore the given prefix path")
set(ftk_BUILD "default" CACHE STRING "Build configuration: 'default', 'SuperBuild', 'vcpkg'")
set(ftk_GCOV OFF CACHE BOOL "Enable gcov code coverage")
set(ftk_GPROF OFF CACHE BOOL "Enable gprof code profiling")

#-------------------------------------------------------------------------------
# Configuration

if(ftk_BUILD STREQUAL "SuperBuild")
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
endif()

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
    # \bug Should this be set automatically?
    if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
        set(CMAKE_BUILD_RPATH "@executable_path/../../../install-Debug/lib")
    elseif (CMAKE_BUILD_TYPE MATCHES "^RelWithDebInfo$")
        set(CMAKE_BUILD_RPATH "@executable_path/../../../install-RelWithDebInfo/lib")
    elseif (CMAKE_BUILD_TYPE MATCHES "^Release$")
        set(CMAKE_BUILD_RPATH "@executable_path/../../../install-Release/lib")
    endif()
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

if(ftk_UI_LIB)
    add_definitions(-DFTK_UI_LIB)
endif()

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    add_definitions(-DFTK_ASSERT)
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

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(lib)
if(ftk_TESTS)
    include_directories(tests)
endif()

#-------------------------------------------------------------------------------
# Dependencies

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)
find_package(lunasvg REQUIRED)

if(ftk_UI_LIB)
    if ("${ftk_API}" STREQUAL "GL_4_1")
        include_directories(${PROJECT_SOURCE_DIR}/deps/glad_GL_4_1)
    elseif ("${ftk_API}" STREQUAL "GL_4_1_Debug")
        include_directories(${PROJECT_SOURCE_DIR}/deps/glad_GL_4_1_Debug)
    elseif ("${ftk_API}" STREQUAL "GLES_2")
        include_directories(${PROJECT_SOURCE_DIR}/deps/glad_GLES_2)
    endif()
    if ("${ftk_API}" STREQUAL "GL_4_1" OR
        "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
        "${ftk_API}" STREQUAL "GLES_2")
        find_package(OpenGL REQUIRED)
        find_package(SDL2 REQUIRED)
        add_subdirectory(deps/glad_${ftk_API})
    endif()
    add_definitions(-DFTK_API_${ftk_API})
    if(${ftk_API} STREQUAL "GL_4_1_Debug")
        add_definitions(-DFTK_API_GL_4_1)
    endif()

    if(ftk_nfd)
        find_package(nfd)
        if(nfd_FOUND)
            add_definitions(-DFTK_NFD)
        endif()
    endif()

    if(ftk_PYTHON)
        include_directories(python/lib)
        if(ftk_TESTS)
            include_directories(python/tests)
        endif()
        add_definitions(-DFTK_PYTHON)
        find_package(Python3 REQUIRED Interpreter Development)
        find_package(pybind11 REQUIRED)
    endif()
endif()

#-------------------------------------------------------------------------------
# Subdirectories

add_subdirectory(bin/ftk-resource)
add_subdirectory(lib/ftk)
if(ftk_UI_LIB AND ftk_PYTHON)
    add_subdirectory(python)
endif()
if(ftk_TESTS)
    add_subdirectory(tests)
endif()
if(ftk_UI_LIB AND ftk_EXAMPLES)
    add_subdirectory(examples)
endif()

install(
    FILES LICENSE.txt
    DESTINATION share/ftk)

if(ftk_BUILD STREQUAL "SuperBuild")
    install(
        FILES cmake/Modules/FindZLIB.cmake
        DESTINATION share/ftk)
    install(
        FILES cmake/Modules/FindPNG.cmake
        DESTINATION share/ftk)
    if(ftk_UI_LIB)
        install(
            FILES cmake/Modules/Findnfd.cmake
            DESTINATION share/ftk)
    endif()
endif()
include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR include)
configure_package_config_file(
    ftkConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ftkConfig.cmake
    INSTALL_DESTINATION share/ftk
    PATH_VARS INCLUDE_INSTALL_DIR)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/ftkConfig.cmake
    DESTINATION share/ftk)
