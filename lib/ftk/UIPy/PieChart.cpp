// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/PieChart.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void pieChart(py::module_& m)
        {
            py::class_<PieChartData>(m, "PieChartData")
                .def(py::init<>())
                .def(
                    py::init<float, const Color4F&>(),
                    py::arg("percentage"),
                    py::arg("color"))
                .def_readwrite("percentage", &PieChartData::percentage)
                .def_readwrite("color", &PieChartData::color)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<PieChart, IWidget, std::shared_ptr<PieChart> >(m, "PieChart")
                .def(
                    py::init(&PieChart::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("data", &PieChart::getData, &PieChart::setData)
                .def_property("sizeMult", &PieChart::getSizeMult, &PieChart::setSizeMult);
        }
    }
}
