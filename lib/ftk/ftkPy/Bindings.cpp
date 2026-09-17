// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>
#include <ftk/UIPy/Bindings.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

NB_MODULE(_feather_tk, m)
{
    m.doc() = "feather-tk is a lightweight toolkit for building professional tools for film, VFX, and animation.";
    ftk::python::coreBind(m);
    ftk::python::uiBind(m);
}
