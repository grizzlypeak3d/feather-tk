// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void intModel(py::module_& m)
        {
            py::class_<IntModel, std::shared_ptr<IntModel> >(m, "IntModel")
                .def(py::init(&IntModel::create))
                .def_property("value", &IntModel::getValue, &IntModel::setValue)
                .def("observeValue", &IntModel::observeValue)
                .def_property("range", &IntModel::getRange, &IntModel::setRange, py::return_value_policy::copy)
                .def("observeRange", &IntModel::observeRange)
                .def_property("rangeSoft", &IntModel::isRangeSoft, &IntModel::setRangeSoft)
                .def_property("step", &IntModel::getStep, &IntModel::setStep)
                .def_property("largeStep", &IntModel::getLargeStep, &IntModel::setLargeStep)
                .def("stepInc", py::overload_cast<>(&IntModel::step))
                .def("stepDec", &IntModel::stepDec)
                .def("largeStepInc", py::overload_cast<>(&IntModel::largeStep))
                .def("largeStepDec", &IntModel::largeStepDec)
                .def_property_readonly("hasDefault", &IntModel::hasDefault)
                .def("observeHasDefault", &IntModel::observeHasDefault)
                .def_property("defaultValue",
                    &IntModel::getDefault,
                    py::overload_cast<int>(&IntModel::setDefault))
                .def("observeDefault", &IntModel::observeDefault)
                .def("setDefault", py::overload_cast<>(&IntModel::setDefault))
                .def("clearDefault", &IntModel::clearDefault);
        }
    }
}
