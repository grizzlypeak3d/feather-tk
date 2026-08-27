# Continuous integration on Linux against OpenGL ES 2, which has no coverage
# build.
include("${CMAKE_CURRENT_LIST_DIR}/local.cmake" OPTIONAL)
set(ftk_API "GLES_3" CACHE STRING "Graphics API")
set(ftk_GCOV OFF CACHE BOOL "Build with gcov support")
include("${CMAKE_CURRENT_LIST_DIR}/ci-linux.cmake")
