// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/GraphWidget.h>

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
        void graphWidget(nb::module_& m)
        {
            nb::class_<GraphLabel>(m, "GraphLabel")
                .def(nb::init<>())
                .def_rw("colorRole", &GraphLabel::colorRole)
                .def_rw("text", &GraphLabel::text)
                .def_rw("format", &GraphLabel::format);

            nb::class_<GraphWidget, IContainer>(m, "GraphWidget")
                .def(
                    nb::new_(&GraphWidget::create),
                    nb::arg("context"),
                    nb::arg("title"),
                    nb::arg("labels"),
                    nb::arg("parent") = nullptr)
                .def("setSamples", &GraphWidget::setSamples)
                .def("addSample", &GraphWidget::addSample);
        }
    }
}
