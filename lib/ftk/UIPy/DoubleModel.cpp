// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void doubleModel(py::module_& m)
        {
            py::class_<DoubleModel, std::shared_ptr<DoubleModel> >(m, "DoubleModel")
                .def(py::init(&DoubleModel::create))
                .def_property("value", &DoubleModel::getValue, &DoubleModel::setValue)
                .def("setValueSoft", &DoubleModel::setValueSoft)
                .def("observeValue", &DoubleModel::observeValue)
                .def_property("range", &DoubleModel::getRange, &DoubleModel::setRange, py::return_value_policy::copy)
                .def("observeRange", &DoubleModel::observeRange)
                .def_property("rangeSoft", &DoubleModel::isRangeSoft, &DoubleModel::setRangeSoft)
                .def_property("step", &DoubleModel::getStep, &DoubleModel::setStep)
                .def_property("largeStep", &DoubleModel::getLargeStep, &DoubleModel::setLargeStep)
                .def("stepInc", py::overload_cast<>(&DoubleModel::step))
                .def("stepDec", &DoubleModel::stepDec)
                .def("largeStepInc", py::overload_cast<>(&DoubleModel::largeStep))
                .def("largeStepDec", &DoubleModel::largeStepDec)
                .def_property_readonly("hasDefault", &DoubleModel::hasDefault)
                .def("observeHasDefault", &DoubleModel::observeHasDefault)
                .def_property("defaultValue",
                    &DoubleModel::getDefault,
                    py::overload_cast<double>(&DoubleModel::setDefault))
                .def("observeDefault", &DoubleModel::observeDefault)
                .def("setDefault", py::overload_cast<>(&DoubleModel::setDefault))
                .def("clearDefault", &DoubleModel::clearDefault);
        }
    }
}
