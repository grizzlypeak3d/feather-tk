// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Color.h>

#include <ftk/Core/Color.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void color(py::module_& m)
        {
            py::class_<Color1F>(m, "Color1F")
                .def(py::init<>())
                .def(py::init<float>())
                .def("__setitem__",
                    [](Color1F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](Color1F& self, unsigned index) { return self.e.at(index); })
                .def_property("l", [](Color1F& self) { return self.l; }, [](Color1F& self, float v) { self.l = v; })
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const Color1F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<Color2F>(m, "Color2F")
                .def(py::init<>())
                .def(py::init<float>())
                .def(py::init<float, float>())
                .def("__setitem__",
                    [](Color2F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](Color2F& self, unsigned index) { return self.e.at(index); })
                .def_property("l", [](Color2F& self) { return self.l; }, [](Color2F& self, float v) { self.l = v; })
                .def_property("a", [](Color2F& self) { return self.a; }, [](Color2F& self, float v) { self.a = v; })
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const Color2F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<Color3F>(m, "Color3F")
                .def(py::init<>())
                .def(py::init<float, float, float>())
                .def("__setitem__",
                    [](Color3F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](Color3F& self, unsigned index) { return self.e.at(index); })
                .def_property("r", [](Color3F& self) { return self.r; }, [](Color3F& self, float v) { self.r = v; })
                .def_property("g", [](Color3F& self) { return self.g; }, [](Color3F& self, float v) { self.g = v; })
                .def_property("b", [](Color3F& self) { return self.b; }, [](Color3F& self, float v) { self.b = v; })
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const Color3F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            py::class_<Color4F>(m, "Color4F")
                .def(py::init<>())
                .def(py::init<float, float, float>())
                .def(py::init<float, float, float, float>())
                .def("__setitem__",
                    [](Color4F& self, unsigned index, float v) { self.e.at(index) = v; })
                .def("__getitem__",
                    [](Color4F& self, unsigned index) { return self.e.at(index); })
                .def_property("r", [](Color4F& self) { return self.r; }, [](Color4F& self, float v) { self.r = v; })
                .def_property("g", [](Color4F& self) { return self.g; }, [](Color4F& self, float v) { self.g = v; })
                .def_property("b", [](Color4F& self) { return self.b; }, [](Color4F& self, float v) { self.b = v; })
                .def_property("a", [](Color4F& self) { return self.a; }, [](Color4F& self, float v) { self.a = v; })
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def("__repr__", [](const Color4F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });
        }
    }
}
