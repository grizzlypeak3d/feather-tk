// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UIPy/FileBrowser.h>

#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/RecentFilesModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

namespace ftk
{
    void fileBrowser(py::module_& m)
    {
        py::enum_<FileBrowserMode>(m, "FileBrowserMode")
            .value("File", FileBrowserMode::File)
            .value("Dir", FileBrowserMode::Dir);

        py::class_<FileBrowserModel, std::shared_ptr<FileBrowserModel> >(m, "FileBrowserModel")
            .def(
                py::init(&FileBrowserModel::create),
                py::arg("context"))
            .def_property(
                "path",
                &FileBrowserModel::getPath,
                &FileBrowserModel::setPath)
            .def("observePath", &FileBrowserModel::observePath)
            .def("forward", &FileBrowserModel::forward)
            .def("observeHasForward", &FileBrowserModel::observeHasForward)
            .def("back", &FileBrowserModel::back)
            .def("observeHasBack", &FileBrowserModel::observeHasBack)
            .def_property(
                "options",
                &FileBrowserModel::getOptions,
                &FileBrowserModel::setOptions)
            .def("observeOptions", &FileBrowserModel::observeOptions)
            .def_property(
                "exts",
                &FileBrowserModel::getExts,
                &FileBrowserModel::setExts)
            .def("observeExts", &FileBrowserModel::observeExts)
            .def_property(
                "ext",
                &FileBrowserModel::getExt,
                &FileBrowserModel::setExt)
            .def("observeExt", &FileBrowserModel::observeExt);

        py::class_<FileBrowserSystem, ISystem, std::shared_ptr<FileBrowserSystem> >(m, "FileBrowserSystem")
            .def(
                py::init(&FileBrowserSystem::create),
                py::arg("context"))
            .def("open",
                &FileBrowserSystem::open,
                py::arg("window"),
                py::arg("callback"),
                py::arg("title") = "Open",
                py::arg("fileName") = std::string(),
                py::arg("mode") = FileBrowserMode::File)
            .def_property(
                "nativeFileDialog",
                &FileBrowserSystem::isNativeFileDialog,
                &FileBrowserSystem::setNativeFileDialog)
            .def_property_readonly(
                "model",
                &FileBrowserSystem::getModel)
            .def_property(
                "recentFilesModel",
                &FileBrowserSystem::getRecentFilesModel,
                &FileBrowserSystem::setRecentFilesModel);
    }
}
