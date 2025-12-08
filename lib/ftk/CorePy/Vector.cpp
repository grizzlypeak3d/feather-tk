// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Vector.h>

#include <ftk/Core/Vector.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void vector(py::module_& m)
        {
            py::class_<V2I>(m, "V2I")
                .def(py::init<>())
                .def(py::init<int, int>())
                .def("__setitem__",
                    [](V2I& self, unsigned index, int v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](V2I& self, unsigned index) { return self.e.at(index); })
                .def_property("x", [](V2I& self) { return self.x; }, [](V2I& self, int v) { self.x = v; })
                .def_property("y", [](V2I& self) { return self.y; }, [](V2I& self, int v) { self.y = v; })
                .def(py::self + py::self)
                .def(py::self + int())
                .def(py::self - py::self)
                .def(py::self - int())
                .def(py::self * int())
                .def(py::self / int())
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const V2I& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<V2F>(m, "V2F")
                .def(py::init<>())
                .def(py::init<float, float>())
                .def("__setitem__",
                    [](V2F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](V2F& self, unsigned index) { return self.e.at(index); })
                .def_property("x", [](V2F& self) { return self.x; }, [](V2F& self, float v) { self.x = v; })
                .def_property("y", [](V2F& self) { return self.y; }, [](V2F& self, float v) { self.y = v; })
                .def(py::self + py::self)
                .def(py::self + float())
                .def(py::self - py::self)
                .def(py::self - float())
                .def(py::self * float())
                .def(py::self / float())
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const V2F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<V3F>(m, "V3F")
                .def(py::init<>())
                .def(py::init<float, float, float>())
                .def("__setitem__",
                    [](V3F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](V3F& self, unsigned index) { return self.e.at(index); })
                .def_property("x", [](V3F& self) { return self.x; }, [](V3F& self, float v) { self.x = v; })
                .def_property("y", [](V3F& self) { return self.y; }, [](V3F& self, float v) { self.y = v; })
                .def_property("z", [](V3F& self) { return self.z; }, [](V3F& self, float v) { self.z = v; })
                .def(py::self + py::self)
                .def(py::self + float())
                .def(py::self - py::self)
                .def(py::self - float())
                .def(py::self * float())
                .def(py::self / float())
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const V3F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<V4F>(m, "V4F")
                .def(py::init<>())
                .def(py::init<float, float, float, float>())
                .def("__setitem__",
                    [](V4F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](V4F& self, unsigned index) { return self.e.at(index); })
                .def_property("x", [](V4F& self) { return self.x; }, [](V4F& self, float v) { self.x = v; })
                .def_property("y", [](V4F& self) { return self.y; }, [](V4F& self, float v) { self.y = v; })
                .def_property("z", [](V4F& self) { return self.z; }, [](V4F& self, float v) { self.z = v; })
                .def_property("w", [](V4F& self) { return self.w; }, [](V4F& self, float v) { self.w = v; })
                .def(py::self + py::self)
                .def(py::self + float())
                .def(py::self - py::self)
                .def(py::self - float())
                .def(py::self * float())
                .def(py::self / float())
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const V4F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            m.def("length", [](const V2I& v) { return length(v); });
            m.def("length", [](const V2F& v) { return length(v); });
            m.def("length", [](const V3F& v) { return length(v); });
            m.def("length", [](const V4F& v) { return length(v); });

            m.def("normalize", [](const V2F& v) { return normalize(v); });
            m.def("normalize", [](const V3F& v) { return normalize(v); });
            m.def("normalize", [](const V4F& v) { return normalize(v); });

            m.def("dot", [](const V2F& a, const V2F& b) { return dot(a, b); });
            m.def("dot", [](const V3F& a, const V3F& b) { return dot(a, b); });
            m.def("dot", [](const V4F& a, const V4F& b) { return dot(a, b); });

            m.def("cross", [](const V3F& a, const V3F& b) { return cross(a, b); });

            m.def("perpCW", [](const V2I& v) { return perpCW(v); });
            m.def("perpCW", [](const V2F& v) { return perpCW(v); });
            m.def("perpCCW", [](const V2I& v) { return perpCCW(v); });
            m.def("perpCCW", [](const V2F& v) { return perpCCW(v); });

            m.def("convert", [](const V2I& v) { return convert(v); });

            m.def("round", [](const V2F& v) { return round(v); });
            m.def("floor", [](const V2F& v) { return floor(v); });
            m.def("ceil", [](const V2F& v) { return ceil(v); });
            
            m.def("to_string", [](const V2I& v) { return to_string(v); });
            m.def("to_string", [](const V2F& v) { return to_string(v); });
            m.def("to_string", [](const V3F& v) { return to_string(v); });
            m.def("to_string", [](const V4F& v) { return to_string(v); });
            
            m.def("from_string", [](const std::string& s, V2I& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, V2F& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, V3F& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, V4F& v) { from_string(s, v); });
        }
    }
}
