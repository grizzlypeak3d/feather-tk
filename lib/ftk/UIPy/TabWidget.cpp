// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

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
                .def("clear", &TabWidget::clear)
                .def_property("current", &TabWidget::getCurrent, &TabWidget::setCurrent)
                .def("setCallback", &TabWidget::setCallback)
                .def_property("currentWidget", &TabWidget::getCurrentWidget, &TabWidget::setCurrentWidget)
                .def("setWidgetCallback", &TabWidget::setWidgetCallback)
                .def(
                    "setTabText",
                    [](const std::shared_ptr<TabWidget> self, int index, const std::string& text)
                    {
                        self->setText(index, text);
                    })
                .def(
                    "setTabText",
                    [](const std::shared_ptr<TabWidget> self, const std::shared_ptr<IWidget>& widget, const std::string& text)
                    {
                        self->setText(widget, text);
                    })
                .def(
                    "setTabTooltip",
                    [](const std::shared_ptr<TabWidget> self, int index, const std::string& tooltip)
                    {
                        self->setTabTooltip(index, tooltip);
                    })
                .def(
                    "setTabTooltip",
                    [](const std::shared_ptr<TabWidget> self, const std::shared_ptr<IWidget>& widget, const std::string& tooltip)
                    {
                        self->setTabTooltip(widget, tooltip);
                    })
                .def_property("closable", &TabWidget::isClosable, &TabWidget::setClosable)
                .def("setCloseCallback", &TabWidget::setCloseCallback);
        }
    }
}
