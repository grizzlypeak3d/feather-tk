// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void floatModel(py::module_& m)
        {
            py::class_<FloatModel, std::shared_ptr<FloatModel> >(m, "FloatModel")
                .def(py::init(&FloatModel::create))
                .def_property("value", &FloatModel::getValue, &FloatModel::setValue)
                .def("observeValue", &FloatModel::observeValue)
                .def_property("range", &FloatModel::getRange, &FloatModel::setRange, py::return_value_policy::copy)
                .def("observeRange", &FloatModel::observeRange)
                .def_property("rangeSoft", &FloatModel::isRangeSoft, &FloatModel::setRangeSoft)
                .def_property("step", &FloatModel::getStep, &FloatModel::setStep)
                .def_property("largeStep", &FloatModel::getLargeStep, &FloatModel::setLargeStep)
                .def("stepInc", py::overload_cast<>(&FloatModel::step))
                .def("stepDec", &FloatModel::stepDec)
                .def("largeStepInc", py::overload_cast<>(&FloatModel::largeStep))
                .def("largeStepDec", &FloatModel::largeStepDec)
                .def_property_readonly("hasDefault", &FloatModel::hasDefault)
                .def("observeHasDefault", &FloatModel::observeHasDefault)
                .def_property("defaultValue",
                    &FloatModel::getDefault,
                    py::overload_cast<float>(&FloatModel::setDefault))
                .def("observeDefault", &FloatModel::observeDefault)
                .def("setDefault", py::overload_cast<>(&FloatModel::setDefault))
                .def("clearDefault", &FloatModel::clearDefault);
        }
    }
}
