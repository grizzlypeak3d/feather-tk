// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/FileEdit.h>
#include <ftk/UI/RecentFilesModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void fileEdit(py::module_& m)
        {
            py::enum_<FileEditDisplay>(m, "Endian")
                .value("FullPath", FileEditDisplay::FullPath)
                .value("FileName", FileEditDisplay::FileName);
            FTK_ENUM_BIND(m, FileEditDisplay);

            py::class_<FileEdit, IWidget, std::shared_ptr<FileEdit> >(m, "FileEdit")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FileEdit::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        FileBrowserMode,
                        const std::shared_ptr<IWidget>&>(&FileEdit::create)),
                    py::arg("context"),
                    py::arg("mode"),
                    py::arg("parent") = nullptr)
                .def_property("path",
                    [](const std::shared_ptr<FileEdit>& w)
                    {
                        return w->getPath().get();
                    },
                    [](const std::shared_ptr<FileEdit>& w, const std::string& s)
                    {
                        w->setPath(Path(s));
                    })
                .def_property("display", &FileEdit::getDisplay, &FileEdit::setDisplay)
                .def("setCallback", &FileEdit::setCallback);
        }
    }
}
