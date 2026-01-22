// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/TabBar.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void tabBar(py::module_& m)
        {
            py::class_<TabBar, IWidget, std::shared_ptr<TabBar> >(m, "TabBar")
                .def(
                    py::init(&TabBar::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("tabs", &TabBar::getTabs, &TabBar::setTabs)
                .def("addTab", &TabBar::addTab)
                .def("clear", &TabBar::clearTabs)
                .def_property("currentTab", &TabBar::getCurrentTab, &TabBar::setCurrentTab)
                .def("setCallback", &TabBar::setCallback)
                .def("setTabText", &TabBar::setTabText, py::arg("index"), py::arg("text"))
                .def("setTabTooltip", &TabBar::setTabTooltip, py::arg("index"), py::arg("tooltip"));
        }
    }
}
