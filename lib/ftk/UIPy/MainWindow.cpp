// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/App.h>
#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/MainWindow.h>
#include <ftk/UI/MenuBar.h>

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
        class PyMainWindow : public MainWindow
        {
        public:
            NB_TRAMPOLINE(MainWindow);
            using Base = MainWindow;

            void pyInit(
                const std::shared_ptr<Context>& context,
                const std::shared_ptr<App>& app,
                const Size2I& size)
            {
                _init(context, app, size);
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

            FTK_WIDGET_EVENT_REF(dragEnterEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(dragLeaveEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(dragMoveEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(dropEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(keyPressEvent, KeyEvent)
            FTK_WIDGET_EVENT_REF(keyReleaseEvent, KeyEvent)
        };

        void mainWindow(nb::module_& m)
        {
            nb::class_<MainWindow, Window, PyMainWindow >(m, "MainWindow")
                .def(
                    "__init__",
                    [](MainWindow* self,
                       const std::shared_ptr<Context>& context,
                       const std::shared_ptr<App>& app,
                       const Size2I& size)
                    {
                        pyConstruct<PyMainWindow>(self,
                            [&](PyMainWindow& w)
                            {
                                w.pyInit(context, app, size);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("app"),
                    nb::arg("size") = Size2I(1280, 960))
                .def_prop_rw("menuBar", &MainWindow::getMenuBar, &MainWindow::setMenuBar)
                .def_prop_rw("widget", &MainWindow::getWidget, &MainWindow::setWidget);
        }
    }
}
