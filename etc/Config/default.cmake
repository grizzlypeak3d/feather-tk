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
# Say so: the file is not tracked, so a build that behaves oddly has nothing
# else to notice it by. Every config reaches this file, so once here covers
# all of them.
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/local.cmake")
    message(STATUS "etc/Config/local.cmake is in use; personal settings are affecting this build")
endif()

set(ftk_API "GL_4_1" CACHE STRING "Graphics API")
set(ftk_SDL2 ON CACHE BOOL "Build SDL2")
set(ftk_SDL3 OFF CACHE BOOL "Build SDL3")
set(ftk_nfd OFF CACHE BOOL "Build NFD")
set(ftk_PYTHON OFF CACHE BOOL "Build support for Python")
set(ftk_TESTS ON CACHE BOOL "Build tests")
set(ftk_EXAMPLES ON CACHE BOOL "Build examples")
set(ftk_GCOV OFF CACHE BOOL "Build with gcov support")

# Shared when Python is on, the same rule tlRender, DJV and DJV Studio
# carry. Each binding module would otherwise link its own static copy of the
# stack, and two copies of a library in one process do not share its type
# information -- on macOS SDL announces the duplicate as an Objective-C class
# implemented twice.
#
# Windows as well, now. Three things were in the way and none of them was the
# export macros being absent:
#
# * FTK_API_TYPE on a class is empty on Windows -- only the per-member
#   FTK_API carries the declspec there, where the other platforms get the
#   whole class from one visibility attribute. The members that had been
#   marked at the class level alone are marked individually.
# * OpenTimelineIO defined OTIO_EXPORTS PUBLIC, so everything consuming it
#   compiled its API as dllexport where it wanted dllimport, and its own
#   members were marked the same class-at-a-time way. The super build patches
#   both until that is upstream.
# * The libraries with no API of their own -- resources, glad, the test
#   helpers -- are built static. A shared library exporting nothing gets no
#   import library on Windows, and the first thing to ask for one stops.
#
# One wrinkle is left. FTK_API is a single macro for every library in this
# project rather than one apiece, so a library compiling a sibling's headers
# has FTK_EXPORTS set for its own API and reads the sibling's as dllexport.
# Functions live with that -- the linker takes them from the import library --
# and data does not, so a variable exported across two of these libraries will
# not link. There is none today.
set(BUILD_SHARED_LIBS ${ftk_PYTHON} CACHE BOOL "Build shared libraries")

if(APPLE)
    # The deployment target is policy: the oldest system that is supported.
    # The architecture is not -- it is whatever the machine building is, and
    # naming one here cross compiles on any other. That broke the Intel
    # continuous integration runner, which builds a tool and then runs it.
    # Set CMAKE_OSX_ARCHITECTURES in local.cmake, or in a package config where
    # the answer has to be decided rather than discovered.
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "macOS deployment target")
endif()
