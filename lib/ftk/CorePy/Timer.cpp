// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Timer.h>

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void timer(py::module_& m)
        {
            py::class_<Timer, std::shared_ptr<Timer> >(m, "Timer")
                .def(
                    py::init(&Timer::create),
                    py::arg("context"))
                .def_property("repeating", &Timer::isRepeating, &Timer::setRepeating)
                .def("start", [](
                    const std::shared_ptr<Timer>& timer,
                    float seconds,
                    const std::function<void(void)>& callback)
                    {
                        timer->start(std::chrono::milliseconds(long(seconds * 1000.0)), callback);
                    })
                .def("stop", &Timer::stop)
                .def_property_readonly("active", &Timer::isActive)
                .def_property_readonly("timeout", &Timer::getTimeout);
        }
    }
}
