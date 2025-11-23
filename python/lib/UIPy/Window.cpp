// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UIPy/Window.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Window.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    /*class PyWindow : public Window
    {
    public:
        static std::shared_ptr<PyWindow> create(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::string& name,
            const Size2I& size)
        {
            auto out = std::shared_ptr<PyWindow>(new PyWindow);
            out->_init(context, app, name, size);
            return out;
        }

        virtual void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
        {
            PYBIND11_OVERRIDE(
                void,
                Window,
                drawEvent,
                drawRect,
                event);
        }
    };*/

    void window(py::module_& m)
    {
        //py::class_<Window, IWindow, std::shared_ptr<Window>, PyWindow>(m, "Window")
        py::class_<Window, IWindow, std::shared_ptr<Window> >(m, "Window")
            .def(
                py::init(&Window::create),
                py::arg("context"),
                py::arg("app"),
                py::arg("name"),
                py::arg("size"));
    }
}
