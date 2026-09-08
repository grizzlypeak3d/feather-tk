// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntEditSlider.h>

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
        void intEditSlider(nb::module_& m)
        {
            nb::class_<IntEditSlider, IContainer>(m, "IntEditSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &IntEditSlider::getValue, &IntEditSlider::setValue)
                .def("setCallback", &IntEditSlider::setCallback)
                .def_prop_rw("range", &IntEditSlider::getRange, nb::overload_cast<const RangeI&>(&IntEditSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<int, int>(&IntEditSlider::setRange))
                .def_prop_rw("step", &IntEditSlider::getStep, &IntEditSlider::setStep)
                .def_prop_rw("largeStep", &IntEditSlider::getLargeStep, &IntEditSlider::setLargeStep)
                .def_prop_rw("defaultValue", &IntEditSlider::getDefault, &IntEditSlider::setDefault)
                .def("getModel", &IntEditSlider::getModel)
                .def("setPressedCallback", &IntEditSlider::setPressedCallback)
                .def_prop_rw("font", &IntEditSlider::getFont, &IntEditSlider::setFont);
        }
    }
}
