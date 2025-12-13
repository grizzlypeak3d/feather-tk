// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Window.h>

#include <ftk/UI/App.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/MenuBar.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        class PyMainWindow : public MainWindow
        {
        public:
            static std::shared_ptr<PyMainWindow> create(
                const std::shared_ptr<Context>& context,
                const std::shared_ptr<App>& app,
                const Size2I& size)
            {
                auto out = std::shared_ptr<PyMainWindow>(new PyMainWindow);
                out->_init(context, app, size);
                return out;
            }
            
            void setGeometry(const Box2I& value) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    setGeometry,
                    value);
            }
            
            void tickEvent(
                bool parentsVisible,
                bool parentsEnabled,
                const TickEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    tickEvent,
                    parentsVisible,
                    parentsEnabled,
                    event);
            }
            
            void sizeHintEvent(const SizeHintEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    sizeHintEvent,
                    event);
            }
            
            void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    drawEvent,
                    drawRect,
                    event);
            }
    
            void dragEnterEvent(DragDropEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    dragEnterEvent,
                    event);
            }

            void dragLeaveEvent(DragDropEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    dragLeaveEvent,
                    event);
            }

            void dragMoveEvent(DragDropEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    dragMoveEvent,
                    event);
            }

            void dropEvent(DragDropEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    MainWindow,
                    dropEvent,
                    event);
            }
        };

        void mainWindow(py::module_& m)
        {
            py::class_<MainWindow, Window, std::shared_ptr<MainWindow>, PyMainWindow >(m, "MainWindow")
                .def(
                    py::init(&PyMainWindow::create),
                    py::arg("context"),
                    py::arg("app"),
                    py::arg("size") = Size2I(1280, 960))
                .def_property("menuBar", &MainWindow::getMenuBar, &MainWindow::setMenuBar)
                .def_property("widget", &MainWindow::getWidget, &MainWindow::setWidget);
        }
    }
}
