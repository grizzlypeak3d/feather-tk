#!/bin/sh

# Usage: sh sbuild-macos.sh [source directory] [build type] [config]
#
# The config names a file in etc/Config; "default" builds everything. Personal
# settings go in etc/Config/local.cmake, which is not tracked. For the number
# of build jobs, export CMAKE_BUILD_PARALLEL_LEVEL.

sh ${1:-feather-tk}/etc/macOS/sbuild.sh ${1:-feather-tk} ${2:-Release} ${3:-default}
