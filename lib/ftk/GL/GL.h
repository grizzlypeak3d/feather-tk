// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

// One loader for OpenGL and OpenGL ES: the functions and constants of both,
// filled in for whichever the context is. See gl::getAPI() in Init.h for
// which that is; a function the API in use lacks is a null pointer.
#include <ftk/glad/gl.h>
