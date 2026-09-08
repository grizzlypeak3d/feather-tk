// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatEditSlider.h>

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
        void floatEditSlider(nb::module_& m)
        {
            nb::class_<FloatEditSlider, IContainer>(m, "FloatEditSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &FloatEditSlider::getValue, &FloatEditSlider::setValue)
                .def("setCallback", &FloatEditSlider::setCallback)
                .def_prop_rw("range", &FloatEditSlider::getRange, nb::overload_cast<const RangeF&>(&FloatEditSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&FloatEditSlider::setRange))
                .def_prop_rw("step", &FloatEditSlider::getStep, &FloatEditSlider::setStep)
                .def_prop_rw("largeStep", &FloatEditSlider::getLargeStep, &FloatEditSlider::setLargeStep)
                .def_prop_rw("defaultValue", &FloatEditSlider::getDefault, &FloatEditSlider::setDefault)
                .def("getModel", &FloatEditSlider::getModel)
                .def("setPressedCallback", &FloatEditSlider::setPressedCallback)
                .def_prop_rw("precision", &FloatEditSlider::getPrecision, &FloatEditSlider::setPrecision)
                .def_prop_rw("font", &FloatEditSlider::getFont, &FloatEditSlider::setFont);
        }
    }
}
