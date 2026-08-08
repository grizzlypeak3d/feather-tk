#!/bin/sh

set -x

sudo apt-get update
cmake --version
python --version

# Coverage is a property of the build config now rather than an environment
# variable, and lcov is small, so install it either way instead of teaching
# this script which configs measure coverage.
sudo apt-get install lcov

sudo apt-get install xorg-dev libglu1-mesa-dev mesa-common-dev mesa-utils xvfb
xvfb-run glxinfo
