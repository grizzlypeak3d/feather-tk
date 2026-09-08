// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatSlider.h>

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
        void floatSlider(nb::module_& m)
        {
            nb::class_<IFloatSlider, IMouseWidget>(m, "IFloatSlider")
                .def_prop_rw("value", &IFloatSlider::getValue, &IFloatSlider::setValue)
                .def("setCallback", &IFloatSlider::setCallback)
                .def("setPressedCallback", &IFloatSlider::setPressedCallback)
                .def_prop_rw("range", &IFloatSlider::getRange, nb::overload_cast<const RangeF&>(&IFloatSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&IFloatSlider::setRange))
                .def_prop_rw("step", &IFloatSlider::getStep, &IFloatSlider::setStep)
                .def_prop_rw("largeStep", &IFloatSlider::getLargeStep, &IFloatSlider::setLargeStep)
                .def_prop_rw("defaultValue", &IFloatSlider::getDefault, &IFloatSlider::setDefault)
                .def("getModel", &IFloatSlider::getModel);

            nb::class_<FloatSlider, IFloatSlider>(m, "FloatSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr);
        }
    }
}
