// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Bellows.h>

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
        void bellows(nb::module_& m)
        {
            nb::class_<Bellows, IContainer>(m, "Bellows")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&Bellows::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&Bellows::create)),
                    nb::arg("context"),
                    nb::arg("text"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("text", &Bellows::getText, &Bellows::setText)
                .def_prop_rw("widget", &Bellows::getWidget, &Bellows::setWidget)
                .def_prop_rw("open", &Bellows::isOpen, &Bellows::setOpen)
                .def_prop_rw("toolWidget", &Bellows::getToolWidget, &Bellows::setToolWidget)
                .def("setOpenCallback", &Bellows::setOpenCallback);
        }
    }
}
