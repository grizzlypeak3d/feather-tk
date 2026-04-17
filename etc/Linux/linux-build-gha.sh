#!/bin/sh

set -x

SOURCE_DIR=$1
BUILD_TYPE=$2

cmake \
    -S $SOURCE_DIR/etc/SuperBuild \
    -B sbuild-$BUILD_TYPE \
    -Dftk_API=$FTK_API \
    -Dftk_SDL2=$FTK_SDL2 \
    -Dftk_SDL3=$FTK_SDL3 \
    -Dftk_nfd=$FTK_NFD \
    -Dftk_PYTHON=$FTK_PYTHON \
    -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PWD/install-$BUILD_TYPE \
    -DCMAKE_PREFIX_PATH=$PWD/install-$BUILD_TYPE
cmake --build sbuild-$BUILD_TYPE -j $JOBS --config $BUILD_TYPE

cmake \
    -S $SOURCE_DIR \
    -B build-$BUILD_TYPE \
    -Dftk_API=$FTK_API \
    -Dftk_SDL2=$FTK_SDL2 \
    -Dftk_SDL3=$FTK_SDL3 \
    -Dftk_nfd=$FTK_NFD \
    -Dftk_PYTHON=$FTK_PYTHON \
    -Dftk_TESTS=$FTK_TESTS \
    -Dftk_EXAMPLES=$FTK_EXAMPLES \
    -Dftk_GCOV=$FTK_GCOV \
    -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
    -DCMAKE_INSTALL_PREFIX=$PWD/install-$BUILD_TYPE \
    -DCMAKE_PREFIX_PATH=$PWD/install-$BUILD_TYPE \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build build-$BUILD_TYPE -j $JOBS --config $BUILD_TYPE
cmake --build build-$BUILD_TYPE --config $BUILD_TYPE --target install
