// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Tooltip.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void tooltip(py::module_& m)
        {
            py::class_<Tooltip, IPopup, std::shared_ptr<Tooltip> >(m, "Tooltip")
                .def(
                    py::init(&Tooltip::create),
                    py::arg("context"),
                    py::arg("text"),
                    py::arg("pos"),
                    py::arg("window"));
        }
    }
}
