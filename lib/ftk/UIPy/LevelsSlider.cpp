// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/LevelsSlider.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void levelsSlider(py::module_& m)
        {
            py::class_<LevelsModel, std::shared_ptr<LevelsModel> >(m, "LevelsModel")
                .def(py::init(&LevelsModel::create))
                .def_property("value", &LevelsModel::getValue, &LevelsModel::setValue, py::return_value_policy::copy)
                .def("observeValue", &LevelsModel::observeValue)
                .def_property("range", &LevelsModel::getRange, &LevelsModel::setRange, py::return_value_policy::copy)
                .def("observeRange", &LevelsModel::observeRange)
                .def_property("step", &LevelsModel::getStep, &LevelsModel::setStep)
                .def_property("largeStep", &LevelsModel::getLargeStep, &LevelsModel::setLargeStep)
                .def_property_readonly("hasDefault", &LevelsModel::hasDefault)
                .def("observeHasDefault", &LevelsModel::observeHasDefault)
                .def_property(
                    "defaultValue",
                    &LevelsModel::getDefault,
                    py::overload_cast<const RangeF&>(&LevelsModel::setDefault),
                    py::return_value_policy::copy)
                .def("observeDefault", &LevelsModel::observeDefault)
                .def("setDefault", py::overload_cast<>(&LevelsModel::setDefault))
                .def("clearDefault", &LevelsModel::clearDefault);

            py::class_<LevelsSlider, IMouseWidget, std::shared_ptr<LevelsSlider> >(m, "LevelsSlider")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&LevelsSlider::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<LevelsModel>&,
                        const std::shared_ptr<IWidget>&>(&LevelsSlider::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &LevelsSlider::getValue, &LevelsSlider::setValue, py::return_value_policy::copy)
                .def("setCallback", &LevelsSlider::setCallback)
                .def("setPressedCallback", &LevelsSlider::setPressedCallback)
                .def_property("range", &LevelsSlider::getRange, py::overload_cast<const RangeF&>(&LevelsSlider::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<float, float>(&LevelsSlider::setRange))
                .def("getModel", &LevelsSlider::getModel);

            py::class_<LevelsEditSlider, IContainer, std::shared_ptr<LevelsEditSlider> >(m, "LevelsEditSlider")
                .def(
                    py::init(&LevelsEditSlider::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("value", &LevelsEditSlider::getValue, &LevelsEditSlider::setValue, py::return_value_policy::copy)
                .def("setCallback", &LevelsEditSlider::setCallback)
                .def("setPressedCallback", &LevelsEditSlider::setPressedCallback)
                .def_property("range", &LevelsEditSlider::getRange, py::overload_cast<const RangeF&>(&LevelsEditSlider::setRange), py::return_value_policy::copy)
                .def("setRange", py::overload_cast<float, float>(&LevelsEditSlider::setRange))
                .def("getModel", &LevelsEditSlider::getModel)
                .def_property("precision", &LevelsEditSlider::getPrecision, &LevelsEditSlider::setPrecision)
                .def_property("font", &LevelsEditSlider::getFont, &LevelsEditSlider::setFont);
        }
    }
}
