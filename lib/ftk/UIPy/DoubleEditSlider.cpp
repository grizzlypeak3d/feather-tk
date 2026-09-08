// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleEditSlider.h>

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
        void doubleEditSlider(nb::module_& m)
        {
            nb::class_<DoubleEditSlider, IContainer>(m, "DoubleEditSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &DoubleEditSlider::getValue, &DoubleEditSlider::setValue)
                .def("setCallback", &DoubleEditSlider::setCallback)
                .def_prop_rw("range", &DoubleEditSlider::getRange, nb::overload_cast<const RangeD&>(&DoubleEditSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<double, double>(&DoubleEditSlider::setRange))
                .def_prop_rw("step", &DoubleEditSlider::getStep, &DoubleEditSlider::setStep)
                .def_prop_rw("largeStep", &DoubleEditSlider::getLargeStep, &DoubleEditSlider::setLargeStep)
                .def_prop_rw("defaultValue", &DoubleEditSlider::getDefault, &DoubleEditSlider::setDefault)
                .def("getModel", &DoubleEditSlider::getModel)
                .def("setPressedCallback", &DoubleEditSlider::setPressedCallback)
                .def_prop_rw("precision", &DoubleEditSlider::getPrecision, &DoubleEditSlider::setPrecision)
                .def_prop_rw("font", &DoubleEditSlider::getFont, &DoubleEditSlider::setFont);

        }
    }
}
