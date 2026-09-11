// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Settings.h>
#include <ftk/UI/Style.h>
#include <ftk/UI/Window.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/Context.h>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/filesystem.h>

using namespace nanobind::literals;
namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        class PyApp : public App
        {
        public:
            NB_TRAMPOLINE(App);

            // One control block for the life of the application, the way
            // pybind11's holder was: the windows store weak_ptrs to the app,
            // and a weak_ptr into nanobind's per-call control block expires
            // as soon as the call returns. The deleter does nothing because
            // the Python object owns the C++ one -- a block that owned a
            // Python reference would be a cycle the garbage collector
            // cannot see, and an application that never ran ("-h",
            // "-sysInfo") would be reported as leaked at exit. Made in the
            // constructor so that it is in place before anything casts the
            // instance: the shared_ptr caster reuses a live weak_from_this().
            PyApp() :
                _anchor(this, [](App*) {})
            {}

            void pyInit(
                const std::shared_ptr<Context>& context,
                const std::vector<std::string>& argv,
                const std::string& name,
                const std::string& summary,
                const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
                const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions,
                const AppFiles& appFiles)
            {
                _init(context, argv, name, summary, cmdLineArgs, cmdLineOptions, appFiles);
            }

            // run() is deliberately NOT trampolined. It is only ever
            // initiated from Python, where the subclass's own method runs
            // first and calls super().run() -- and nanobind's re-entrancy
            // guard only covers overrides entered through the trampoline,
            // so dispatching here would run the Python override a second
            // time (two main windows, one set of models). Nothing in C++
            // calls run() virtually. tick() stays trampolined: the C++
            // event loop dispatches it, which is the entry the guard
            // covers.
            virtual void tick() override
            {
                NB_OVERRIDE(tick);
            }

        private:
            std::shared_ptr<App> _anchor;
        };

        void app(nb::module_& m)
        {
            FTK_ENUM_PY(m, ColorStyle);
            FTK_ENUM_BIND(m, ColorStyle);

            nb::class_<ColorControls>(m, "ColorControls")
                .def(nb::init<>())
                .def_rw("brightness", &ColorControls::brightness)
                .def_rw("contrast", &ColorControls::contrast)
                .def_rw("disabledAlpha", &ColorControls::disabledAlpha)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<Style>(m, "Style")
                .def_prop_rw(
                    "colorControls",
                    &Style::getColorControls,
                    &Style::setColorControls,
                    nb::rv_policy::copy)
                .def_prop_rw(
                    "fonts",
                    &Style::getFonts,
                    &Style::setFonts,
                    nb::rv_policy::copy)
                .def(
                    "getSizeRole",
                    &Style::getSizeRole,
                    nb::arg("role"),
                    nb::arg("scale"))
                .def(
                    "getColorRole",
                    nb::overload_cast<ColorRole>(&Style::getColorRole, nb::const_),
                    nb::arg("role"));

            nb::class_<MonitorInfo>(m, "MonitorInfo")
                .def_rw("name", &MonitorInfo::name)
                .def_rw("size", &MonitorInfo::size)
                .def_rw("refreshRate", &MonitorInfo::refreshRate)
                .def_rw("dpi", &MonitorInfo::dpi)
                .def_rw("bounds", &MonitorInfo::bounds)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<AppFiles>(m, "AppFiles")
                .def(
                    "__init__",
                    [](AppFiles* self,
                        const std::string& dirName,
                        const std::string& baseName,
                        int version)
                    {
                        new (self) AppFiles{ dirName, baseName, version };
                    },
                    nb::arg("dirName") = std::string(),
                    nb::arg("baseName") = std::string(),
                    nb::arg("version") = 0)
                .def_rw("dirName", &AppFiles::dirName)
                .def_rw("baseName", &AppFiles::baseName)
                .def_rw("version", &AppFiles::version);

            //nb::class_<App, IApp>(m, "App")
            nb::class_<App, IApp, PyApp>(m, "App")
                .def(
                    "__init__",
                    [](App* self,
                       const std::shared_ptr<Context>& context,
                       const std::vector<std::string>& argv,
                       const std::string& name,
                       const std::string& summary,
                       const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
                       const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions,
                       const AppFiles& appFiles)
                    {
                        pyConstruct<PyApp>(self,
                            [&](PyApp& a)
                            {
                                a.pyInit(
                                    context, argv, name, summary,
                                    cmdLineArgs, cmdLineOptions, appFiles);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("argv"),
                    nb::arg("name"),
                    nb::arg("summary"),
                    nb::arg("cmdLineArgs") = std::vector<std::shared_ptr<ICmdLineArg> >(),
                    nb::arg("cmdLineOptions") = std::vector<std::shared_ptr<ICmdLineOption> >(),
                    nb::arg("appFiles") = AppFiles())
                .def_prop_ro("settings", &App::getSettings)
                .def_prop_ro("settingsPath", &App::getSettingsPath)
                .def_prop_ro("logFilePath", &App::getLogFilePath)
                .def_prop_ro("windows", &App::getWindows)
                .def_prop_ro("observeMonitors", &App::observeMonitors)
                .def_prop_ro("fontSystem", &App::getFontSystem)
                .def_prop_ro("iconSystem", &App::getIconSystem)
                .def_prop_ro("style", &App::getStyle)
                .def_prop_rw("colorStyle", &App::getColorStyle, &App::setColorStyle)
                .def_prop_ro("observeColorStyle", &App::observeColorStyle)
                .def_prop_ro("defaultDisplayScale", &App::getDefaultDisplayScale)
                .def_prop_rw("displayScale", &App::getDisplayScale, &App::setDisplayScale)
                .def_prop_ro("observeDisplayScale", &App::observeDisplayScale)
                .def_prop_rw("tooltipsEnabled", &App::areTooltipsEnabled, &App::setTooltipsEnabled)
                .def_prop_ro("observeTooltipsEnabled", &App::observeTooltipsEnabled)
                .def_static("setOffscreenDefault", &App::setOffscreenDefault, nb::arg("value"))
                .def("writeScreenshot", &App::writeScreenshot, nb::arg("path"))
                .def("writeWidgetDump", &App::writeWidgetDump, nb::arg("path"))
                .def("exit", &App::exit)
                // Self as a shared_ptr: the run loop hands weak references
                // to the app out through shared_from_this() -- screenshots,
                // widget dumps, captures -- which only works while a C++
                // shared_ptr for the instance exists.
                .def(
                    "run",
                    [](const std::shared_ptr<App>& self)
                    {
                        self->run();
                    })
                .def(
                    "tick",
                    [](const std::shared_ptr<App>& self)
                    {
                        self->tick();
                    });
        }
    }
}
