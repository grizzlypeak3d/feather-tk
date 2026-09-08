// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IMenuPopup.h>
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
        void iMenuPopup(nb::module_& m)
        {
            nb::class_<IMenuPopup, IPopup>(m, "IMenuPopup")
                .def(
                    "open",
                    nb::overload_cast<
                        const std::shared_ptr<IWindow>&,
                        const Box2I&>(&IMenuPopup::open),
                    nb::arg("window"),
                    nb::arg("buttonGeometry"))
                .def(
                    "open",
                    nb::overload_cast<
                        const std::shared_ptr<IWindow>&,
                        const V2I&>(&IMenuPopup::open),
                    nb::arg("window"),
                    nb::arg("pos"))
                .def("isOpen", &IMenuPopup::isOpen)
                .def(
                    "setCloseCallback",
                    &IMenuPopup::setCloseCallback,
                    nb::arg("callback"))
                .def(
                    "setPopup",
                    &IMenuPopup::setPopup,
                    nb::arg("type"))
                .def(
                    "setPopupRole",
                    &IMenuPopup::setPopupRole,
                    nb::arg("role"))
                .def(
                    "setWidget",
                    &IMenuPopup::setWidget,
                    nb::arg("widget"));
        }
    }
}
