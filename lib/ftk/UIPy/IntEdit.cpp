// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntEdit.h>

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
        void intEdit(nb::module_& m)
        {
            nb::class_<IntEdit, IContainer>(m, "IntEdit")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntEdit::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntEdit::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &IntEdit::getValue, &IntEdit::setValue)
                .def("setCallback", &IntEdit::setCallback)
                .def_prop_rw("range", &IntEdit::getRange, nb::overload_cast<const RangeI&>(&IntEdit::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<int, int>(&IntEdit::setRange))
                .def_prop_rw("step", &IntEdit::getStep, &IntEdit::setStep)
                .def_prop_rw("largeStep", &IntEdit::getLargeStep, &IntEdit::setLargeStep)
                .def_prop_rw("defaultValue", &IntEdit::getDefault, &IntEdit::setDefault)
                .def("getModel", &IntEdit::getModel)
                .def_prop_rw("font", &IntEdit::getFont, &IntEdit::setFont);
        }
    }
}
