// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Noise.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void noise(nb::module_& m)
        {
            nb::class_<Noise>(m, "Noise")
                .def(nb::init<>())
                .def("get", &Noise::get, nb::arg("x"), nb::arg("y"), nb::arg("z"));
        }
    }
}
