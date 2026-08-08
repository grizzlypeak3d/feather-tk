# What continuous integration builds everywhere.
#
# Overrides come before the file they are based on: a plain cache set does not
# overwrite a value that is already there, so the first to set a value wins.
include("${CMAKE_CURRENT_LIST_DIR}/local.cmake" OPTIONAL)

# On here and off in the default build. Nothing else compiles the Python
# bindings, so an API change that breaks them builds clean everywhere else and
# fails only here.
set(ftk_PYTHON ON CACHE BOOL "Build support for Python")

# The examples are built on Linux only, where they are cheapest.
set(ftk_EXAMPLES OFF CACHE BOOL "Build examples")

include("${CMAKE_CURRENT_LIST_DIR}/default.cmake")
