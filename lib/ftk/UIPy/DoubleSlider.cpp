// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleSlider.h>

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
        void doubleSlider(nb::module_& m)
        {
            nb::class_<IDoubleSlider, IMouseWidget>(m, "IDoubleSlider")
                .def_prop_rw("value", &IDoubleSlider::getValue, &IDoubleSlider::setValue)
                .def("setCallback", &IDoubleSlider::setCallback)
                .def("setPressedCallback", &IDoubleSlider::setPressedCallback)
                .def_prop_rw("range", &IDoubleSlider::getRange, nb::overload_cast<const RangeD&>(&IDoubleSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<double, double>(&IDoubleSlider::setRange))
                .def_prop_rw("step", &IDoubleSlider::getStep, &IDoubleSlider::setStep)
                .def_prop_rw("largeStep", &IDoubleSlider::getLargeStep, &IDoubleSlider::setLargeStep)
                .def_prop_rw("defaultValue", &IDoubleSlider::getDefault, &IDoubleSlider::setDefault)
                .def("getModel", &IDoubleSlider::getModel);

            nb::class_<DoubleSlider, IDoubleSlider>(m, "DoubleSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr);
        }
    }
}
