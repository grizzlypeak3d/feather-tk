// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/ToolBar.h>
#include <ftk/UI/ToolButton.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void toolBar(py::module_& m)
        {
            py::class_<ToolBar, IWidget, std::shared_ptr<ToolBar> >(m, "ToolBar")
                .def(
                    py::init(&ToolBar::create),
                    py::arg("context"),
                    py::arg("orientation") = Orientation::Horizontal,
                    py::arg("parent") = nullptr)
                .def("addAction", &ToolBar::addAction)
                .def("addWidget", &ToolBar::addWidget)
                .def("clear", &ToolBar::clear);
        }
    }
}
