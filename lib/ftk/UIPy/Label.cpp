// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Label.h>

#include <ftk/UI/Label.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void label(py::module_& m)
        {
            py::class_<Label, IWidget, std::shared_ptr<Label> >(m, "Label")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&Label::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&Label::create)),
                    py::arg("context"),
                    py::arg("text"),
                    py::arg("parent") = nullptr)
                .def_property("text", &Label::getText, &Label::setText)
                .def_property("textRole", &Label::getTextRole, &Label::setTextRole)
                .def_property("marginRole", &Label::getMarginRole, py::overload_cast<SizeRole>(&Label::setMarginRole))
                .def_property("hMarginRole", &Label::getHMarginRole, &Label::setHMarginRole)
                .def_property("vMarginRole", &Label::getVMarginRole, &Label::setVMarginRole)
                .def("setMarginRole", py::overload_cast<SizeRole, SizeRole>(&Label::setMarginRole))
                .def_property("fontRole", &Label::getFontRole, &Label::setFontRole)
                .def_property("fontInfo", &Label::getFontInfo, &Label::setFontInfo);
        }
    }
}
