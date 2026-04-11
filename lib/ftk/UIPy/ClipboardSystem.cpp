// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ClipboardSystem.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void clipboardSystem(py::module_& m)
        {
            py::class_<ClipboardSystem, ISystem, std::shared_ptr<ClipboardSystem> >(
                m, "ClipboardSystem")
                .def(
                    py::init(&ClipboardSystem::create),
                    py::arg("context"))
                .def_property("text",
                    &ClipboardSystem::getText,
                    &ClipboardSystem::setText)
                .def("observeText", &ClipboardSystem::observeText);
        }
    }
}
