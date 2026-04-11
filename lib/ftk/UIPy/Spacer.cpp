// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Spacer.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void spacer(py::module_& m)
        {
            py::class_<Spacer, IWidget, std::shared_ptr<Spacer> >(m, "Spacer")
                .def(
                    py::init(&Spacer::create),
                    py::arg("context"),
                    py::arg("orientation"),
                    py::arg("parent") = nullptr)
                .def_property("spacingRole",
                    &Spacer::getSpacingRole,
                    &Spacer::setSpacingRole);
        }
    }
}
