// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IWidgetPopup.h>
#include <ftk/UI/IWindow.h>

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
        void iWidgetPopup(nb::module_& m)
        {
            nb::class_<IWidgetPopup, IPopup>(m, "IWidgetPopup")
                .def(
                    "open",
                    &IWidgetPopup::open,
                    nb::arg("window"),
                    nb::arg("buttonGeometry"),
                    nb::arg("widgetGeometry") = std::optional<Box2I>())
                .def_prop_ro("isOpen", &IWidgetPopup::isOpen)
                .def("setCloseCallback", &IWidgetPopup::setCloseCallback)
                .def_prop_rw("widget", &IWidgetPopup::getWidget, &IWidgetPopup::setWidget)
                .def_prop_rw("popupRole", &IWidgetPopup::getPopupRole, &IWidgetPopup::setPopupRole);
        }
    }
}
