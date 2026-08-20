// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorSlider.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void colorSlider(py::module_& m)
        {
            py::class_<ColorIntSlider, IIntSlider, std::shared_ptr<ColorIntSlider> >(m, "ColorIntSlider")
                .def(
                    py::init(&ColorIntSlider::create),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("setColors", &ColorIntSlider::setColors);

            py::class_<ColorIntEditSlider, IContainer, std::shared_ptr<ColorIntEditSlider> >(m, "ColorIntEditSlider")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ColorIntEditSlider::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&ColorIntEditSlider::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("getModel", &ColorIntEditSlider::getModel)
                .def("setColors", &ColorIntEditSlider::setColors)
                .def_property("value", &ColorIntEditSlider::getValue, &ColorIntEditSlider::setValue)
                .def("setCallback", &ColorIntEditSlider::setCallback)
                .def("setPressedCallback", &ColorIntEditSlider::setPressedCallback)
                .def_property("range", &ColorIntEditSlider::getRange, py::overload_cast<const RangeI&>(&ColorIntEditSlider::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<int, int>(&ColorIntEditSlider::setRange));

            py::class_<ColorFloatSlider, IFloatSlider, std::shared_ptr<ColorFloatSlider> >(m, "ColorFloatSlider")
                .def(
                    py::init(&ColorFloatSlider::create),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("setColors", &ColorFloatSlider::setColors);

            py::class_<ColorFloatEditSlider, IContainer, std::shared_ptr<ColorFloatEditSlider> >(m, "ColorFloatEditSlider")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ColorFloatEditSlider::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&ColorFloatEditSlider::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("getModel", &ColorFloatEditSlider::getModel)
                .def("setColors", &ColorFloatEditSlider::setColors)
                .def_property("value", &ColorFloatEditSlider::getValue, &ColorFloatEditSlider::setValue)
                .def("setCallback", &ColorFloatEditSlider::setCallback)
                .def("setPressedCallback", &ColorFloatEditSlider::setPressedCallback)
                .def_property("range", &ColorFloatEditSlider::getRange, py::overload_cast<const RangeF&>(&ColorFloatEditSlider::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<float, float>(&ColorFloatEditSlider::setRange));
        }
    }
}
