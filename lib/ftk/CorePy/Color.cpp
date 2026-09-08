// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Color.h>

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
#include <stdexcept>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void color(nb::module_& m)
        {
            nb::class_<Color1F>(m, "Color1F")
                .def(nb::init<>())
                .def(nb::init<float>())
                .def("__setitem__",
                    [](Color1F& self, unsigned index, float v) { if (index >= 1) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Color1F& self, unsigned index) { if (index >= 1) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("l", [](Color1F& self) { return self.l; }, [](Color1F& self, float v) { self.l = v; })
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Color1F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Color2F>(m, "Color2F")
                .def(nb::init<>())
                .def(nb::init<float>())
                .def(nb::init<float, float>())
                .def("__setitem__",
                    [](Color2F& self, unsigned index, float v) { if (index >= 2) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Color2F& self, unsigned index) { if (index >= 2) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("l", [](Color2F& self) { return self.l; }, [](Color2F& self, float v) { self.l = v; })
                .def_prop_rw("a", [](Color2F& self) { return self.a; }, [](Color2F& self, float v) { self.a = v; })
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Color2F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Color3F>(m, "Color3F")
                .def(nb::init<>())
                .def(nb::init<float, float, float>())
                .def("__setitem__",
                    [](Color3F& self, unsigned index, float v) { if (index >= 3) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Color3F& self, unsigned index) { if (index >= 3) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("r", [](Color3F& self) { return self.r; }, [](Color3F& self, float v) { self.r = v; })
                .def_prop_rw("g", [](Color3F& self) { return self.g; }, [](Color3F& self, float v) { self.g = v; })
                .def_prop_rw("b", [](Color3F& self) { return self.b; }, [](Color3F& self, float v) { self.b = v; })
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Color3F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Color4F>(m, "Color4F")
                .def(nb::init<>())
                .def(nb::init<float, float, float>())
                .def(nb::init<float, float, float, float>())
                .def("__setitem__",
                    [](Color4F& self, unsigned index, float v) { if (index >= 4) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Color4F& self, unsigned index) { if (index >= 4) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("r", [](Color4F& self) { return self.r; }, [](Color4F& self, float v) { self.r = v; })
                .def_prop_rw("g", [](Color4F& self) { return self.g; }, [](Color4F& self, float v) { self.g = v; })
                .def_prop_rw("b", [](Color4F& self) { return self.b; }, [](Color4F& self, float v) { self.b = v; })
                .def_prop_rw("a", [](Color4F& self) { return self.a; }, [](Color4F& self, float v) { self.a = v; })
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Color4F& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });
        }
    }
}
