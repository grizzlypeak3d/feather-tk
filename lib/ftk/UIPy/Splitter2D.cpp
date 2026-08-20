// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Splitter2D.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void splitter2D(py::module_& m)
        {
            py::class_<Splitter2D, IWidget, std::shared_ptr<Splitter2D> >(m, "Splitter2D")
                .def(
                    py::init(&Splitter2D::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def("setWidgets", &Splitter2D::setWidgets, py::arg("widgets"))
                .def_property("split", &Splitter2D::getSplit, &Splitter2D::setSplit, py::return_value_policy::copy)
                .def("setSplitCallback", &Splitter2D::setSplitCallback)
                .def_property("border", &Splitter2D::hasBorder, &Splitter2D::setBorder);
        }
    }
}
