// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/ToolBar.h>
#include <ftk/UI/ToolButton.h>

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
        namespace
        {
            //! Exposes the protected constructor and _init for Python-owned
            //! construction; adds no members, so the base storage fits.
            class PyToolBar : public ToolBar
            {
            public:
                template<typename... Args>
                void pyInit(Args&&... args)
                {
                    _init(std::forward<Args>(args)...);
                }
            };
        }

        void toolBar(nb::module_& m)
        {
            nb::class_<ToolBar, IContainer>(m, "ToolBar")
                .def(
                    "__init__",
                    // Python subclasses pass their own __init__ arguments
                    // through __new__, which a factory constructor cannot
                    // accept -- so construction is __init__-based (see
                    // WidgetTrampoline.h).
                    [](ToolBar* self,
                       const std::shared_ptr<Context>& context,
                       Orientation orientation,
                       const std::shared_ptr<IWidget>& parent)
                    {
                        pyConstruct<PyToolBar>(self,
                            [&](PyToolBar& w)
                            {
                                w.pyInit(context, orientation, parent);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("orientation") = Orientation::Horizontal,
                    nb::arg("parent") = nullptr)
                .def_prop_rw("marginRole", &ToolBar::getMarginRole, &ToolBar::setMarginRole)
                .def("addAction", &ToolBar::addAction)
                .def("addWidget", &ToolBar::addWidget)
                .def("clear", &ToolBar::clear);
        }
    }
}
