// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>
#include <ftk/UIPy/Bindings.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

NB_MODULE(ftkPy, m)
{
    m.doc() = "Feather-tk is a lightweight toolkit for building cross-platform C++ applications.";
    ftk::python::coreBind(m);
    ftk::python::uiBind(m);
}
