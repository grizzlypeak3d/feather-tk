// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/LineEditModel.h>

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
        void lineEditModel(nb::module_& m)
        {
            nb::class_<LineEditSelection>(m, "LineEditSelection")
                .def(nb::init<>())
                .def(nb::init<int>(), nb::arg("pos"))
                .def(nb::init<int, int>(), nb::arg("first"), nb::arg("second"))
                .def_rw("first", &LineEditSelection::first)
                .def_rw("second", &LineEditSelection::second)
                .def("isValid", &LineEditSelection::isValid)
                .def("min", &LineEditSelection::min)
                .def("max", &LineEditSelection::max)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<LineEditModel>(m, "LineEditModel")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&>(&LineEditModel::create)),
                    nb::arg("context"),
                    nb::arg("text") = std::string())
                .def_prop_rw("text", &LineEditModel::getText, &LineEditModel::setText)
                .def("observeText", &LineEditModel::observeText)
                .def("clearText", &LineEditModel::clearText)
                .def_prop_rw("readOnly", &LineEditModel::isReadOnly, &LineEditModel::setReadOnly)
                .def("observeReadOnly", &LineEditModel::observeReadOnly)
                .def_prop_rw("cursor", &LineEditModel::getCursor, &LineEditModel::setCursor)
                .def("observeCursor", &LineEditModel::observeCursor)
                .def_prop_rw("selection", &LineEditModel::getSelection, &LineEditModel::setSelection, nb::rv_policy::copy)
                .def("observeSelection", &LineEditModel::observeSelection)
                .def("selectAll", &LineEditModel::selectAll)
                .def("clearSelection", &LineEditModel::clearSelection)
                .def("undo", &LineEditModel::undo)
                .def("redo", &LineEditModel::redo)
                .def("cut", &LineEditModel::cut)
                .def("copy", &LineEditModel::copy)
                .def("paste", &LineEditModel::paste)
                .def("input", &LineEditModel::input, nb::arg("text"))
                .def("key", &LineEditModel::key, nb::arg("key"), nb::arg("modifiers") = 0)
                .def_prop_rw("regex", &LineEditModel::getRegex, &LineEditModel::setRegex);
        }
    }
}
