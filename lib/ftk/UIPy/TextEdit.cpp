// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/TextEdit.h>
#include <ftk/UI/TextEditModel.h>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
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
        void textEdit(nb::module_& m)
        {
            nb::class_<TextEditOptions>(m, "TextEditOptions")
                .def(nb::init<>())
                .def_rw("fontInfo", &TextEditOptions::fontInfo)
                .def_rw("cursorBlink", &TextEditOptions::cursorBlink)
                .def_rw("autoScrollTimeout", &TextEditOptions::autoScrollTimeout)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<TextEdit, IWidget>(
                m, "TextEdit",
                // The Python examples hold this through weakref, which
                // nanobind classes opt into.
                nb::is_weak_referenceable())
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&TextEdit::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<TextEditModel>&,
                        const std::shared_ptr<IWidget>&>(&TextEdit::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("getModel", &TextEdit::getModel)
                .def("getScrollWidget", &TextEdit::getScrollWidget)
                .def_prop_rw("text", &TextEdit::getText, &TextEdit::setText)
                .def("clearText", &TextEdit::clearText)
                .def("setCallback", &TextEdit::setCallback)
                .def("setFocusCallback", &TextEdit::setFocusCallback)
                .def_prop_rw("readOnly", &TextEdit::isReadOnly, &TextEdit::setReadOnly)
                .def("selectAll", &TextEdit::selectAll)
                .def("clearSelection", &TextEdit::clearSelection)
                .def_prop_rw("options", &TextEdit::getOptions, &TextEdit::setOptions, nb::rv_policy::copy)
                .def("observeOptions", &TextEdit::observeOptions)
                .def_prop_rw("sizeHintRole",
                    &TextEdit::getSizeHintRole,
                    &TextEdit::setSizeHintRole)
                .def_prop_rw("marginRole",
                    &TextEdit::getMarginRole,
                    &TextEdit::setMarginRole);
        }
    }
}
