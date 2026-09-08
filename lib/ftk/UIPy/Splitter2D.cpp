// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Splitter2D.h>

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
        void splitter2D(nb::module_& m)
        {
            nb::class_<Splitter2D, IWidget>(m, "Splitter2D")
                .def(
                    nb::new_(&Splitter2D::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def("setWidgets", &Splitter2D::setWidgets, nb::arg("widgets"))
                .def_prop_rw("split", &Splitter2D::getSplit, &Splitter2D::setSplit, nb::rv_policy::copy)
                .def("setSplitCallback", &Splitter2D::setSplitCallback)
                .def_prop_rw("border", &Splitter2D::hasBorder, &Splitter2D::setBorder);
        }
    }
}
