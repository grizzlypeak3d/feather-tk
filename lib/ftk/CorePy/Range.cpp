// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Range.h>

#include <ftk/Core/Range.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void range(py::module_& m)
        {
            py::class_<RangeI>(m, "RangeI")
                .def(py::init<>())
                .def(py::init<int, int>())
                .def_property_readonly("min", &RangeI::min)
                .def_property_readonly("max", &RangeI::max)
                .def_property_readonly("equal", &RangeI::equal)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def(pybind11::self < pybind11::self)
                .def("__repr__", [](const RangeI& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<RangeI64>(m, "RangeI64")
                .def(py::init<>())
                .def(py::init<int64_t, int64_t>())
                .def_property_readonly("min", &RangeI64::min)
                .def_property_readonly("max", &RangeI64::max)
                .def_property_readonly("equal", &RangeI64::equal)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def(pybind11::self < pybind11::self)
                .def("__repr__", [](const RangeI64& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<RangeF>(m, "RangeF")
                .def(py::init<>())
                .def(py::init<float, float>())
                .def_property_readonly("min", &RangeF::min)
                .def_property_readonly("max", &RangeF::max)
                .def_property_readonly("equal", &RangeF::equal)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def(pybind11::self < pybind11::self)
                .def("__repr__", [](const RangeF& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<RangeD>(m, "RangeD")
                .def(py::init<>())
                .def(py::init<double, double>())
                .def_property_readonly("min", &RangeD::min)
                .def_property_readonly("max", &RangeD::max)
                .def_property_readonly("equal", &RangeD::equal)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self)
                .def(pybind11::self < pybind11::self)
                .def("__repr__", [](const RangeD& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            m.def("contains", [](const RangeI& a, int b) { return contains(a, b); });
            m.def("contains", [](const RangeI64& a, int64_t b) { return contains(a, b); });
            m.def("contains", [](const RangeF& a, float b) { return contains(a, b); });
            m.def("contains", [](const RangeD& a, double b) { return contains(a, b); });

            m.def("intersects", [](const RangeI& a, const RangeI& b) { return intersects(a, b); });
            m.def("intersects", [](const RangeI64& a, const RangeI64& b) { return intersects(a, b); });
            m.def("intersects", [](const RangeF& a, const RangeF& b) { return intersects(a, b); });
            m.def("intersects", [](const RangeD& a, const RangeD& b) { return intersects(a, b); });

            m.def("expand", [](const RangeI& a, int b) { return expand(a, b); });
            m.def("expand", [](const RangeI64& a, int64_t b) { return expand(a, b); });
            m.def("expand", [](const RangeF& a, float b) { return expand(a, b); });
            m.def("expand", [](const RangeD& a, double b) { return expand(a, b); });
        }
    }
}
