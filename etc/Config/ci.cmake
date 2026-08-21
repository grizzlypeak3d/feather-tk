# What continuous integration builds everywhere: the default configuration,
# which is no Python bindings and static libraries. The -python
# configurations beside this one are the other half, and each platform is
# built both ways.
#
# Overrides come before the file they are based on: a plain cache set does not
# overwrite a value that is already there, so the first to set a value wins.
include("${CMAKE_CURRENT_LIST_DIR}/local.cmake" OPTIONAL)

# The macOS and Windows runners have no working OpenGL, so the tests that make
# a context are left out there rather than the suite being skipped. Linux runs
# everything under xvfb.
if(APPLE OR WIN32)
    set(ftk_TESTS_NO_GL ON CACHE BOOL "")
endif()

# The examples are built on Linux only, where they are cheapest.
set(ftk_EXAMPLES OFF CACHE BOOL "Build examples")

include("${CMAKE_CURRENT_LIST_DIR}/default.cmake")
