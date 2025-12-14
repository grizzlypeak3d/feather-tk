// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntEdit.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void intEdit(py::module_& m)
        {
            py::class_<IntEdit, IWidget, std::shared_ptr<IntEdit> >(m, "IntEdit")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&IntEdit::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IntModel>&,
                        const std::shared_ptr<IWidget>&>(&IntEdit::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def_property("value", &IntEdit::getValue, &IntEdit::setValue)
                .def("setCallback", &IntEdit::setCallback)
                .def_property("range", &IntEdit::getRange, py::overload_cast<const RangeI&>(&IntEdit::setRange))
                .def("setRange", py::overload_cast<int, int>(&IntEdit::setRange))
                .def_property("step", &IntEdit::getStep, &IntEdit::setStep)
                .def_property("largeStep", &IntEdit::getLargeStep, &IntEdit::setLargeStep)
                .def_property("defaultValue", &IntEdit::getDefaultValue, &IntEdit::setDefaultValue)
                .def("getModel", &IntEdit::getModel)
                .def_property("fontRole", &IntEdit::getFontRole, &IntEdit::setFontRole);
        }
    }
}
