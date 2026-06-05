#!/bin/sh

set -x

sudo apt-get update
cmake --version
python --version

if [ $FTK_GCOV = "ON" ]; then
    sudo apt-get install lcov
fi

sudo apt-get install xorg-dev libglu1-mesa-dev mesa-common-dev mesa-utils xvfb
xvfb-run glxinfo
