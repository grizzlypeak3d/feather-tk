// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/TabBar.h>

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
        void tabBar(nb::module_& m)
        {
            nb::class_<TabBar, IContainer>(m, "TabBar")
                .def(
                    nb::new_(&TabBar::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("tabs", &TabBar::getTabs, &TabBar::setTabs)
                .def("addTab", &TabBar::addTab)
                .def("clear", &TabBar::clear)
                .def_prop_rw("current", &TabBar::getCurrent, &TabBar::setCurrent)
                .def("setCallback", &TabBar::setCallback)
                .def("setText", &TabBar::setText, nb::arg("index"), nb::arg("text"))
                .def("setTabTooltip", &TabBar::setTabTooltip, nb::arg("index"), nb::arg("tooltip"))
                .def_prop_rw("closable", &TabBar::isClosable, &TabBar::setClosable)
                .def("setTabCloseCallback", &TabBar::setCloseCallback)
                .def_prop_rw("scrollBarVisible", &TabBar::isScrollBarVisible, &TabBar::setScrollBarVisible);
        }
    }
}
