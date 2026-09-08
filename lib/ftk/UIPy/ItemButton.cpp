// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/ItemButton.h>
#include <ftk/UI/ItemButtonList.h>

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
        void itemButton(nb::module_& m)
        {
            nb::class_<ItemButton, IButton>(m, "ItemButton")
                .def(
                    nb::new_(&ItemButton::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("widget", &ItemButton::getWidget, &ItemButton::setWidget)
                .def("setFocusCallback", &ItemButton::setFocusCallback)
                .def("setDragDropDataCallback", &ItemButton::setDragDropDataCallback)
                .def("setDragDropCursorCallback", &ItemButton::setDragDropCursorCallback);

            nb::class_<ItemControls, IMouseWidget>(m, "ItemControls")
                .def(
                    nb::new_(&ItemControls::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("widget", &ItemControls::getWidget, &ItemControls::setWidget);
        }

        void itemButtonList(nb::module_& m)
        {
            nb::class_<ItemButtonList, VerticalLayout>(m, "ItemButtonList")
                .def(
                    nb::new_(&ItemButtonList::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("current", &ItemButtonList::getCurrent, &ItemButtonList::setCurrent)
                .def("setCurrentCallback", &ItemButtonList::setCurrentCallback)
                .def("setActivateCallback", &ItemButtonList::setActivateCallback)
                .def("setDeleteCallback", &ItemButtonList::setDeleteCallback);
        }
    }
}
