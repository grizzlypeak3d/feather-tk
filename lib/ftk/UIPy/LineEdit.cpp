// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/LineEdit.h>
#include <ftk/UI/LineEditModel.h>

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
        void lineEdit(nb::module_& m)
        {
            nb::class_<LineEdit, IWidget>(m, "LineEdit")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&LineEdit::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<LineEditModel>&,
                        const std::shared_ptr<IWidget>&>(&LineEdit::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("getModel", &LineEdit::getModel)
                .def_prop_rw("text", &LineEdit::getText, &LineEdit::setText)
                .def("clearText", &LineEdit::clearText)
                .def("setCallback", &LineEdit::setCallback)
                .def_prop_ro("hasCallbackOnFocusLost", &LineEdit::hasCallbackOnFocusLost)
                .def("setCallbackOnFocusLost", &LineEdit::setCallbackOnFocusLost)
                .def("setTextChangedCallback", &LineEdit::setTextChangedCallback)
                .def_prop_rw("format", &LineEdit::getFormat, &LineEdit::setFormat)
                .def("setFocusCallback", &LineEdit::setFocusCallback)
                .def_prop_rw("readOnly", &LineEdit::isReadOnly, &LineEdit::setReadOnly)
                .def_prop_rw("selectAllOnFocus", &LineEdit::hasSelectAllOnFocus, &LineEdit::setSelectAllOnFocus)
                .def_prop_ro("preedit", &LineEdit::getPreedit)
                .def("selectAll", &LineEdit::selectAll)
                .def("clearSelection", &LineEdit::clearSelection)
                .def_prop_rw("font", &LineEdit::getFont, &LineEdit::setFont)
                .def_prop_rw("borderRole", &LineEdit::getBorderRole, &LineEdit::setBorderRole);
        }
    }
}
