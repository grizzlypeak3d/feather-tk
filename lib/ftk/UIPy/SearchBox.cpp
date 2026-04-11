// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/SearchBox.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void searchBox(py::module_& m)
        {
            py::class_<SearchBox, IWidget, std::shared_ptr<SearchBox> >(m, "SearchBox")
                .def(
                    py::init(&SearchBox::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("text", &SearchBox::getText, &SearchBox::setText)
                .def("setCallback", &SearchBox::setCallback);
        }
    }
}
