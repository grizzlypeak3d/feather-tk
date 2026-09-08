// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/DrivesModel.h>

#include <ftk/Core/Context.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void drivesModel(nb::module_& m)
        {
            observableList<std::filesystem::path>(m, "FileSystemPath");

            nb::class_<DrivesModel>(m, "DrivesModel")
                .def(
                    nb::new_(&DrivesModel::create),
                    nb::arg("context"))
                .def("observeDrives", &DrivesModel::observeDrives);
        }
    }
}
