// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatEdit.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void floatEdit(py::module_& m)
        {
            py::class_<FloatEdit, IWidget, std::shared_ptr<FloatEdit> >(m, "FloatEdit")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&FloatEdit::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<FloatModel>&,
                        const std::shared_ptr<IWidget>&>(&FloatEdit::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &FloatEdit::getValue, &FloatEdit::setValue)
                .def("setCallback", &FloatEdit::setCallback)
                .def_property("range", &FloatEdit::getRange, py::overload_cast<const RangeF&>(&FloatEdit::setRange))
                .def("setRange", py::overload_cast<float, float>(&FloatEdit::setRange))
                .def_property("step", &FloatEdit::getStep, &FloatEdit::setStep)
                .def_property("largeStep", &FloatEdit::getLargeStep, &FloatEdit::setLargeStep)
                .def_property("defaultValue", &FloatEdit::getDefaultValue, &FloatEdit::setDefaultValue)
                .def("getModel", &FloatEdit::getModel)
                .def_property("precision", &FloatEdit::getPrecision, &FloatEdit::setPrecision)
                .def_property("fontRole", &FloatEdit::getFontRole, &FloatEdit::setFontRole);
        }
    }
}
