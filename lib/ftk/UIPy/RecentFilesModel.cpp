// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/RecentFilesModel.h>

#include <ftk/Core/Context.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
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
        void recentFilesModel(nb::module_& m)
        {
            observableList<Path>(m, "Path");

            nb::class_<RecentFilesModel>(m, "RecentFilesModel")
                .def(
                    nb::new_(&RecentFilesModel::create),
                    nb::arg("context"))
                .def_prop_rw(
                    "recentMax",
                    &RecentFilesModel::getRecentMax,
                    &RecentFilesModel::setRecentMax)
                .def_prop_ro("observeRecentMax", &RecentFilesModel::observeRecentMax)
                .def_prop_rw(
                    "recent",
                    [](RecentFilesModel& model)
                    {
                        std::vector<std::string> out;
                        for (const auto& recent : model.getRecent())
                        {
                            out.push_back(recent.get());
                        }
                        return out;
                    },
                    [](RecentFilesModel& model, const std::vector<std::string>& value)
                    {
                        std::vector<Path> paths;
                        for (const auto& i : value)
                        {
                            paths.push_back(Path(i));
                        }
                        model.setRecent(paths);
                    })
                .def_prop_ro("observeRecent", &RecentFilesModel::observeRecent)
                .def("addRecent", &RecentFilesModel::addRecent);
        }
    }
}
