# Build configuration: what to build, in one file for every platform.
#
# Used with "cmake -C", which reads it before the project, so everything here
# lands in the cache as a default. Plain CMake rather than shell, so the same
# file serves Linux, macOS, Windows and CI -- a build option is written once
# here instead of once per platform script and again per continuous
# integration job.
#
# Personal settings do not belong here. Put them in etc/Config/local.cmake,
# which is not tracked: it is included first, and a plain cache set does not
# overwrite a value that is already there, so anything it sets wins over the
# defaults below.
#
# For the number of build jobs, set CMAKE_BUILD_PARALLEL_LEVEL in your
# environment; "cmake --build" reads it without being told.

include("${CMAKE_CURRENT_LIST_DIR}/local.cmake" OPTIONAL)

set(ftk_API "GL_4_1" CACHE STRING "Graphics API")
set(ftk_SDL2 ON CACHE BOOL "Build SDL2")
set(ftk_SDL3 OFF CACHE BOOL "Build SDL3")
set(ftk_nfd OFF CACHE BOOL "Build NFD")
set(ftk_PYTHON OFF CACHE BOOL "Build support for Python")
set(ftk_TESTS ON CACHE BOOL "Build tests")
set(ftk_EXAMPLES ON CACHE BOOL "Build examples")
set(ftk_GCOV OFF CACHE BOOL "Build with gcov support")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

if(APPLE)
    # The deployment target is policy: the oldest system that is supported.
    # The architecture is not -- it is whatever the machine building is, and
    # naming one here cross compiles on any other. That broke the Intel
    # continuous integration runner, which builds a tool and then runs it.
    # Set CMAKE_OSX_ARCHITECTURES in local.cmake, or in a package config where
    # the answer has to be decided rather than discovered.
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "macOS deployment target")
endif()
