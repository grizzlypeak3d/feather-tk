// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

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
                .value("Open", FileBrowserMode::Open)
                .value("Save", FileBrowserMode::Save)
                .value("Dir", FileBrowserMode::Dir);
            FTK_ENUM_BIND(m, FileBrowserMode);

            py::class_<FileBrowserOptions>(m, "FileBrowserOptions")
                .def(py::init())
                .def_readwrite("dirList", &FileBrowserOptions::dirList)
                .def_readwrite("panel", &FileBrowserOptions::panel)
                .def_readwrite("pathEditable", &FileBrowserOptions::pathEditable)
                .def_readwrite("bellows", &FileBrowserOptions::bellows)
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
                .def_property_readonly("observePath", &FileBrowserModel::observePath)
                .def("forward", &FileBrowserModel::forward)
                .def_property_readonly("observeHasForward", &FileBrowserModel::observeHasForward)
                .def("back", &FileBrowserModel::back)
                .def_property_readonly("observeHasBack", &FileBrowserModel::observeHasBack)
                .def_property(
                    "options",
                    &FileBrowserModel::getOptions,
                    &FileBrowserModel::setOptions)
                .def_property_readonly("observeOptions", &FileBrowserModel::observeOptions)
                .def_property(
                    "exts",
                    &FileBrowserModel::getExts,
                    &FileBrowserModel::setExts)
                .def_property_readonly("observeExts", &FileBrowserModel::observeExts)
                .def_property(
                    "ext",
                    &FileBrowserModel::getExt,
                    &FileBrowserModel::setExt)
                .def_property_readonly("observeExt", &FileBrowserModel::observeExt);

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
                    py::arg("mode") = FileBrowserMode::Open)
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

            m.def("to_json",
                [](const FileBrowserOptions& value)
                {
                    nlohmann::json json;
                    to_json(json, value);
                    return json.dump();
                });
            m.def("from_json",
                [](const std::string& value, FileBrowserOptions& out)
                {
                    from_json(nlohmann::json().parse(value), out);
                });
        }
    }
}
