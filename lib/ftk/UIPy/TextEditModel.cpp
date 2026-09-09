// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/TextEditModel.h>

#include <ftk/CorePy/Bindings.h>

#include <nanobind/stl/function.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
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
        void textEditModel(nb::module_& m)
        {
            nb::class_<TextEditPos>(m, "TextEditPos")
                .def(nb::init<>())
                .def(nb::init<int, int>(), nb::arg("line"), nb::arg("chr"))
                .def_rw("line", &TextEditPos::line)
                .def_rw("chr", &TextEditPos::chr)
                .def("isValid", &TextEditPos::isValid)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self)
                .def("__lt__", &TextEditPos::operator<)
                .def("__gt__", &TextEditPos::operator>);

            nb::class_<TextEditSelection>(m, "TextEditSelection")
                .def(nb::init<>())
                .def(nb::init<const TextEditPos&>(), nb::arg("pos"))
                .def(nb::init<const TextEditPos&, const TextEditPos&>(),
                    nb::arg("first"), nb::arg("second"))
                .def_rw("first", &TextEditSelection::first)
                .def_rw("second", &TextEditSelection::second)
                .def("isValid", &TextEditSelection::isValid)
                .def("min", &TextEditSelection::min)
                .def("max", &TextEditSelection::max)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<TextEditModelOptions>(m, "TextEditModelOptions")
                .def(nb::init<>())
                .def_rw("tabSpaces", &TextEditModelOptions::tabSpaces)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            ftk::python::observable<TextEditPos>(m, "TextEditPos");
            ftk::python::observable<TextEditSelection>(m, "TextEditSelection");

            nb::class_<TextEditModel>(m, "TextEditModel")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::vector<std::string>&>(&TextEditModel::create)),
                    nb::arg("context"),
                    nb::arg("text") = std::vector<std::string>{})
                .def_prop_rw("text", &TextEditModel::getText, &TextEditModel::setText)
                .def("observeText", &TextEditModel::observeText)
                .def("clearText", &TextEditModel::clearText)
                .def_prop_rw("readOnly", &TextEditModel::isReadOnly, &TextEditModel::setReadOnly)
                .def("observeReadOnly", &TextEditModel::observeReadOnly)
                .def_prop_rw("cursor", &TextEditModel::getCursor, &TextEditModel::setCursor, nb::rv_policy::copy)
                .def("observeCursor", &TextEditModel::observeCursor)
                .def_prop_rw("selection", &TextEditModel::getSelection, &TextEditModel::setSelection, nb::rv_policy::copy)
                .def("observeSelection", &TextEditModel::observeSelection)
                .def("selectAll", &TextEditModel::selectAll)
                .def("clearSelection", &TextEditModel::clearSelection)
                .def("undo", &TextEditModel::undo)
                .def("observeHasUndo", &TextEditModel::observeHasUndo)
                .def("observeHasRedo", &TextEditModel::observeHasRedo)
                .def("redo", &TextEditModel::redo)
                .def("cut", &TextEditModel::cut)
                .def("copy", &TextEditModel::copy)
                .def("paste", &TextEditModel::paste)
                .def("input", &TextEditModel::input, nb::arg("text"))
                .def("key", &TextEditModel::key, nb::arg("key"), nb::arg("modifiers") = 0)
                .def_prop_rw("options", &TextEditModel::getOptions, &TextEditModel::setOptions, nb::rv_policy::copy)
                .def("observeOptions", &TextEditModel::observeOptions)
                .def("setPageRows", &TextEditModel::setPageRows, nb::arg("rows"));
        }
    }
}
