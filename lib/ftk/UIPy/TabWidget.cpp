// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/TabWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void tabWidget(nb::module_& m)
        {
            nb::class_<TabWidget, IContainer>(m, "TabWidget")
                .def(
                    nb::new_(&TabWidget::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_ro("tabs", &TabWidget::getTabs)
                .def(
                    "addTab",
                    &TabWidget::addTab,
                    nb::arg("name"),
                    nb::arg("widget"),
                    nb::arg("tooltip") = std::string())
                .def(
                    "removeTab",
                    nb::overload_cast<int>(&TabWidget::removeTab),
                    nb::arg("index"))
                .def(
                    "removeTab",
                    nb::overload_cast<const std::shared_ptr<IWidget>&>(&TabWidget::removeTab),
                    nb::arg("widget"))
                .def("clear", &TabWidget::clear)
                .def_prop_rw("current", &TabWidget::getCurrent, &TabWidget::setCurrent)
                .def("setCallback", &TabWidget::setCallback)
                .def_prop_rw("currentWidget", &TabWidget::getCurrentWidget, &TabWidget::setCurrentWidget)
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
                .def_prop_rw("closable", &TabWidget::isClosable, &TabWidget::setClosable)
                .def("setCloseCallback", &TabWidget::setCloseCallback);
        }
    }
}
