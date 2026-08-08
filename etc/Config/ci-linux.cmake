# Continuous integration on Linux, which also measures coverage and builds the
# examples.
include("${CMAKE_CURRENT_LIST_DIR}/local.cmake" OPTIONAL)
set(ftk_EXAMPLES ON CACHE BOOL "Build examples")
set(ftk_GCOV ON CACHE BOOL "Build with gcov support")
include("${CMAKE_CURRENT_LIST_DIR}/ci.cmake")
