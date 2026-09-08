// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ComboBox.h>

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
        void comboBox(nb::module_& m)
        {
            nb::class_<ComboBoxItem>(m, "ComboBoxItem")
                .def(nb::init<>())
                .def(
                    nb::init<const std::string&, const std::string&>(),
                    nb::arg("text"),
                    nb::arg("icon") = std::string())
                .def_rw("text", &ComboBoxItem::text)
                .def_rw("icon", &ComboBoxItem::icon)
                .def("__eq__", &ComboBoxItem::operator==)
                .def("__ne__", &ComboBoxItem::operator!=);

            nb::class_<ComboBox, IWidget>(m, "ComboBox")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ComboBox::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::vector<ComboBoxItem>&,
                        const std::shared_ptr<IWidget>&>(&ComboBox::create)),
                    nb::arg("context"),
                    nb::arg("items"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::vector<std::string>&,
                        const std::shared_ptr<IWidget>&>(&ComboBox::create)),
                    nb::arg("context"),
                    nb::arg("items"),
                    nb::arg("parent") = nullptr)
                .def("getItems", &ComboBox::getItems)
                .def(
                    "setItems",
                    nb::overload_cast<const std::vector<ComboBoxItem>&>(&ComboBox::setItems))
                .def(
                    "setItems",
                    nb::overload_cast<const std::vector<std::string>&>(&ComboBox::setItems))
                .def_prop_rw("currentIndex",
                    &ComboBox::getCurrentIndex,
                    &ComboBox::setCurrentIndex)
                .def(
                    "setElide",
                    &ComboBox::setElide,
                    nb::arg("max"),
                    nb::arg("mode") = ElideMode::Right)
                .def("setIndexCallback", &ComboBox::setIndexCallback)
                .def("setItemCallback", &ComboBox::setItemCallback)
                .def_prop_rw("font", &ComboBox::getFont, &ComboBox::setFont);
        }
    }
}
