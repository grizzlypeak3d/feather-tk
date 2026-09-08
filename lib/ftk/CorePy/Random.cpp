// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Random.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void random(nb::module_& m)
        {
            nb::class_<Random>(m, "Random")
                .def(nb::init<>())
                .def("getF", nb::overload_cast<>(&Random::getF))
                .def("getF", nb::overload_cast<float>(&Random::getF))
                .def("getF", nb::overload_cast<float, float>(&Random::getF), nb::arg("min"), nb::arg("max"))
                .def("getI", nb::overload_cast<int>(&Random::getI))
                .def("getI", nb::overload_cast<int, int>(&Random::getI), nb::arg("min"), nb::arg("max"))
                .def("setSeed", nb::overload_cast<unsigned int>(&Random::setSeed))
                .def("setSeed", nb::overload_cast<>(&Random::setSeed));
        }
    }
}
