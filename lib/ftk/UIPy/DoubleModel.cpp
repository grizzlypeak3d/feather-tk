// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleModel.h>

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
        void doubleModel(nb::module_& m)
        {
            nb::class_<DoubleModel>(m, "DoubleModel")
                .def(nb::new_(&DoubleModel::create))
                .def_prop_rw("value", &DoubleModel::getValue, &DoubleModel::setValue)
                .def("setValueSoft", &DoubleModel::setValueSoft)
                .def("observeValue", &DoubleModel::observeValue)
                .def_prop_rw("range", &DoubleModel::getRange, &DoubleModel::setRange, nb::rv_policy::copy)
                .def("observeRange", &DoubleModel::observeRange)
                .def_prop_rw("rangeSoft", &DoubleModel::isRangeSoft, &DoubleModel::setRangeSoft)
                .def_prop_rw("step", &DoubleModel::getStep, &DoubleModel::setStep)
                .def_prop_rw("largeStep", &DoubleModel::getLargeStep, &DoubleModel::setLargeStep)
                .def("stepInc", nb::overload_cast<>(&DoubleModel::step))
                .def("stepDec", &DoubleModel::stepDec)
                .def("largeStepInc", nb::overload_cast<>(&DoubleModel::largeStep))
                .def("largeStepDec", &DoubleModel::largeStepDec)
                .def_prop_ro("hasDefault", &DoubleModel::hasDefault)
                .def("observeHasDefault", &DoubleModel::observeHasDefault)
                .def_prop_rw("defaultValue",
                    &DoubleModel::getDefault,
                    nb::overload_cast<double>(&DoubleModel::setDefault))
                .def("observeDefault", &DoubleModel::observeDefault)
                .def("setDefault", nb::overload_cast<>(&DoubleModel::setDefault))
                .def("clearDefault", &DoubleModel::clearDefault);
        }
    }
}
