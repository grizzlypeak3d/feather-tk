// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/ToolButton.h>

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
        void toolButton(nb::module_& m)
        {
            nb::class_<ToolButton, IButton>(m, "ToolButton")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&ToolButton::create)),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&ToolButton::create)),
                    nb::arg("context"),
                    nb::arg("text"),
                    nb::arg("parent") = nullptr)
                .def(
                    nb::new_(nb::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<Action>&,
                        const std::shared_ptr<IWidget>&>(&ToolButton::create)),
                    nb::arg("context"),
                    nb::arg("action"),
                    nb::arg("parent") = nullptr)
            .def_prop_rw("popupIcon", &ToolButton::hasPopupIcon, nb::overload_cast<bool>(&ToolButton::setPopupIcon))
            .def_prop_rw("secondaryText", &ToolButton::getSecondaryText, &ToolButton::setSecondaryText);
        }
    }
}
