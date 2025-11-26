// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/IWindow.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IWindow.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        class PyIWindow : public IWindow
        {
        public:
            virtual void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IWindow,
                    drawEvent,
                    drawRect,
                    event);
            }
        };

        void iWindow(py::module_& m)
        {
            //py::class_<IWindow, IWidget, std::shared_ptr<IWindow> >(m, "IWindow")
            py::class_<IWindow, IWidget, std::shared_ptr<IWindow>, PyIWindow>(m, "IWindow")
                .def_property_readonly("app", &IWindow::getApp)
                .def_property_readonly("windowID", &IWindow::getID)
                .def_property_readonly("screen", &IWindow::getScreen)
                .def_property("title", &IWindow::getTitle, &IWindow::setTitle)
                .def_property("size", &IWindow::getSize, &IWindow::setSize)
                .def_property("minSize", &IWindow::getMinSize, &IWindow::setMinSize)
                .def_property("fullScreen", &IWindow::isFullScreen, &IWindow::setFullScreen)
                .def("observeFullScreen", &IWindow::observeFullScreen)
                .def_property("floatOnTop", &IWindow::isFloatOnTop, &IWindow::setFloatOnTop)
                .def("observeFloatOnTop", &IWindow::observeFloatOnTop)
                .def_property_readonly("frameBufferSize", &IWindow::getFrameBufferSize)
                .def_property("frameBufferType", &IWindow::getFrameBufferType, &IWindow::setFrameBufferType)
                .def("observeFrameBufferType", &IWindow::observeFrameBufferType)
                .def_property_readonly("contentScale", &IWindow::getContentScale)
                .def_property("displayScale", &IWindow::getDisplayScale, &IWindow::setDisplayScale)
                .def("observeDisplayScale", &IWindow::observeDisplayScale).def_property("keyFocus", &IWindow::getKeyFocus, &IWindow::setKeyFocus)
                .def("getNextKeyFocus", &IWindow::getNextKeyFocus)
                .def("getPrevKeyFocus", &IWindow::getPrevKeyFocus)
                .def_property("tooltipsEnabled", &IWindow::getTooltipsEnabled, &IWindow::setTooltipsEnabled)
                .def("setIcon", &IWindow::setIcon)
                .def("screenshot", &IWindow::screenshot)
                .def("close", &IWindow::close)
                .def("setCloseCallback", &IWindow::setCloseCallback);
        }
    }
}
