// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FileBrowserWidgets.h>
#include <ftk/UI/RecentFilesModel.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/operators.h>
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
        void fileBrowserWidgets(nb::module_& m)
        {
            nb::class_<FileBrowserPath, IContainer>(m, "FileBrowserPath")
                .def(
                    nb::new_(&FileBrowserPath::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw(
                    "path",
                    &FileBrowserPath::getPath,
                    &FileBrowserPath::setPath)
                .def("setCallback", &FileBrowserPath::setCallback)
                .def_prop_rw(
                    "editable",
                    &FileBrowserPath::isEditable,
                    &FileBrowserPath::setEditable)
                .def("setEditableCallback", &FileBrowserPath::setEditableCallback);

            nb::class_<FileBrowserDrives, IContainer>(m, "FileBrowserDrives")
                .def(
                    nb::new_(&FileBrowserDrives::create),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr);

            nb::class_<FileBrowserShortcuts, IContainer>(m, "FileBrowserShortcuts")
                .def(
                    nb::new_(&FileBrowserShortcuts::create),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr);

            nb::class_<FileBrowserRecent, IContainer>(m, "FileBrowserRecent")
                .def(
                    nb::new_(&FileBrowserRecent::create),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw(
                    "recentFilesModel",
                    &FileBrowserRecent::getRecentFilesModel,
                    &FileBrowserRecent::setRecentFilesModel);

            nb::class_<FileBrowserSettings, IContainer>(m, "FileBrowserSettings")
                .def(
                    nb::new_(&FileBrowserSettings::create),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr);

            nb::class_<FileBrowserView, IWidget>(m, "FileBrowserView")
                .def(
                    nb::new_(&FileBrowserView::create),
                    nb::arg("context"),
                    nb::arg("mode"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("reload", &FileBrowserView::reload)
                .def("setCallback", &FileBrowserView::setCallback)
                .def("setSelectCallback", &FileBrowserView::setSelectCallback)
                .def_prop_rw(
                    "multiple",
                    &FileBrowserView::isMultiple,
                    &FileBrowserView::setMultiple)
                .def_prop_ro("selection", &FileBrowserView::getSelection)
                .def_prop_rw(
                    "search",
                    &FileBrowserView::getSearch,
                    &FileBrowserView::setSearch)
                .def_prop_ro("observeCurrent", &FileBrowserView::observeCurrent)
                .def("getRect", &FileBrowserView::getRect);
        }
    }
}
