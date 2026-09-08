// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/FileEdit.h>
#include <ftk/UI/RecentFilesModel.h>

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

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void fileEdit(nb::module_& m)
        {
            nb::class_<FileEdit, IContainer>(m, "FileEdit")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FileEdit::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        FileBrowserMode,
                        const std::shared_ptr<IWidget>&>(&FileEdit::create)),
                    nb::arg("context"),
                    nb::arg("mode"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("path",
                    [](const std::shared_ptr<FileEdit>& w)
                    {
                        return w->getPath().get();
                    },
                    [](const std::shared_ptr<FileEdit>& w, const std::string& s)
                    {
                        w->setPath(Path(s));
                    })
                .def("setCallback", &FileEdit::setCallback);
        }
    }
}
