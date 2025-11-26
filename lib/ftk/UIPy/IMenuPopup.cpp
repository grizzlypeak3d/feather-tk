// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/IMenuPopup.h>

#include <ftk/UI/IMenuPopup.h>
#include <ftk/UI/IWindow.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void iMenuPopup(py::module_& m)
        {
            py::class_<IMenuPopup, IPopup, std::shared_ptr<IMenuPopup> >(m, "IMenuPopup")
                .def(
                    "open",
                    &IMenuPopup::open,
                    py::arg("window"),
                    py::arg("buttonGeometry"))
                .def("isOpen", &IMenuPopup::isOpen)
                .def(
                    "setCloseCallback",
                    &IMenuPopup::setCloseCallback,
                    py::arg("callback"))
                .def(
                    "setPopup",
                    &IMenuPopup::setPopup,
                    py::arg("type"))
                .def(
                    "setPopupRole",
                    &IMenuPopup::setPopupRole,
                    py::arg("role"))
                .def(
                    "setWidget",
                    &IMenuPopup::setWidget,
                    py::arg("widget"));
        }
    }
}
