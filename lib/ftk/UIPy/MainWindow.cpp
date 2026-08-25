// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

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
                // The event is passed by pointer: passed by reference,
                // pybind hands Python a copy, and anything Python writes
                // to it -- the accept flag -- is written to the copy.
                pybind11::gil_scoped_acquire gil;
                if (pybind11::function override = pybind11::get_override(
                    static_cast<const MainWindow*>(this), "dragEnterEvent"))
                {
                    override(&event);
                }
                else
                {
                    MainWindow::dragEnterEvent(event);
                }
            }

            void dragLeaveEvent(DragDropEvent& event) override
            {
                // The event is passed by pointer: passed by reference,
                // pybind hands Python a copy, and anything Python writes
                // to it -- the accept flag -- is written to the copy.
                pybind11::gil_scoped_acquire gil;
                if (pybind11::function override = pybind11::get_override(
                    static_cast<const MainWindow*>(this), "dragLeaveEvent"))
                {
                    override(&event);
                }
                else
                {
                    MainWindow::dragLeaveEvent(event);
                }
            }

            void dragMoveEvent(DragDropEvent& event) override
            {
                // The event is passed by pointer: passed by reference,
                // pybind hands Python a copy, and anything Python writes
                // to it -- the accept flag -- is written to the copy.
                pybind11::gil_scoped_acquire gil;
                if (pybind11::function override = pybind11::get_override(
                    static_cast<const MainWindow*>(this), "dragMoveEvent"))
                {
                    override(&event);
                }
                else
                {
                    MainWindow::dragMoveEvent(event);
                }
            }

            void dropEvent(DragDropEvent& event) override
            {
                // The event is passed by pointer: passed by reference,
                // pybind hands Python a copy, and anything Python writes
                // to it -- the accept flag -- is written to the copy.
                pybind11::gil_scoped_acquire gil;
                if (pybind11::function override = pybind11::get_override(
                    static_cast<const MainWindow*>(this), "dropEvent"))
                {
                    override(&event);
                }
                else
                {
                    MainWindow::dropEvent(event);
                }
            }

            void keyPressEvent(KeyEvent& event) override
            {
                // The event is passed by pointer: passed by reference,
                // pybind hands Python a copy, and anything Python writes
                // to it -- the accept flag -- is written to the copy.
                pybind11::gil_scoped_acquire gil;
                if (pybind11::function override = pybind11::get_override(
                    static_cast<const MainWindow*>(this), "keyPressEvent"))
                {
                    override(&event);
                }
                else
                {
                    MainWindow::keyPressEvent(event);
                }
            }

            void keyReleaseEvent(KeyEvent& event) override
            {
                // The event is passed by pointer: passed by reference,
                // pybind hands Python a copy, and anything Python writes
                // to it -- the accept flag -- is written to the copy.
                pybind11::gil_scoped_acquire gil;
                if (pybind11::function override = pybind11::get_override(
                    static_cast<const MainWindow*>(this), "keyReleaseEvent"))
                {
                    override(&event);
                }
                else
                {
                    MainWindow::keyReleaseEvent(event);
                }
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
