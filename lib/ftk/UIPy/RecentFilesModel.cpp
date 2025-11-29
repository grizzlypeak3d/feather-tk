// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/RecentFilesModel.h>

#include <ftk/CorePy/ObservableValue.h>

#include <ftk/UI/RecentFilesModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void recentFilesModel(py::module_& m)
        {
            observableValue<std::filesystem::path>(m, "Path");

            py::class_<RecentFilesModel, std::shared_ptr<RecentFilesModel> >(m, "RecentFilesModel")
                .def(
                    py::init(&RecentFilesModel::create),
                    py::arg("context"))
                .def_property(
                    "recentMax",
                    &RecentFilesModel::getRecentMax,
                    &RecentFilesModel::setRecentMax)
                .def("observeRecentMax", &RecentFilesModel::observeRecentMax)
                .def_property(
                    "recent",
                    &RecentFilesModel::getRecent,
                    &RecentFilesModel::setRecent)
                .def("observeRecent", &RecentFilesModel::observeRecent)
                .def("addRecent", &RecentFilesModel::addRecent);
        }
    }
}
