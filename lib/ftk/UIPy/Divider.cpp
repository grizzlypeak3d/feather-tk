// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Divider.h>

#include <ftk/UI/Divider.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void divider(py::module_& m)
        {
            py::class_<Divider, IWidget, std::shared_ptr<Divider> >(m, "Divider")
                .def(
                    py::init(&Divider::create),
                    py::arg("context"),
                    py::arg("orientation"),
                    py::arg("parent") = nullptr);
        }
    }
}
