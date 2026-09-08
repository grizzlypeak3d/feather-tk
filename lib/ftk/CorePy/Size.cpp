// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Size.h>

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
        void size(nb::module_& m)
        {
            nb::class_<Size2I>(m, "Size2I")
                .def(nb::init<>())
                .def(nb::init<int, int>())
                .def("__setitem__",
                    [](Size2I& self, unsigned index, int v) { if (index >= 2) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Size2I& self, unsigned index) { if (index >= 2) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("w", [](Size2I& self) { return self.w; }, [](Size2I& self, int v) { self.w = v; })
                .def_prop_rw("h", [](Size2I& self) { return self.h; }, [](Size2I& self, int v) { self.h = v; })
                .def_prop_ro("valid", &Size2I::isValid)
                .def(nb::self + int())
                .def(nb::self - int())
                .def(nb::self * int())
                .def(nb::self / int())
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Size2I& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Size2F>(m, "Size2F")
                .def(nb::init<>())
                .def(nb::init<float, float>())
                .def("__setitem__",
                    [](Size2F& self, unsigned index, float v) { if (index >= 2) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Size2F& self, unsigned index) { if (index >= 2) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("w", [](Size2F& self) { return self.w; }, [](Size2F& self, int v) { self.w = v; })
                .def_prop_rw("h", [](Size2F& self) { return self.h; }, [](Size2F& self, int v) { self.h = v; })
                .def_prop_ro("valid", &Size2F::isValid)
                .def(nb::self + float())
                .def(nb::self - float())
                .def(nb::self * float())
                .def(nb::self / float())
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Size2I& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            nb::class_<Size3F>(m, "Size3F")
                .def(nb::init<>())
                .def(nb::init<float, float, float>())
                .def("__setitem__",
                    [](Size3F& self, unsigned index, float v) { if (index >= 3) throw std::out_of_range("index"); self[static_cast<int>(index)] = v; })
                .def("__getitem__",
                    [](Size3F& self, unsigned index) { if (index >= 3) throw std::out_of_range("index"); return self[static_cast<int>(index)]; })
                .def_prop_rw("w", [](Size3F& self) { return self.w; }, [](Size3F& self, int v) { self.w = v; })
                .def_prop_rw("h", [](Size3F& self) { return self.h; }, [](Size3F& self, int v) { self.h = v; })
                .def_prop_rw("d", [](Size3F& self) { return self.d; }, [](Size3F& self, int v) { self.d = v; })
                .def_prop_ro("valid", &Size3F::isValid)
                .def(nb::self + float())
                .def(nb::self - float())
                .def(nb::self * float())
                .def(nb::self / float())
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def("__repr__", [](const Size2I& v)
                    {
                        std::stringstream ss;
                        ss << v;
                        return ss.str();
                    });

            m.def("aspectRatio", [](const Size2I& v) { return aspectRatio(v); });
            m.def("aspectRatio", [](const Size2F& v) { return aspectRatio(v); });

            m.def("area", [](const Size2I& v) { return area(v); });
            m.def("area", [](const Size2F& v) { return area(v); });

            m.def("volume", [](const Size3F& v) { return volume(v); });

            m.def("margin", [](const Size2I& a, int b) { return margin(a, b); });
            m.def("margin", [](const Size2F& a, float b) { return margin(a, b); });
            m.def("margin", [](const Size2I& a, int b, int c) { return margin(a, b, c); });
            m.def("margin", [](const Size2F& a, float b, float c) { return margin(a, b, c); });
            
            m.def("to_string", [](const Size2I& v) { return to_string(v); });
            m.def("to_string", [](const Size2F& v) { return to_string(v); });
            m.def("to_string", [](const Size3F& v) { return to_string(v); });
            
            m.def("from_string", [](const std::string& s, Size2I& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, Size2F& v) { from_string(s, v); });
            m.def("from_string", [](const std::string& s, Size3F& v) { from_string(s, v); });
        }
    }
}
