// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/Menu.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        namespace
        {
            //! Exposes the protected constructor and _init for Python-owned
            //! construction; adds no members, so the base storage fits.
            class PyMenu : public Menu
            {
            public:
                template<typename... Args>
                void pyInit(Args&&... args)
                {
                    _init(std::forward<Args>(args)...);
                }
            };
        }

        void menu(nb::module_& m)
        {
            nb::class_<Menu, IMenuPopup>(m, "Menu")
                .def(
                    "__init__",
                    // Python subclasses pass their own __init__ arguments
                    // through __new__, which a factory constructor cannot
                    // accept -- so construction is __init__-based (see
                    // WidgetTrampoline.h).
                    [](Menu* self,
                       const std::shared_ptr<Context>& context,
                       const std::shared_ptr<IWidget>& parent)
                    {
                        pyConstruct<PyMenu>(self,
                            [&](PyMenu& w)
                            {
                                w.pyInit(context, parent);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def("getActions", &Menu::getActions)
                .def(
                    "setCurrentCallback",
                    &Menu::setCurrentCallback,
                    nb::arg("callback"))
                .def(
                    "addAction",
                    // Self as a shared_ptr: the menu wires the action's
                    // button through shared_from_this().
                    [](const std::shared_ptr<Menu>& self,
                       const std::shared_ptr<Action>& action)
                    {
                        self->addAction(action);
                    },
                    nb::arg("action"))
                .def(
                    "setChecked",
                    &Menu::setChecked,
                    nb::arg("action"),
                    nb::arg("checked"))
                .def(
                    "setEnabled",
                    nb::overload_cast<bool>(&IWidget::setEnabled),
                    nb::arg("enabled"))
                .def(
                    "setEnabled",
                    nb::overload_cast<const std::shared_ptr<Action>&, bool>(&Menu::setEnabled),
                    nb::arg("action"),
                    nb::arg("enabled"))
                .def("getSubMenus", &Menu::getSubMenus)
                .def(
                    "addSubMenu",
                    // Self as a shared_ptr, like addAction.
                    [](const std::shared_ptr<Menu>& self,
                       const std::string& text)
                    {
                        return self->addSubMenu(text);
                    },
                    nb::arg("text"))
                .def(
                    "setSubMenuEnabled",
                    &Menu::setSubMenuEnabled,
                    nb::arg("menu"),
                    nb::arg("enabled"))
                .def("addDivider", &Menu::addDivider)
                .def("clear", &Menu::clear)
                .def(
                    "shortcut",
                    &Menu::shortcut,
                    nb::arg("key"),
                    nb::arg("modifiers"));
        }
    }
}
