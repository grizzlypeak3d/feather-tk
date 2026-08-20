// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FileLogSystem.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void fileLogSystem(py::module_& m)
        {
            py::class_<FileLogSystem, ISystem, std::shared_ptr<FileLogSystem> >(m, "FileLogSystem")
                .def(
                    py::init(&FileLogSystem::create),
                    py::arg("context"),
                    py::arg("path"));
        }
    }
}
