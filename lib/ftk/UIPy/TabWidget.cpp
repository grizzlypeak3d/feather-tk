// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/TabWidget.h>

#include <ftk/UI/TabWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void tabWidget(py::module_& m)
        {
            py::class_<TabWidget, IWidget, std::shared_ptr<TabWidget> >(m, "TabWidget")
                .def(
                    py::init(&TabWidget::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property_readonly("tabs", &TabWidget::getTabs)
                .def(
                    "addTab",
                    &TabWidget::addTab,
                    py::arg("name"),
                    py::arg("widget"),
                    py::arg("tooltip") = std::string())
                .def("clearTabs", &TabWidget::clearTabs)
                .def_property("currentTab", &TabWidget::getCurrentTab, &TabWidget::setCurrentTab)
                .def("setCurrentTabCallback", &TabWidget::setCurrentTabCallback)
                .def_property("currentWidget", &TabWidget::getCurrentWidget, &TabWidget::setCurrentWidget)
                .def("setCurrentWidgetCallback", &TabWidget::setCurrentWidgetCallback);
        }
    }
}
