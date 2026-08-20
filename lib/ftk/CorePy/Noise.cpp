// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Noise.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void noise(py::module_& m)
        {
            py::class_<Noise>(m, "Noise")
                .def(py::init<>())
                .def("get", &Noise::get, py::arg("x"), py::arg("y"), py::arg("z"));
        }
    }
}
