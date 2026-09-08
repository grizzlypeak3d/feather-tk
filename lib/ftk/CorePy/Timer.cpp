// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Timer.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/chrono.h>
#include <nanobind/stl/function.h>
#include <nanobind/operators.h>
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
        void timer(nb::module_& m)
        {
            nb::class_<Timer>(m, "Timer")
                .def(
                    nb::new_(&Timer::create),
                    nb::arg("context"))
                .def_prop_rw("repeating", &Timer::isRepeating, &Timer::setRepeating)
                .def("start", [](
                    const std::shared_ptr<Timer>& timer,
                    float seconds,
                    const std::function<void(void)>& callback)
                    {
                        timer->start(std::chrono::milliseconds(long(seconds * 1000.0)), callback);
                    })
                .def("stop", &Timer::stop)
                .def_prop_ro("active", &Timer::isActive)
                .def_prop_ro("timeout", &Timer::getTimeout);
        }
    }
}
