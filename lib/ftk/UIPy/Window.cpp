// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/App.h>
#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/Window.h>

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
        class PyWindow : public Window
        {
        public:
            NB_TRAMPOLINE(Window);

            void pyInit(
                const std::shared_ptr<Context>& context,
                const std::shared_ptr<App>& app,
                const std::string& name,
                const Size2I& size)
            {
                _init(context, app, name, size);
            }
            
            void setGeometry(const Box2I& value) override
            {
                NB_OVERRIDE(setGeometry, value);
            }
            
            void tickEvent(
                bool parentsVisible,
                bool parentsEnabled,
                const TickEvent& event) override
            {
                NB_OVERRIDE(tickEvent, parentsVisible, parentsEnabled, event);
            }
            
            void sizeHintEvent(const SizeHintEvent& event) override
            {
                NB_OVERRIDE(sizeHintEvent, event);
            }

            void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
            {
                NB_OVERRIDE(drawEvent, drawRect, event);
            }
        };

        void window(nb::module_& m)
        {
            //nb::class_<Window, IWindow>(m, "Window")
            nb::class_<Window, IWindow, PyWindow>(m, "Window")
                .def(
                    "__init__",
                    [](Window* self,
                       const std::shared_ptr<Context>& context,
                       const std::shared_ptr<App>& app,
                       const std::string& name,
                       const Size2I& size)
                    {
                        pyConstruct<PyWindow>(self,
                            [&](PyWindow& w)
                            {
                                w.pyInit(context, app, name, size);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("app"),
                    nb::arg("name"),
                    nb::arg("size") = Size2I(1280, 960));
        }
    }
}
