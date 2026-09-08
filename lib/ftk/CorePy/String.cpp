// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/String.h>

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
        void string(nb::module_& m)
        {
            nb::enum_<ElideMode>(m, "ElideMode")
                .value("Right", ElideMode::Right)
                .value("Left", ElideMode::Left)
                .value("Middle", ElideMode::Middle);

            m.def(
                "elide",
                &elide,
                nb::arg("value"),
                nb::arg("max") = 32,
                nb::arg("mode") = ElideMode::Right);

            m.def(
                "getLoremIpsum",
                nb::overload_cast<>(&getLoremIpsum));
            m.def(
                "getLoremIpsum",
                nb::overload_cast<size_t>(&getLoremIpsum),
                nb::arg("lineCount"));
        }
    }
}
