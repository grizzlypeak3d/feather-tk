// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/MenuBar.h>

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
            class PyMenuBar : public MenuBar
            {
            public:
                template<typename... Args>
                void pyInit(Args&&... args)
                {
                    _init(std::forward<Args>(args)...);
                }
            };
        }

        void menuBar(nb::module_& m)
        {
            nb::class_<MenuBar, IContainer>(m, "MenuBar")
                .def(
                    "__init__",
                    // Python subclasses pass their own __init__ arguments
                    // through __new__, which a factory constructor cannot
                    // accept -- so construction is __init__-based (see
                    // WidgetTrampoline.h).
                    [](MenuBar* self,
                       const std::shared_ptr<Context>& context,
                       const std::shared_ptr<IWidget>& parent)
                    {
                        pyConstruct<PyMenuBar>(self,
                            [&](PyMenuBar& w)
                            {
                                w.pyInit(context, parent);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def(
                    "setCurrentCallback",
                    // The highlighted item, for a status bar hint; None
                    // when the menus close.
                    &MenuBar::setCurrentCallback,
                    nb::arg("callback"))
                .def(
                    "addMenu",
                    nb::overload_cast<const std::string&>(&MenuBar::addMenu),
                    nb::arg("text"))
                .def(
                    "addMenu",
                    nb::overload_cast<
                        const std::string&,
                        const std::shared_ptr<Menu>&>(&MenuBar::addMenu),
                    nb::arg("text"),
                    nb::arg("menu"))
                .def(
                    "getMenu",
                    &MenuBar::getMenu,
                    nb::arg("text"))
                .def("clear", &MenuBar::clear)
                .def(
                    "shortcut",
                    &MenuBar::shortcut,
                    nb::arg("key"),
                    nb::arg("modifiers"));
        }
    }
}
