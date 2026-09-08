// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorSlider.h>

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
        void colorSlider(nb::module_& m)
        {
            nb::class_<ColorIntSlider, IIntSlider>(m, "ColorIntSlider")
                .def(
                    nb::new_(&ColorIntSlider::create),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("setColors", &ColorIntSlider::setColors);

            nb::class_<ColorIntEditSlider, IContainer>(m, "ColorIntEditSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ColorIntEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&ColorIntEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("getModel", &ColorIntEditSlider::getModel)
                .def("setColors", &ColorIntEditSlider::setColors)
                .def_prop_rw("value", &ColorIntEditSlider::getValue, &ColorIntEditSlider::setValue)
                .def("setCallback", &ColorIntEditSlider::setCallback)
                .def("setPressedCallback", &ColorIntEditSlider::setPressedCallback)
                .def_prop_rw("range", &ColorIntEditSlider::getRange, nb::overload_cast<const RangeI&>(&ColorIntEditSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<int, int>(&ColorIntEditSlider::setRange));

            nb::class_<ColorFloatSlider, IFloatSlider>(m, "ColorFloatSlider")
                .def(
                    nb::new_(&ColorFloatSlider::create),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("setColors", &ColorFloatSlider::setColors);

            nb::class_<ColorFloatEditSlider, IContainer>(m, "ColorFloatEditSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ColorFloatEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&ColorFloatEditSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def("getModel", &ColorFloatEditSlider::getModel)
                .def("setColors", &ColorFloatEditSlider::setColors)
                .def_prop_rw("value", &ColorFloatEditSlider::getValue, &ColorFloatEditSlider::setValue)
                .def("setCallback", &ColorFloatEditSlider::setCallback)
                .def("setPressedCallback", &ColorFloatEditSlider::setPressedCallback)
                .def_prop_rw("range", &ColorFloatEditSlider::getRange, nb::overload_cast<const RangeF&>(&ColorFloatEditSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&ColorFloatEditSlider::setRange));
        }
    }
}
