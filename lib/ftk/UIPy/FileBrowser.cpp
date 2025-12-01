// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/FileBrowser.h>

#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/RecentFilesModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void fileBrowser(py::module_& m)
        {
            py::enum_<FileBrowserMode>(m, "FileBrowserMode")
                .value("File", FileBrowserMode::File)
                .value("Dir", FileBrowserMode::Dir);

            py::class_<FileBrowserOptions>(m, "FileBrowserOptions")
                .def(py::init())
                .def_readwrite("dirList", &FileBrowserOptions::dirList)
                .def_readwrite("panel", &FileBrowserOptions::panel)
                .def_readwrite("pathEdit", &FileBrowserOptions::pathEdit)
                .def_readwrite("bellows", &FileBrowserOptions::bellows)
                .def("to_json",
                    [](FileBrowserOptions& self)
                    {
                        nlohmann::json json;
                        to_json(json, self);
                        return json.dump();
                    })
                .def("from_json",
                    [](FileBrowserOptions& self, const std::string& value)
                    {
                        from_json(nlohmann::json().parse(value), self);
                    })
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self);

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
}
