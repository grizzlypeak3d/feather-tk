// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/IWindow.h>
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
        void fileBrowser(nb::module_& m)
        {
            FTK_ENUM_PY(m, FileBrowserMode);
            FTK_ENUM_BIND(m, FileBrowserMode);

            observable<std::filesystem::path>(m, "FileSystemPath");

            nb::class_<FileBrowserOpenOptions>(m, "FileBrowserOpenOptions")
                .def(nb::init<>())
                .def_rw("title", &FileBrowserOpenOptions::title)
                .def_rw("path", &FileBrowserOpenOptions::path)
                .def_rw("mode", &FileBrowserOpenOptions::mode)
                .def_rw("fileName", &FileBrowserOpenOptions::fileName)
                .def_rw("extensions", &FileBrowserOpenOptions::extensions)
                .def_rw("extensionsLabel", &FileBrowserOpenOptions::extensionsLabel)
                .def_rw("multiple", &FileBrowserOpenOptions::multiple);

            nb::class_<FileBrowserOptions>(m, "FileBrowserOptions")
                .def(nb::init<>())
                .def_rw("dirList", &FileBrowserOptions::dirList)
                .def_rw("panel", &FileBrowserOptions::panel)
                .def_rw("pathEditable", &FileBrowserOptions::pathEditable)
                .def_rw("bellows", &FileBrowserOptions::bellows)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<FileBrowserModel>(m, "FileBrowserModel")
                .def(
                    nb::new_(&FileBrowserModel::create),
                    nb::arg("context"))
                .def_prop_rw(
                    "path",
                    &FileBrowserModel::getPath,
                    // Named, because setPath is overloaded on the path and a
                    // UTF-8 string and its address alone does not say which.
                    // The path one: pybind11's filesystem caster already
                    // brings str and os.PathLike across as a path.
                    nb::overload_cast<
                        const std::filesystem::path&>(&FileBrowserModel::setPath))
                .def_prop_ro("observePath", &FileBrowserModel::observePath)
                .def("forward", &FileBrowserModel::forward)
                .def_prop_ro("observeHasForward", &FileBrowserModel::observeHasForward)
                .def("back", &FileBrowserModel::back)
                .def_prop_ro("observeHasBack", &FileBrowserModel::observeHasBack)
                .def_prop_rw(
                    "options",
                    &FileBrowserModel::getOptions,
                    &FileBrowserModel::setOptions,
                    nb::rv_policy::copy)
                .def_prop_ro("observeOptions", &FileBrowserModel::observeOptions)
                .def_prop_rw(
                    "exts",
                    &FileBrowserModel::getExts,
                    &FileBrowserModel::setExts)
                .def_prop_ro("observeExts", &FileBrowserModel::observeExts)
                .def_prop_rw(
                    "ext",
                    &FileBrowserModel::getExt,
                    &FileBrowserModel::setExt)
                .def_prop_ro("observeExt", &FileBrowserModel::observeExt);

            nb::class_<FileBrowserSystem, ISystem>(m, "FileBrowserSystem")
                .def(
                    nb::new_(&FileBrowserSystem::create),
                    nb::arg("context"))
                .def("open",
                    static_cast<void (FileBrowserSystem::*)(
                        const std::shared_ptr<IWindow>&,
                        const std::function<void(const Path&)>&,
                        const FileBrowserOpenOptions&)>(&FileBrowserSystem::open),
                    nb::arg("window"),
                    nb::arg("callback"),
                    nb::arg("options") = FileBrowserOpenOptions())
                .def("openMultiple",
                    static_cast<void (FileBrowserSystem::*)(
                        const std::shared_ptr<IWindow>&,
                        const std::function<void(const std::vector<Path>&)>&,
                        const FileBrowserOpenOptions&)>(&FileBrowserSystem::open),
                    nb::arg("window"),
                    nb::arg("callback"),
                    nb::arg("options") = FileBrowserOpenOptions())
                .def_prop_rw(
                    "nativeFileDialog",
                    &FileBrowserSystem::isNativeFileDialog,
                    &FileBrowserSystem::setNativeFileDialog)
                .def_prop_ro(
                    "model",
                    &FileBrowserSystem::getModel)
                .def_prop_rw(
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
