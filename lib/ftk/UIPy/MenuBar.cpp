// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/MenuBar.h>

#include <ftk/UI/MenuBar.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void menuBar(py::module_& m)
        {
            py::class_<MenuBar, IWidget, std::shared_ptr<MenuBar> >(m, "MenuBar")
                .def(
                    py::init(&MenuBar::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    "addMenu",
                    &MenuBar::addMenu,
                    py::arg("text"),
                    py::arg("menu"))
                .def(
                    "getMenu",
                    &MenuBar::getMenu,
                    py::arg("text"))
                .def("clear", &MenuBar::clear)
                .def(
                    "shortcut",
                    &MenuBar::shortcut,
                    py::arg("key"),
                    py::arg("modifiers"));
        }
    }
}
