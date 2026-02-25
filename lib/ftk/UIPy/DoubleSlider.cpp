// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleSlider.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void doubleSlider(py::module_& m)
        {
            py::class_<IDoubleSlider, IMouseWidget, std::shared_ptr<IDoubleSlider> >(m, "IDoubleSlider")
                .def_property("value", &IDoubleSlider::getValue, &IDoubleSlider::setValue)
                .def("setCallback", &IDoubleSlider::setCallback)
                .def("setPressedCallback", &IDoubleSlider::setPressedCallback)
                .def_property("range", &IDoubleSlider::getRange, py::overload_cast<const RangeD&>(&IDoubleSlider::setRange))
                .def("setRange", py::overload_cast<double, double>(&IDoubleSlider::setRange))
                .def_property("step", &IDoubleSlider::getStep, &IDoubleSlider::setStep)
                .def_property("largeStep", &IDoubleSlider::getLargeStep, &IDoubleSlider::setLargeStep)
                .def_property("defaultValue", &IDoubleSlider::getDefaultValue, &IDoubleSlider::setDefaultValue)
                .def("getModel", &IDoubleSlider::getModel);

            py::class_<DoubleSlider, IDoubleSlider, std::shared_ptr<DoubleSlider> >(m, "DoubleSlider")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleSlider::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleSlider::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr);
        }
    }
}
