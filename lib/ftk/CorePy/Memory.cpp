// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Memory.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void memory(nb::module_& m)
        {
            m.attr("kilobyte") = kilobyte;
            m.attr("megabyte") = megabyte;
            m.attr("gigabyte") = gigabyte;
            m.attr("terabyte") = terabyte;

            nb::enum_<Endian>(m, "Endian")
                .value("MSB", Endian::MSB)
                .value("LSB", Endian::LSB);
            FTK_ENUM_BIND(m, Endian);

            m.def("getEndian", &getEndian);
            m.def("oppositeEndian", &opposite);
        }
    }
}
