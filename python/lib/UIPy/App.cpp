// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UIPy/App.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Settings.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

using namespace pybind11::literals;
namespace py = pybind11;

namespace ftk
{
    class PyApp : public App
    {
    public:
        static std::shared_ptr<PyApp> create(
            const std::shared_ptr<Context>& context,
            const std::vector<std::string>& argv,
            const std::string& name,
            const std::string& summary,
            const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
            const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions)
        {
            auto out = std::shared_ptr<PyApp>(new PyApp);
            out->_init(context, argv, name, summary, cmdLineArgs, cmdLineOptions);
            return out;
        }

        virtual void tick() override
        {
            PYBIND11_OVERRIDE(
                void,
                App,
                tick);
        }
    };

    void app(py::module_& m)
    {
        py::class_<MonitorInfo>(m, "MonitorInfo")
            .def_readwrite("name", &MonitorInfo::name)
            .def_readwrite("size", &MonitorInfo::size)
            .def_readwrite("refreshRate", &MonitorInfo::refreshRate)
            .def_readwrite("dpi", &MonitorInfo::dpi)
            .def_readwrite("bounds", &MonitorInfo::bounds)
            .def(pybind11::self == pybind11::self)
            .def(pybind11::self != pybind11::self);

        //py::class_<App, IApp, std::shared_ptr<App> >(m, "App")
        py::class_<App, IApp, std::shared_ptr<App>, PyApp>(m, "App")
            .def(
                py::init(py::overload_cast<
                    const std::shared_ptr<Context>&,
                    const std::vector<std::string>&,
                    const std::string&,
                    const std::string&,
                    const std::vector<std::shared_ptr<ICmdLineArg> >&,
                    const std::vector<std::shared_ptr<ICmdLineOption> >&>(&PyApp::create)),
                py::arg("context"),
                py::arg("argv"),
                py::arg("name"),
                py::arg("summary"),
                py::arg("cmdLineArgs") = std::vector<std::shared_ptr<ICmdLineArg> >(),
                py::arg("cmdLineOptions") = std::vector<std::shared_ptr<ICmdLineOption> >())
            .def_property_readonly("windows", &App::getWindows)
            .def("observeMonitors", &App::observeMonitors)
            .def_property_readonly("fontSystem", &App::getFontSystem)
            .def_property_readonly("iconSystem", &App::getIconSystem)
            .def_property_readonly("style", &App::getStyle)
            .def_property("colorStyle", &App::getColorStyle, &App::setColorStyle)
            .def("observeColorStyle", &App::observeColorStyle)
            .def_property("customColorRoles", &App::getCustomColorRoles, &App::setCustomColorRoles)
            .def("observeCustomColorRoles", &App::observeCustomColorRoles)
            .def_property_readonly("defaultDisplayScale", &App::getDefaultDisplayScale)
            .def_property("displayScale", &App::getDisplayScale, &App::setDisplayScale)
            .def("observeDisplayScale", &App::observeDisplayScale)
            .def_property("tooltipsEnabled", &App::areTooltipsEnabled, &App::setTooltipsEnabled)
            .def("observeTooltipsEnabled", &App::observeTooltipsEnabled)
            .def("exit", &App::exit)
            .def("tick", &App::tick);
    }
}
