// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Mesh.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void mesh(py::module_& m)
        {
            py::class_<Vertex2>(m, "Vertex2")
                .def(py::init<>())
                .def(py::init<size_t>())
                .def(py::init<size_t, size_t>())
                .def(py::init<size_t, size_t, size_t>())
                .def_readwrite("v", &Vertex2::v)
                .def_readwrite("t", &Vertex2::t)
                .def_readwrite("c", &Vertex2::c);

            py::class_<Vertex3>(m, "Vertex3")
                .def(py::init<>())
                .def(py::init<size_t>())
                .def(py::init<size_t, size_t>())
                .def(py::init<size_t, size_t, size_t>())
                .def(py::init<size_t, size_t, size_t, size_t>())
                .def_readwrite("v", &Vertex3::v)
                .def_readwrite("t", &Vertex3::t)
                .def_readwrite("n", &Vertex3::n)
                .def_readwrite("c", &Vertex3::c);

            py::class_<Triangle2>(m, "Triangle2")
                .def(py::init<>())
                .def(py::init<const Vertex2&, const Vertex2&, const Vertex2&>())
                .def_readwrite("v", &Triangle2::v);

            py::class_<Triangle3>(m, "Triangle3")
                .def(py::init<>())
                .def(py::init<const Vertex3&, const Vertex3&, const Vertex3&>())
                .def_readwrite("v", &Triangle3::v);

            py::class_<TriMesh2F>(m, "TriMesh2F")
                .def(py::init<>())
                .def_readwrite("v", &TriMesh2F::v)
                .def_readwrite("c", &TriMesh2F::c)
                .def_readwrite("t", &TriMesh2F::t)
                .def_readwrite("triangles", &TriMesh2F::triangles)
                .def("getByteCount", &TriMesh2F::getByteCount);

            py::class_<TriMesh3F>(m, "TriMesh3F")
                .def(py::init<>())
                .def_readwrite("v", &TriMesh3F::v)
                .def_readwrite("c", &TriMesh3F::c)
                .def_readwrite("t", &TriMesh3F::t)
                .def_readwrite("n", &TriMesh3F::n)
                .def_readwrite("triangles", &TriMesh3F::triangles)
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
