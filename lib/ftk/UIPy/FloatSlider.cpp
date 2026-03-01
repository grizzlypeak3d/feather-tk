// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatSlider.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void floatSlider(py::module_& m)
        {
            py::class_<IFloatSlider, IMouseWidget, std::shared_ptr<IFloatSlider> >(m, "IFloatSlider")
                .def_property("value", &IFloatSlider::getValue, &IFloatSlider::setValue)
                .def("setCallback", &IFloatSlider::setCallback)
                .def("setPressedCallback", &IFloatSlider::setPressedCallback)
                .def_property("range", &IFloatSlider::getRange, py::overload_cast<const RangeF&>(&IFloatSlider::setRange))
                .def("setRange", py::overload_cast<float, float>(&IFloatSlider::setRange))
                .def_property("step", &IFloatSlider::getStep, &IFloatSlider::setStep)
                .def_property("largeStep", &IFloatSlider::getLargeStep, &IFloatSlider::setLargeStep)
                .def_property("defaultValue", &IFloatSlider::getDefault, &IFloatSlider::setDefault)
                .def("getModel", &IFloatSlider::getModel);

            py::class_<FloatSlider, IFloatSlider, std::shared_ptr<FloatSlider> >(m, "FloatSlider")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatSlider::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatSlider::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr);
        }
    }
}
