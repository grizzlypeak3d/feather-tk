// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/GraphWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void graphWidget(py::module_& m)
        {
            py::class_<GraphLabel>(m, "GraphLabel")
                .def(py::init<>())
                .def_readwrite("colorRole", &GraphLabel::colorRole)
                .def_readwrite("text", &GraphLabel::text)
                .def_readwrite("format", &GraphLabel::format);

            py::class_<GraphWidget, IContainer, std::shared_ptr<GraphWidget> >(m, "GraphWidget")
                .def(
                    py::init(&GraphWidget::create),
                    py::arg("context"),
                    py::arg("title"),
                    py::arg("labels"),
                    py::arg("parent") = nullptr)
                .def("setSamples", &GraphWidget::setSamples)
                .def("addSample", &GraphWidget::addSample);
        }
    }
}
