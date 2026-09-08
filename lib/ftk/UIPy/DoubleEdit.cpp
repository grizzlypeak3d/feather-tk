// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleEdit.h>

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
        void doubleEdit(nb::module_& m)
        {
            nb::class_<DoubleEdit, IContainer>(m, "DoubleEdit")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEdit::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEdit::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &DoubleEdit::getValue, &DoubleEdit::setValue)
                .def("setCallback", &DoubleEdit::setCallback)
                .def_prop_rw("range", &DoubleEdit::getRange, nb::overload_cast<const RangeD&>(&DoubleEdit::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<double, double>(&DoubleEdit::setRange))
                .def_prop_rw("step", &DoubleEdit::getStep, &DoubleEdit::setStep)
                .def_prop_rw("largeStep", &DoubleEdit::getLargeStep, &DoubleEdit::setLargeStep)
                .def_prop_rw("defaultValue", &DoubleEdit::getDefault, &DoubleEdit::setDefault)
                .def("getModel", &DoubleEdit::getModel)
                .def_prop_rw("precision", &DoubleEdit::getPrecision, &DoubleEdit::setPrecision)
                .def_prop_rw("font", &DoubleEdit::getFont, &DoubleEdit::setFont)
                .def_prop_rw("borderRole", &DoubleEdit::getBorderRole, &DoubleEdit::setBorderRole);
        }
    }
}
