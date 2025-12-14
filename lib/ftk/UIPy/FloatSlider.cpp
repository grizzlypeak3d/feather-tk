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
            py::class_<FloatSlider, IWidget, std::shared_ptr<FloatSlider> >(m, "FloatSlider")
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
                    py::arg("parent") = nullptr)
                .def_property("value", &FloatSlider::getValue, &FloatSlider::setValue)
                .def("setCallback", &FloatSlider::setCallback)
                .def_property("range", &FloatSlider::getRange, py::overload_cast<const RangeF&>(&FloatSlider::setRange))
                .def("setRange", py::overload_cast<float, float>(&FloatSlider::setRange))
                .def_property("step", &FloatSlider::getStep, &FloatSlider::setStep)
                .def_property("largeStep", &FloatSlider::getLargeStep, &FloatSlider::setLargeStep)
                .def_property("defaultValue", &FloatSlider::getDefaultValue, &FloatSlider::setDefaultValue)
                .def("getModel", &FloatSlider::getModel);
        }
    }
}
