// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/FileBrowserWidgets.h>

#include <ftk/UI/FileBrowserWidgets.h>
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
        void fileBrowserWidgets(py::module_& m)
        {
            py::class_<FileBrowserPath, IWidget, std::shared_ptr<FileBrowserPath> >(m, "FileBrowserPath")
                .def(
                    py::init(&FileBrowserPath::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property(
                    "path",
                    &FileBrowserPath::getPath,
                    &FileBrowserPath::setPath)
                .def("setCallback", &FileBrowserPath::setCallback)
                .def_property(
                    "editable",
                    &FileBrowserPath::isEditable,
                    &FileBrowserPath::setEditable)
                .def("setEditableCallback", &FileBrowserPath::setEditableCallback);

            py::class_<FileBrowserDrives, IWidget, std::shared_ptr<FileBrowserDrives> >(m, "FileBrowserDrives")
                .def(
                    py::init(&FileBrowserDrives::create),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr);

            py::class_<FileBrowserShortcuts, IWidget, std::shared_ptr<FileBrowserShortcuts> >(m, "FileBrowserShortcuts")
                .def(
                    py::init(&FileBrowserShortcuts::create),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr);

            py::class_<FileBrowserRecent, IWidget, std::shared_ptr<FileBrowserRecent> >(m, "FileBrowserRecent")
                .def(
                    py::init(&FileBrowserRecent::create),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property(
                    "recentFilesModel",
                    &FileBrowserRecent::getRecentFilesModel,
                    &FileBrowserRecent::setRecentFilesModel);

            py::class_<FileBrowserSettings, IWidget, std::shared_ptr<FileBrowserSettings> >(m, "FileBrowserSettings")
                .def(
                    py::init(&FileBrowserSettings::create),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr);

            py::class_<FileBrowserView, IWidget, std::shared_ptr<FileBrowserView> >(m, "FileBrowserView")
                .def(
                    py::init(&FileBrowserView::create),
                    py::arg("context"),
                    py::arg("mode"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("reload", &FileBrowserView::reload)
                .def("setCallback", &FileBrowserView::setCallback)
                .def("setSelectCallback", &FileBrowserView::setSelectCallback)
                .def_property(
                    "search",
                    &FileBrowserView::getSearch,
                    &FileBrowserView::setSearch)
                .def_property_readonly("observeCurrent", &FileBrowserView::observeCurrent)
                .def("getRect", &FileBrowserView::getRect);
        }
    }
}
