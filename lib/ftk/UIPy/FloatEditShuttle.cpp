// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatEditShuttle.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void floatEditShuttle(py::module_& m)
        {
            py::class_<FloatEditShuttle, IContainer, std::shared_ptr<FloatEditShuttle> >(m, "FloatEditShuttle")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatEditShuttle::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatEditShuttle::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &FloatEditShuttle::getValue, &FloatEditShuttle::setValue)
                .def("setCallback", &FloatEditShuttle::setCallback)
                .def("setPressedCallback", &FloatEditShuttle::setPressedCallback)
                .def_property("range", &FloatEditShuttle::getRange, py::overload_cast<const RangeF&>(&FloatEditShuttle::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<float, float>(&FloatEditShuttle::setRange))
                .def_property("step", &FloatEditShuttle::getStep, &FloatEditShuttle::setStep)
                .def_property("largeStep", &FloatEditShuttle::getLargeStep, &FloatEditShuttle::setLargeStep)
                .def_property("defaultValue", &FloatEditShuttle::getDefault, &FloatEditShuttle::setDefault)
                .def("getModel", &FloatEditShuttle::getModel)
                .def_property("precision", &FloatEditShuttle::getPrecision, &FloatEditShuttle::setPrecision)
                .def_property("font", &FloatEditShuttle::getFont, &FloatEditShuttle::setFont);
        }
    }
}
