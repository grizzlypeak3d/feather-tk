// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UIPy/Window.h>

#include <ftk/UI/App.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/MenuBar.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
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
            .def("setWidget", &MainWindow::setWidget);
    }
}
