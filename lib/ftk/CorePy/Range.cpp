// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Range.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/operators.h>

#include <sstream>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void range(nb::module_& m)
        {
            nb::class_<RangeI>(m, "RangeI")
                .def(nb::init<>())
                .def(nb::init<int, int>())
                .def_prop_ro("min", &RangeI::min)
                .def_prop_ro("max", &RangeI::max)
                .def_prop_ro("equal", &RangeI::equal)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self)
                .def(nanobind::self < nanobind::self)
                .def("__repr__", [](const RangeI& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<RangeI64>(m, "RangeI64")
                .def(nb::init<>())
                .def(nb::init<int64_t, int64_t>())
                .def_prop_ro("min", &RangeI64::min)
                .def_prop_ro("max", &RangeI64::max)
                .def_prop_ro("equal", &RangeI64::equal)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self)
                .def(nanobind::self < nanobind::self)
                .def("__repr__", [](const RangeI64& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<RangeF>(m, "RangeF")
                .def(nb::init<>())
                .def(nb::init<float, float>())
                .def_prop_ro("min", &RangeF::min)
                .def_prop_ro("max", &RangeF::max)
                .def_prop_ro("equal", &RangeF::equal)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self)
                .def(nanobind::self < nanobind::self)
                .def("__repr__", [](const RangeF& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<RangeD>(m, "RangeD")
                .def(nb::init<>())
                .def(nb::init<double, double>())
                .def_prop_ro("min", &RangeD::min)
                .def_prop_ro("max", &RangeD::max)
                .def_prop_ro("equal", &RangeD::equal)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self)
                .def(nanobind::self < nanobind::self)
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
