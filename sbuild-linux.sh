#!/bin/sh

set -x

SOURCE_DIR=$1
BUILD_TYPE=Release
if [ "$#" -eq 2 ]; then
    BUILD_TYPE=$2
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

sh $SOURCE_DIR/etc/Linux/linux-build-gha.sh $SOURCE_DIR $BUILD_TYPE
