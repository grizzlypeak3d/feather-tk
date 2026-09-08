// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Divider.h>

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
        void divider(nb::module_& m)
        {
            nb::class_<Divider, IWidget>(m, "Divider")
                .def(
                    nb::new_(&Divider::create),
                    nb::arg("context"),
                    nb::arg("orientation"),
                    nb::arg("parent") = nullptr);
        }
    }
}
