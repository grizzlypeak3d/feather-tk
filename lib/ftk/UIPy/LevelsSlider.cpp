// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/LevelsSlider.h>

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
        void levelsSlider(nb::module_& m)
        {
            nb::class_<LevelsModel>(m, "LevelsModel")
                .def(nb::new_(&LevelsModel::create))
                .def_prop_rw("value", &LevelsModel::getValue, &LevelsModel::setValue, nb::rv_policy::copy)
                .def("observeValue", &LevelsModel::observeValue)
                .def_prop_rw("range", &LevelsModel::getRange, &LevelsModel::setRange, nb::rv_policy::copy)
                .def("observeRange", &LevelsModel::observeRange)
                .def_prop_rw("step", &LevelsModel::getStep, &LevelsModel::setStep)
                .def_prop_rw("largeStep", &LevelsModel::getLargeStep, &LevelsModel::setLargeStep)
                .def_prop_ro("hasDefault", &LevelsModel::hasDefault)
                .def("observeHasDefault", &LevelsModel::observeHasDefault)
                .def_prop_rw(
                    "defaultValue",
                    &LevelsModel::getDefault,
                    nb::overload_cast<const RangeF&>(&LevelsModel::setDefault),
                    nb::rv_policy::copy)
                .def("observeDefault", &LevelsModel::observeDefault)
                .def("setDefault", nb::overload_cast<>(&LevelsModel::setDefault))
                .def("clearDefault", &LevelsModel::clearDefault);

            nb::class_<LevelsSlider, IMouseWidget>(m, "LevelsSlider")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&LevelsSlider::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<LevelsModel>&,
                        const std::shared_ptr<IWidget>&>(&LevelsSlider::create)),
                    nb::arg("context"),
                    nb::arg("model"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &LevelsSlider::getValue, &LevelsSlider::setValue, nb::rv_policy::copy)
                .def("setCallback", &LevelsSlider::setCallback)
                .def("setPressedCallback", &LevelsSlider::setPressedCallback)
                .def_prop_rw("range", &LevelsSlider::getRange, nb::overload_cast<const RangeF&>(&LevelsSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&LevelsSlider::setRange))
                .def("getModel", &LevelsSlider::getModel);

            nb::class_<LevelsEditSlider, IContainer>(m, "LevelsEditSlider")
                .def(
                    nb::new_(&LevelsEditSlider::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("value", &LevelsEditSlider::getValue, &LevelsEditSlider::setValue, nb::rv_policy::copy)
                .def("setCallback", &LevelsEditSlider::setCallback)
                .def("setPressedCallback", &LevelsEditSlider::setPressedCallback)
                .def_prop_rw("range", &LevelsEditSlider::getRange, nb::overload_cast<const RangeF&>(&LevelsEditSlider::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<float, float>(&LevelsEditSlider::setRange))
                .def("getModel", &LevelsEditSlider::getModel)
                .def_prop_rw("precision", &LevelsEditSlider::getPrecision, &LevelsEditSlider::setPrecision)
                .def_prop_rw("font", &LevelsEditSlider::getFont, &LevelsEditSlider::setFont);
        }
    }
}
