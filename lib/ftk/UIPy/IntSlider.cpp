// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntSlider.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void intSlider(py::module_& m)
        {
            py::class_<IIntSlider, IMouseWidget, std::shared_ptr<IIntSlider> >(m, "IIntSlider")
                .def_property("value", &IIntSlider::getValue, &IIntSlider::setValue)
                .def("setCallback", &IIntSlider::setCallback)
                .def("setPressedCallback", &IIntSlider::setPressedCallback)
                .def_property("range", &IIntSlider::getRange, py::overload_cast<const RangeI&>(&IIntSlider::setRange))
                .def("setRange", py::overload_cast<int, int>(&IIntSlider::setRange))
                .def_property("step", &IIntSlider::getStep, &IIntSlider::setStep)
                .def_property("largeStep", &IIntSlider::getLargeStep, &IIntSlider::setLargeStep)
                .def_property("defaultValue", &IIntSlider::getDefaultValue, &IIntSlider::setDefaultValue)
                .def("getModel", &IIntSlider::getModel);

            py::class_<IntSlider, IIntSlider, std::shared_ptr<IntSlider> >(m, "IntSlider")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntSlider::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntSlider::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr);
        }
    }
}
