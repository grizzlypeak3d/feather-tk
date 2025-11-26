// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bellows.h>

#include <ftk/UI/Bellows.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void bellows(py::module_& m)
        {
            py::class_<Bellows, IWidget, std::shared_ptr<Bellows> >(m, "Bellows")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&Bellows::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&Bellows::create)),
                    py::arg("context"),
                    py::arg("text"),
                    py::arg("parent") = nullptr)
                .def_property("text", &Bellows::getText, &Bellows::setText)
                .def_property("widget", &Bellows::getWidget, &Bellows::setWidget)
                .def_property("open", &Bellows::isOpen, &Bellows::setOpen)
                .def_property("toolWidget", &Bellows::getToolWidget, &Bellows::setToolWidget);
        }
    }
}
