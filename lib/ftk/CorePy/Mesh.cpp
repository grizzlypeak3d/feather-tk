// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Mesh.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void mesh(nb::module_& m)
        {
            nb::class_<Vertex2>(m, "Vertex2")
                .def(nb::init<>())
                .def(nb::init<size_t>())
                .def(nb::init<size_t, size_t>())
                .def(nb::init<size_t, size_t, size_t>())
                .def_rw("v", &Vertex2::v)
                .def_rw("t", &Vertex2::t)
                .def_rw("c", &Vertex2::c);

            nb::class_<Vertex3>(m, "Vertex3")
                .def(nb::init<>())
                .def(nb::init<size_t>())
                .def(nb::init<size_t, size_t>())
                .def(nb::init<size_t, size_t, size_t>())
                .def(nb::init<size_t, size_t, size_t, size_t>())
                .def_rw("v", &Vertex3::v)
                .def_rw("t", &Vertex3::t)
                .def_rw("n", &Vertex3::n)
                .def_rw("c", &Vertex3::c);

            nb::class_<Triangle2>(m, "Triangle2")
                .def(nb::init<>())
                .def(nb::init<const Vertex2&, const Vertex2&, const Vertex2&>())
                .def_rw("v", &Triangle2::v);

            nb::class_<Triangle3>(m, "Triangle3")
                .def(nb::init<>())
                .def(nb::init<const Vertex3&, const Vertex3&, const Vertex3&>())
                .def_rw("v", &Triangle3::v);

            nb::class_<TriMesh2F>(m, "TriMesh2F")
                .def(nb::init<>())
                .def_rw("v", &TriMesh2F::v)
                .def_rw("c", &TriMesh2F::c)
                .def_rw("t", &TriMesh2F::t)
                .def_rw("triangles", &TriMesh2F::triangles)
                .def("getByteCount", &TriMesh2F::getByteCount);

            nb::class_<TriMesh3F>(m, "TriMesh3F")
                .def(nb::init<>())
                .def_rw("v", &TriMesh3F::v)
                .def_rw("c", &TriMesh3F::c)
                .def_rw("t", &TriMesh3F::t)
                .def_rw("n", &TriMesh3F::n)
                .def_rw("triangles", &TriMesh3F::triangles)
                .def("getByteCount", &TriMesh3F::getByteCount);

            m.def("edge", [](const V2F& p, const V2F& v0, const V2F& v1) { return edge(p, v0, v1); });

            m.def("bbox", [](const TriMesh3F& v) { return bbox(v); });

            m.def("mesh", [](const Box2I& v) { return ftk::mesh(v); });
            m.def("mesh", [](const Box2I& v, bool mirrorV) { return ftk::mesh(v, mirrorV); });
            m.def("mesh", [](const Box2F& v) { return ftk::mesh(v); });
            m.def("mesh", [](const Box2F& v, bool mirrorV) { return ftk::mesh(v, mirrorV); });

            m.def("checkers", [](const Box2I& box, const Color4F& color0, const Color4F& color1, const Size2I& checkerSize) { return checkers(box, color0, color1, checkerSize); });

            m.def("sphere", [](float radius, size_t xResolution, size_t yResolution) { return sphere(radius, xResolution, yResolution); });
        }
    }
}
