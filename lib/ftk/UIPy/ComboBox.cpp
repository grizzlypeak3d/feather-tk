// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ComboBox.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void comboBox(py::module_& m)
        {
            py::class_<ComboBoxItem>(m, "ComboBoxItem")
                .def(py::init<>())
                .def(
                    py::init<const std::string&, const std::string&>(),
                    py::arg("text"),
                    py::arg("icon") = std::string())
                .def_readwrite("text", &ComboBoxItem::text)
                .def_readwrite("icon", &ComboBoxItem::icon)
                .def("__eq__", &ComboBoxItem::operator==)
                .def("__ne__", &ComboBoxItem::operator!=);

            py::class_<ComboBox, IWidget, std::shared_ptr<ComboBox> >(m, "ComboBox")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ComboBox::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::vector<ComboBoxItem>&,
                        const std::shared_ptr<IWidget>&>(&ComboBox::create)),
                    py::arg("context"),
                    py::arg("items"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::vector<std::string>&,
                        const std::shared_ptr<IWidget>&>(&ComboBox::create)),
                    py::arg("context"),
                    py::arg("items"),
                    py::arg("parent") = nullptr)
                .def("getItems", &ComboBox::getItems)
                .def(
                    "setItems",
                    py::overload_cast<const std::vector<ComboBoxItem>&>(&ComboBox::setItems))
                .def(
                    "setItems",
                    py::overload_cast<const std::vector<std::string>&>(&ComboBox::setItems))
                .def_property("currentIndex",
                    &ComboBox::getCurrentIndex,
                    &ComboBox::setCurrentIndex)
                .def("setIndexCallback", &ComboBox::setIndexCallback)
                .def("setItemCallback", &ComboBox::setItemCallback)
                .def_property("font", &ComboBox::getFont, &ComboBox::setFont);
        }
    }
}
