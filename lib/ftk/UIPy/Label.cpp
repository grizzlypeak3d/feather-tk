// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Label.h>

#include <nanobind/nanobind.h>
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
        void label(nb::module_& m)
        {
            nb::class_<Label, IWidget>(m, "Label")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&Label::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&Label::create)),
                    nb::arg("context"),
                    nb::arg("text"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("text", &Label::getText, &Label::setText)
                .def_prop_rw("textRole", &Label::getTextRole, &Label::setTextRole)
                .def_prop_rw("marginRole", &Label::getMarginRole, nb::overload_cast<SizeRole>(&Label::setMarginRole))
                .def_prop_rw("hMarginRole", &Label::getHMarginRole, &Label::setHMarginRole)
                .def_prop_rw("vMarginRole", &Label::getVMarginRole, &Label::setVMarginRole)
                .def_prop_rw("hMarginRole", &Label::getHMarginRole, &Label::setHMarginRole)
                .def_prop_rw("vMarginRole", &Label::getVMarginRole, &Label::setVMarginRole)
                .def("setMarginRole", nb::overload_cast<SizeRole, SizeRole>(&Label::setMarginRole))
                .def_prop_rw("font", &Label::getFont, &Label::setFont)
                .def_prop_rw("fontSize", &Label::getFontSize, &Label::setFontSize);
        }
    }
}
