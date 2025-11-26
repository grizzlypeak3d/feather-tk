// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>
#include <ftk/UIPy/Bindings.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(ftkPy, m)
{
    m.doc() = "Feather-tk is a lightweight toolkit for building cross-platform C++ applications.";
    ftk::python::coreBind(m);
    ftk::python::uiBind(m);
}
