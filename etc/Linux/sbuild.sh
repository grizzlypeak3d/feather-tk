#!/bin/sh

# Build the dependencies and then feather-tk, into directories beside the
# current one. What to build is in etc/Config/*.cmake rather than here: this
# script is only the part that differs between platforms.

set -e
set -x

SOURCE_DIR=$1
BUILD_TYPE=$2
CONFIG=${3:-default}
CONFIG_FILE=$SOURCE_DIR/etc/Config/$CONFIG.cmake

# Build with every core unless told otherwise; "cmake --build" reads this.
if [ -z "$CMAKE_BUILD_PARALLEL_LEVEL" ]; then
    CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
    export CMAKE_BUILD_PARALLEL_LEVEL
fi

# SBUILD_STAGES picks the stages: "deps" builds only the dependencies, into
# install-<type>, and "project" only feather-tk, against dependencies already
# there. Unset, both. CI caches the dependencies' install directory between
# the two, which is all the project needs of them.
if [ "$SBUILD_STAGES" != "project" ]; then
cmake \
    -S $SOURCE_DIR/etc/SuperBuild \
    -B sb-$BUILD_TYPE \
    -C $CONFIG_FILE \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PWD/install-$BUILD_TYPE \
    -DCMAKE_PREFIX_PATH=$PWD/install-$BUILD_TYPE
cmake --build sb-$BUILD_TYPE --config $BUILD_TYPE
fi
if [ "$SBUILD_STAGES" = "deps" ]; then
    exit 0
fi

cmake \
    -S $SOURCE_DIR \
    -B build-$BUILD_TYPE \
    -C $CONFIG_FILE \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PWD/install-$BUILD_TYPE \
    -DCMAKE_PREFIX_PATH=$PWD/install-$BUILD_TYPE
cmake --build build-$BUILD_TYPE --config $BUILD_TYPE
cmake --build build-$BUILD_TYPE --config $BUILD_TYPE --target install
