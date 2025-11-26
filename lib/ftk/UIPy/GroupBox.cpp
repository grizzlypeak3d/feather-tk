// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/GroupBox.h>

#include <ftk/UI/GroupBox.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void groupBox(py::module_& m)
        {
            py::class_<GroupBox, IWidget, std::shared_ptr<GroupBox> >(m, "GroupBox")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&GroupBox::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&GroupBox::create)),
                    py::arg("context"),
                    py::arg("text"),
                    py::arg("parent") = nullptr)
                .def_property("text", &GroupBox::getText, &GroupBox::setText)
                .def_property("fontRole", &GroupBox::getFontRole, &GroupBox::setFontRole)
                .def("clear", &GroupBox::clear);
        }
    }
}
