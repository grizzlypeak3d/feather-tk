// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/DrivesModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void drivesModel(py::module_& m)
        {
            observableList<std::filesystem::path>(m, "FileSystemPath");

            py::class_<DrivesModel, std::shared_ptr<DrivesModel> >(m, "DrivesModel")
                .def(
                    py::init(&DrivesModel::create),
                    py::arg("context"))
                .def("observeDrives", &DrivesModel::observeDrives);
        }
    }
}
