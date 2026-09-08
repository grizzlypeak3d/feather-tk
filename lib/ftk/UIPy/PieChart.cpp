// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/PieChart.h>

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

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void pieChart(nb::module_& m)
        {
            nb::class_<PieChartData>(m, "PieChartData")
                .def(nb::init<>())
                .def(
                    nb::init<float, const Color4F&>(),
                    nb::arg("percentage"),
                    nb::arg("color"))
                .def_rw("percentage", &PieChartData::percentage)
                .def_rw("color", &PieChartData::color)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::class_<PieChart, IWidget>(m, "PieChart")
                .def(
                    nb::new_(&PieChart::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("data", &PieChart::getData, &PieChart::setData)
                .def_prop_rw("sizeMult", &PieChart::getSizeMult, &PieChart::setSizeMult);
        }
    }
}
