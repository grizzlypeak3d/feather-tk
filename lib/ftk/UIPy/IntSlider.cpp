// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntSlider.h>

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
        void intSlider(nb::module_& m)
        {
            nb::class_<IIntSlider, IMouseWidget>(m, "IIntSlider")
                .def_prop_rw("value", &IIntSlider::getValue, &IIntSlider::setValue)
                .def("setCallback", &IIntSlider::setCallback)
                .def("setPressedCallback", &IIntSlider::setPressedCallback)
                .def_prop_rw("range", &IIntSlider::getRange, nb::overload_cast<const RangeI&>(&IIntSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<int, int>(&IIntSlider::setRange))
                .def_prop_rw("step", &IIntSlider::getStep, &IIntSlider::setStep)
                .def_prop_rw("largeStep", &IIntSlider::getLargeStep, &IIntSlider::setLargeStep)
                .def_prop_rw("defaultValue", &IIntSlider::getDefault, &IIntSlider::setDefault)
                .def("getModel", &IIntSlider::getModel);

            nb::class_<IntSlider, IIntSlider>(m, "IntSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr);
        }
    }
}
