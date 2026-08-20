// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleEditShuttle.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void doubleEditShuttle(py::module_& m)
        {
            py::class_<DoubleEditShuttle, IContainer, std::shared_ptr<DoubleEditShuttle> >(m, "DoubleEditShuttle")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEditShuttle::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEditShuttle::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &DoubleEditShuttle::getValue, &DoubleEditShuttle::setValue)
                .def("setCallback", &DoubleEditShuttle::setCallback)
                .def("setPressedCallback", &DoubleEditShuttle::setPressedCallback)
                .def_property("range", &DoubleEditShuttle::getRange, py::overload_cast<const RangeD&>(&DoubleEditShuttle::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<double, double>(&DoubleEditShuttle::setRange))
                .def_property("step", &DoubleEditShuttle::getStep, &DoubleEditShuttle::setStep)
                .def_property("largeStep", &DoubleEditShuttle::getLargeStep, &DoubleEditShuttle::setLargeStep)
                .def_property("defaultValue", &DoubleEditShuttle::getDefault, &DoubleEditShuttle::setDefault)
                .def("getModel", &DoubleEditShuttle::getModel)
                .def_property("precision", &DoubleEditShuttle::getPrecision, &DoubleEditShuttle::setPrecision)
                .def_property("font", &DoubleEditShuttle::getFont, &DoubleEditShuttle::setFont);
        }
    }
}
