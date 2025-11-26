// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UIPy/Splitter.h>

#include <ftk/UI/Splitter.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    void splitter(py::module_& m)
    {
        py::class_<Splitter, IWidget, std::shared_ptr<Splitter> >(m, "Splitter")
            .def(
                py::init(&Splitter::create),
                py::arg("context"),
                py::arg("orientation"),
                py::arg("parent") = nullptr)
            .def_property("split", &Splitter::getSplit, &Splitter::setSplit)
            .def_property("spacingRole", &Splitter::hasBorder, &Splitter::setBorder);
    }
}
