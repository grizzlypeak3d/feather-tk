// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/StackLayout.h>

#include <nanobind/nanobind.h>
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
        void stackLayout(nb::module_& m)
        {
            nb::class_<StackLayout, IWidget>(m, "StackLayout")
                .def(
                    nb::new_(&StackLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("currentIndex", &StackLayout::getCurrentIndex, &StackLayout::setCurrentIndex)
                .def("nextIndex", &StackLayout::nextIndex)
                .def_prop_ro("observeHasNextIndex", &StackLayout::observeHasNextIndex)
                .def("prevIndex", &StackLayout::prevIndex)
                .def_prop_ro("observeHasPrevIndex", &StackLayout::observeHasPrevIndex)
                .def("firstIndex", &StackLayout::firstIndex)
                .def("lastIndex", &StackLayout::lastIndex)
                .def_prop_rw("currentWidget", &StackLayout::getCurrentWidget, &StackLayout::setCurrentWidget)
                .def_prop_rw("marginRole", &StackLayout::getMarginRole, &StackLayout::setMarginRole)
                .def("clear", &StackLayout::clear);
        }
    }
}
