// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Splitter.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
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
        void splitter(nb::module_& m)
        {
            nb::class_<Splitter, IWidget>(m, "Splitter")
                .def(
                    nb::new_(&Splitter::create),
                    nb::arg("context"),
                    nb::arg("orientation"),
                    nb::arg("parent") = nullptr)
                .def("setWidgets", &Splitter::setWidgets)
                .def_prop_rw("split", &Splitter::getSplit, &Splitter::setSplit)
                .def("setSplitCallback", &Splitter::setSplitCallback)
                .def_prop_rw("border", &Splitter::hasBorder, &Splitter::setBorder);
        }
    }
}
