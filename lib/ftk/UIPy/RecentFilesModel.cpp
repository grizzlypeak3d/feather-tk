// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/RecentFilesModel.h>

#include <ftk/CorePy/ObservableList.h>
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
            observableList<std::filesystem::path>(m, "Path");

            py::class_<RecentFilesModel, std::shared_ptr<RecentFilesModel> >(m, "RecentFilesModel")
                .def(
                    py::init(&RecentFilesModel::create),
                    py::arg("context"))
                .def_property(
                    "recentMax",
                    &RecentFilesModel::getRecentMax,
                    &RecentFilesModel::setRecentMax)
                .def_property_readonly("observeRecentMax", &RecentFilesModel::observeRecentMax)
                .def_property(
                    "recent",
                    [](RecentFilesModel& model)
                    {
                        std::vector<std::string> out;
                        for (const auto& recent : model.getRecent())
                        {
                            out.push_back(recent.u8string());
                        }
                        return out;
                    },
                    [](RecentFilesModel& model, const std::vector<std::string>& value)
                    {
                        std::vector<std::filesystem::path> paths;
                        for (const auto& i : value)
                        {
                            paths.push_back(std::filesystem::u8path(i));
                        }
                        model.setRecent(paths);
                    })
                .def_property_readonly("observeRecent", &RecentFilesModel::observeRecent)
                .def("addRecent", &RecentFilesModel::addRecent);
        }
    }
}
