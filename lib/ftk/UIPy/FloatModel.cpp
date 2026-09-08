// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatModel.h>

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
        void floatModel(nb::module_& m)
        {
            nb::class_<FloatModel>(m, "FloatModel")
                .def(nb::new_(&FloatModel::create))
                .def_prop_rw("value", &FloatModel::getValue, &FloatModel::setValue)
                .def("setValueSoft", &FloatModel::setValueSoft)
                .def("observeValue", &FloatModel::observeValue)
                .def_prop_rw("range", &FloatModel::getRange, &FloatModel::setRange, nb::rv_policy::copy)
                .def("observeRange", &FloatModel::observeRange)
                .def_prop_rw("rangeSoft", &FloatModel::isRangeSoft, &FloatModel::setRangeSoft)
                .def_prop_rw("step", &FloatModel::getStep, &FloatModel::setStep)
                .def_prop_rw("largeStep", &FloatModel::getLargeStep, &FloatModel::setLargeStep)
                .def("stepInc", nb::overload_cast<>(&FloatModel::step))
                .def("stepDec", &FloatModel::stepDec)
                .def("largeStepInc", nb::overload_cast<>(&FloatModel::largeStep))
                .def("largeStepDec", &FloatModel::largeStepDec)
                .def_prop_ro("hasDefault", &FloatModel::hasDefault)
                .def("observeHasDefault", &FloatModel::observeHasDefault)
                .def_prop_rw("defaultValue",
                    &FloatModel::getDefault,
                    nb::overload_cast<float>(&FloatModel::setDefault))
                .def("observeDefault", &FloatModel::observeDefault)
                .def("setDefault", nb::overload_cast<>(&FloatModel::setDefault))
                .def("clearDefault", &FloatModel::clearDefault);
        }
    }
}
