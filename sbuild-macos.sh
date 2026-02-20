#!/bin/sh

set -x

BUILD_TYPE=Release
if [ "$#" -eq 1 ]; then
    BUILD_TYPE=$1
fi

export JOBS=4
export FTK_API=GL_4_1
export FTK_SDL2=ON
export FTK_SDL3=OFF
export FTK_NFD=OFF
export FTK_PYTHON=OFF
export FTK_TESTS=ON
export FTK_EXAMPLES=ON
export FTK_GCOV=OFF
export BUILD_SHARED_LIBS=OFF
export CMAKE_OSX_DEPLOYMENT_TARGET=10.15
export CMAKE_OSX_ARCHITECTURES=arm64

sh feather-tk/etc/macOS/macos-build-gha.sh $BUILD_TYPE
