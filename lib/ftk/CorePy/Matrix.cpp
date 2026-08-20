// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Matrix.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <sstream>
#include <stdexcept>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void matrix(py::module_& m)
        {
            py::class_<M33F>(m, "M33F")
                .def(py::init<>())
                .def(py::init<
                    float, float, float,
                    float, float, float,
                    float, float, float>())
                .def("get", &M33F::get)
                .def("set", &M33F::set)
                .def("__setitem__",
                    [](M33F& self, unsigned index, float v) { if (index >= 9) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](M33F& self, unsigned index) { if (index >= 9) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def(py::self * py::self)
                .def(py::self * V2F())
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const M33F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<M44F>(m, "M44F")
                .def(py::init<>())
                .def(py::init<
                    float, float, float, float,
                    float, float, float, float,
                    float, float, float, float,
                    float, float, float, float>())
                .def("get", &M44F::get)
                .def("set", &M44F::set)
                .def("__setitem__",
                    [](M44F& self, unsigned index, float v) { if (index >= 16) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](M44F& self, unsigned index) { if (index >= 16) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def(py::self * py::self)
                .def(py::self * V3F())
                .def(py::self * V4F())
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const M44F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            m.def("translate", [](const V2F& v) { return translate(v); });
            m.def("translate", [](const V3F& v) { return translate(v); });

            m.def("rotateX", [](float v) { return rotateX(v); });
            m.def("rotateY", [](float v) { return rotateY(v); });
            m.def("rotateZ", [](float v) { return rotateZ(v); });

            m.def("rotate", [](float angle, const V3F& axis) { return rotate(angle, axis); });

            m.def("rotateXYZ", [](const V3F& v) { return rotateXYZ(v); });

            m.def("getRotateXYZ", [](const M44F& v, const V3F& nearAngles) { return getRotateXYZ(v, nearAngles); });

            m.def("scale", [](const V3F& v) { return scale(v); });

            m.def("ortho", [](float left, float right, float bottom, float top, float nearClip, float farClip) { return ortho(left, right, bottom, top, nearClip, farClip); });

            m.def("perspective", [](float fov, float aspect, float nearClip, float farClip) { return perspective(fov, aspect, nearClip, farClip); });

            m.def("invert", [](const M44F& a, M44F& b) { return invert(a, b); });
        }
    }
}
