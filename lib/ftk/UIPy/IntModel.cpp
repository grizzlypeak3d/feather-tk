// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntModel.h>

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
        void intModel(nb::module_& m)
        {
            nb::class_<IntModel>(m, "IntModel")
                .def(nb::new_(&IntModel::create))
                .def_prop_rw("value", &IntModel::getValue, &IntModel::setValue)
                .def("setValueSoft", &IntModel::setValueSoft)
                .def("observeValue", &IntModel::observeValue)
                .def_prop_rw("range", &IntModel::getRange, &IntModel::setRange, nb::rv_policy::copy)
                .def("observeRange", &IntModel::observeRange)
                .def_prop_rw("rangeSoft", &IntModel::isRangeSoft, &IntModel::setRangeSoft)
                .def_prop_rw("step", &IntModel::getStep, &IntModel::setStep)
                .def_prop_rw("largeStep", &IntModel::getLargeStep, &IntModel::setLargeStep)
                .def("stepInc", nb::overload_cast<>(&IntModel::step))
                .def("stepDec", &IntModel::stepDec)
                .def("largeStepInc", nb::overload_cast<>(&IntModel::largeStep))
                .def("largeStepDec", &IntModel::largeStepDec)
                .def_prop_ro("hasDefault", &IntModel::hasDefault)
                .def("observeHasDefault", &IntModel::observeHasDefault)
                .def_prop_rw("defaultValue",
                    &IntModel::getDefault,
                    nb::overload_cast<int>(&IntModel::setDefault))
                .def("observeDefault", &IntModel::observeDefault)
                .def("setDefault", nb::overload_cast<>(&IntModel::setDefault))
                .def("clearDefault", &IntModel::clearDefault);
        }
    }
}
