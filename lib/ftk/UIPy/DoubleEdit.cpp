// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DoubleEdit.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void doubleEdit(py::module_& m)
        {
            py::class_<DoubleEdit, IWidget, std::shared_ptr<DoubleEdit> >(m, "DoubleEdit")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEdit::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<DoubleModel>&,
                        const std::shared_ptr<IWidget>&>(&DoubleEdit::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &DoubleEdit::getValue, &DoubleEdit::setValue)
                .def("setCallback", &DoubleEdit::setCallback)
                .def_property("range", &DoubleEdit::getRange, py::overload_cast<const RangeD&>(&DoubleEdit::setRange))
                .def("setRange", py::overload_cast<double, double>(&DoubleEdit::setRange))
                .def_property("step", &DoubleEdit::getStep, &DoubleEdit::setStep)
                .def_property("largeStep", &DoubleEdit::getLargeStep, &DoubleEdit::setLargeStep)
                .def_property("defaultValue", &DoubleEdit::getDefaultValue, &DoubleEdit::setDefaultValue)
                .def("getModel", &DoubleEdit::getModel)
                .def_property("precision", &DoubleEdit::getPrecision, &DoubleEdit::setPrecision)
                .def_property("fontRole", &DoubleEdit::getFontRole, &DoubleEdit::setFontRole)
                .def_property("borderRole", &DoubleEdit::getBorderRole, &DoubleEdit::setBorderRole);
        }
    }
}
