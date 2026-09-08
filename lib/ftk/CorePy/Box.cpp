// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Box.h>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

#include <sstream>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void box(nb::module_& m)
        {
            nb::class_<Box2I>(m, "Box2I")
                .def(nb::init<>())
                .def(nb::init<V2I, V2I>())
                .def(nb::init<V2I, Size2I>())
                .def(nb::init<int, int, int, int>())
                .def_rw("min", &Box2I::min)
                .def_rw("max", &Box2I::max)
                .def_prop_ro("x", &Box2I::x)
                .def_prop_ro("y", &Box2I::y)
                .def_prop_ro("size", &Box2I::size)
                .def_prop_ro("w", &Box2I::w)
                .def_prop_ro("h", &Box2I::h)
                .def_prop_ro("valid", &Box2I::isValid)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Box2I& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Box2F>(m, "Box2F")
                .def(nb::init<>())
                .def(nb::init<V2F, V2F>())
                .def(nb::init<V2F, Size2F>())
                .def(nb::init<float, float, float, float>())
                .def_rw("min", &Box2F::min)
                .def_rw("max", &Box2F::max)
                .def_prop_ro("x", &Box2F::x)
                .def_prop_ro("y", &Box2F::y)
                .def_prop_ro("size", &Box2F::size)
                .def_prop_ro("w", &Box2F::w)
                .def_prop_ro("h", &Box2F::h)
                .def_prop_ro("valid", &Box2F::isValid)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Box2F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Box3F>(m, "Box3F")
                .def(nb::init<>())
                .def(nb::init<V3F, V3F>())
                .def(nb::init<V3F, Size3F>())
                .def(nb::init<float, float, float, float, float, float>())
                .def_rw("min", &Box3F::min)
                .def_rw("max", &Box3F::max)
                .def_prop_ro("x", &Box3F::x)
                .def_prop_ro("y", &Box3F::y)
                .def_prop_ro("z", &Box3F::z)
                .def_prop_ro("size", &Box3F::size)
                .def_prop_ro("w", &Box3F::w)
                .def_prop_ro("h", &Box3F::h)
                .def_prop_ro("d", &Box3F::d)
                .def_prop_ro("valid", &Box3F::isValid)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Box3F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            m.def("center", [](const Box2I& v) { return center(v); });
            m.def("center", [](const Box2F& v) { return center(v); });
            m.def("center", [](const Box3F& v) { return center(v); });

            m.def("area", [](const Box2I& v) { return area(v); });
            m.def("area", [](const Box2F& v) { return area(v); });

            m.def("volume", [](const Box3F& v) { return volume(v); });

            m.def("move", [](const Box2I& v, const V2I& m) { return move(v, m); });
            m.def("move", [](const Box2F& v, const V2F& m) { return move(v, m); });
            m.def("move", [](const Box3F& v, const V3F& m) { return move(v, m); });

            m.def("contains",
                [](const Box2I& a, const Box2I& b) { return contains(a, b); });
            m.def("contains",
                [](const Box2F& a, const Box2F& b) { return contains(a, b); });
            m.def("contains",
                [](const Box2I& a, const V2I& b) { return contains(a, b); });
            m.def("contains",
                [](const Box2F& a, const V2F& b) { return contains(a, b); });

            m.def("intersects",
                [](const Box2I& a, const Box2I& b) { return intersects(a, b); });
            m.def("intersects",
                [](const Box2F& a, const Box2F& b) { return intersects(a, b); });

            m.def("intersect",
                [](const Box2I& a, const Box2I& b) { return intersect(a, b); });
            m.def("intersect",
                [](const Box2F& a, const Box2F& b) { return intersect(a, b); });

            m.def("expand",
                [](const Box2I& a, const Box2I& b) { return expand(a, b); });
            m.def("expand",
                [](const Box2F& a, const Box2F& b) { return expand(a, b); });
            m.def("expand",
                [](const Box2I& a, const V2I& b) { return expand(a, b); });
            m.def("expand",
                [](const Box2F& a, const V2F& b) { return expand(a, b); });

            m.def("margin",
                [](const Box2I& v, const V2I& m) { return margin(v, m); });
            m.def("margin",
                [](const Box2F& v, const V2F& m) { return margin(v, m); });
            m.def("margin",
                [](const Box2I& v, int m) { return margin(v, m); });
            m.def("margin",
                [](const Box2F& v, float m) { return margin(v, m); });
            m.def("margin",
                [](const Box2I& v, int x0, int y0, int x1, int y1) { return margin(v, x0, y0, x1, y1); });
            m.def("margin",
                [](const Box2F& v, float x0, float y0, float x1, float y1) { return margin(v, x0, y0, x1, y1); });

            m.def("bbox", [](const std::vector<Box2I>& v) { return bbox(v); });
            m.def("bbox", [](const std::vector<Box2F>& v) { return bbox(v); });
            m.def("bbox", [](const std::vector<Box3F>& v) { return bbox(v); });
            m.def("bbox", [](const std::vector<V2I>& v) { return bbox(v); });
            m.def("bbox", [](const std::vector<V2F>& v) { return bbox(v); });
            m.def("bbox", [](const std::vector<V3F>& v) { return bbox(v); });

            m.def("points", [](const Box2I& v) { return points(v); });
            m.def("points", [](const Box2F& v) { return points(v); });
            m.def("points", [](const Box3F& v) { return points(v); });

            m.def("convert", [](const Box2I& v) { return convert(v); });
            
            m.def("to_string", [](const Box2I& v) { return to_string(v); });
            m.def("to_string", [](const Box2F& v) { return to_string(v); });
            m.def("to_string", [](const Box3F& v) { return to_string(v); });
            
            m.def("from_string", [](const std::string& s, Box2I& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, Box2F& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, Box3F& v) { from_string(s, v); });
        }
    }
}
