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

cmake \
    -S $SOURCE_DIR/etc/SuperBuild \
    -B sb-$BUILD_TYPE \
    -C $CONFIG_FILE \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PWD/install-$BUILD_TYPE \
    -DCMAKE_PREFIX_PATH=$PWD/install-$BUILD_TYPE
cmake --build sb-$BUILD_TYPE --config $BUILD_TYPE

cmake \
    -S $SOURCE_DIR \
    -B build-$BUILD_TYPE \
    -C $CONFIG_FILE \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PWD/install-$BUILD_TYPE \
    -DCMAKE_PREFIX_PATH=$PWD/install-$BUILD_TYPE
cmake --build build-$BUILD_TYPE --config $BUILD_TYPE
cmake --build build-$BUILD_TYPE --config $BUILD_TYPE --target install
