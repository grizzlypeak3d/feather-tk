// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntEditShuttle.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void intEditShuttle(py::module_& m)
        {
            py::class_<IntEditShuttle, IContainer, std::shared_ptr<IntEditShuttle> >(m, "IntEditShuttle")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntEditShuttle::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntEditShuttle::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &IntEditShuttle::getValue, &IntEditShuttle::setValue)
                .def("setCallback", &IntEditShuttle::setCallback)
                .def("setPressedCallback", &IntEditShuttle::setPressedCallback)
                .def_property("range", &IntEditShuttle::getRange, py::overload_cast<const RangeI&>(&IntEditShuttle::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<int, int>(&IntEditShuttle::setRange))
                .def_property("step", &IntEditShuttle::getStep, &IntEditShuttle::setStep)
                .def_property("largeStep", &IntEditShuttle::getLargeStep, &IntEditShuttle::setLargeStep)
                .def_property("defaultValue", &IntEditShuttle::getDefault, &IntEditShuttle::setDefault)
                .def("getModel", &IntEditShuttle::getModel)
                .def_property("font", &IntEditShuttle::getFont, &IntEditShuttle::setFont);
        }
    }
}
