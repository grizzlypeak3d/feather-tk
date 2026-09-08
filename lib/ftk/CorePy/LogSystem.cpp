// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/LogSystem.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/chrono.h>
#include <nanobind/stl/function.h>
#include <nanobind/operators.h>
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
        void logSystem(nb::module_& m)
        {
            nb::enum_<LogType>(m, "LogType")
                .value("Message", LogType::Message)
                .value("Warning", LogType::Warning)
                .value("Error", LogType::Error);
            FTK_ENUM_BIND(m, LogType);
                
            nb::class_<LogItem>(m, "LogItem")
                .def(nb::init<>())
                .def_rw("time", &LogItem::time)
                .def_rw("prefix", &LogItem::prefix)
                .def_rw("message", &LogItem::message)
                .def_rw("type", &LogItem::type)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            m.def("getLabel", [](LogType value) { return getLabel(value); });

            observableList<LogItem>(m, "LogItem");

            nb::class_<LogSystem, IBaseSystem>(m, "LogSystem")
                .def(
                    nb::new_(&LogSystem::create),
                    nb::arg("context"))
                .def("print", &LogSystem::print)
                .def_prop_ro("observeLogItems", &LogSystem::observeLogItems);
        }
    }
}
