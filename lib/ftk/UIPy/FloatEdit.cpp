// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatEdit.h>

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
        void floatEdit(nb::module_& m)
        {
            nb::class_<FloatEdit, IContainer>(m, "FloatEdit")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatEdit::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatEdit::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &FloatEdit::getValue, &FloatEdit::setValue)
                .def("setCallback", &FloatEdit::setCallback)
                .def_prop_rw("range", &FloatEdit::getRange, nb::overload_cast<const RangeF&>(&FloatEdit::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&FloatEdit::setRange))
                .def_prop_rw("step", &FloatEdit::getStep, &FloatEdit::setStep)
                .def_prop_rw("largeStep", &FloatEdit::getLargeStep, &FloatEdit::setLargeStep)
                .def_prop_rw("defaultValue", &FloatEdit::getDefault, &FloatEdit::setDefault)
                .def("getModel", &FloatEdit::getModel)
                .def_prop_rw("precision", &FloatEdit::getPrecision, &FloatEdit::setPrecision)
                .def_prop_rw("font", &FloatEdit::getFont, &FloatEdit::setFont);
        }
    }
}
