// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IWindow.h>

#include <nanobind/trampoline.h>

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
        class PyIWindow : public IWindow
        {
        public:
            NB_TRAMPOLINE(IWindow);

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

        void iWindow(nb::module_& m)
        {
            //nb::class_<IWindow, IWidget>(m, "IWindow")
            nb::class_<IWindow, IWidget, PyIWindow>(m, "IWindow")
                .def_prop_ro("app", &IWindow::getApp)
                .def_prop_ro("windowID", &IWindow::getID)
                .def_prop_ro("screen", &IWindow::getScreen)
                .def_prop_rw("title", &IWindow::getTitle, &IWindow::setTitle)
                .def_prop_rw("size", &IWindow::getSize, &IWindow::setSize, nb::rv_policy::copy)
                .def_prop_rw("minSize", &IWindow::getMinSize, &IWindow::setMinSize)
                .def_prop_rw("fullScreen", &IWindow::isFullScreen, &IWindow::setFullScreen)
                .def_prop_ro("observeFullScreen", &IWindow::observeFullScreen)
                .def_prop_rw("floatOnTop", &IWindow::isFloatOnTop, &IWindow::setFloatOnTop)
                .def_prop_ro("observeFloatOnTop", &IWindow::observeFloatOnTop)
                .def("getWindowInfo", &IWindow::getWindowInfo)
                .def_prop_ro("bufferSize", &IWindow::getBufferSize, nb::rv_policy::copy)
                .def_prop_rw("bufferType", &IWindow::getBufferType, &IWindow::setBufferType)
                .def_prop_ro("observeBufferType", &IWindow::observeBufferType)
                .def_prop_ro("contentScale", &IWindow::getContentScale)
                .def_prop_rw("displayScale", &IWindow::getDisplayScale, &IWindow::setDisplayScale)
                .def_prop_ro("observeDisplayScale", &IWindow::observeDisplayScale).
                def_prop_rw("keyFocus", &IWindow::getKeyFocus, &IWindow::setKeyFocus)
                .def("getNextKeyFocus", &IWindow::getNextKeyFocus)
                .def("getPrevKeyFocus", &IWindow::getPrevKeyFocus)
                .def_prop_rw("tooltipsEnabled", &IWindow::getTooltipsEnabled, &IWindow::setTooltipsEnabled)
                .def_prop_ro("cursorPos", &IWindow::getCursorPos)
                .def_prop_ro("dragDropActive", &IWindow::isDragDropActive)
                .def(
                    "layout",
                    &IWindow::layout,
                    nb::arg("size"))
                .def(
                    "click",
                    &IWindow::click,
                    nb::arg("pos"),
                    nb::arg("button") = MouseButton::Left,
                    nb::arg("modifiers") = 0)
                .def(
                    "drag",
                    &IWindow::drag,
                    nb::arg("path"),
                    nb::arg("modifiers") = 0,
                    nb::arg("release") = true)
                .def(
                    "keyPress",
                    &IWindow::keyPress,
                    nb::arg("key"),
                    nb::arg("modifiers") = 0)
                .def(
                    "text",
                    &IWindow::text,
                    nb::arg("text"))
                .def(
                    "textEditing",
                    &IWindow::textEditing,
                    nb::arg("text"),
                    nb::arg("cursor") = 0)
                .def("setIcon", &IWindow::setIcon)
                .def("screenshot", &IWindow::screenshot)
                .def("close", &IWindow::close)
                .def("setCloseCallback", &IWindow::setCloseCallback);
        }
    }
}
