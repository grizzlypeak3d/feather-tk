// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/LogSystem.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void logSystem(py::module_& m)
        {
            py::enum_<LogType>(m, "LogType")
                .value("Message", LogType::Message)
                .value("Warning", LogType::Warning)
                .value("Error", LogType::Error);
            FTK_ENUM_BIND(m, LogType);
                
            py::class_<LogItem>(m, "LogItem")
                .def(py::init<>())
                .def_readwrite("time", &LogItem::time)
                .def_readwrite("prefix", &LogItem::prefix)
                .def_readwrite("message", &LogItem::message)
                .def_readwrite("type", &LogItem::type)
                .def(pybind11::self == pybind11::self)
                .def(pybind11::self != pybind11::self);

            m.def("getLabel", [](LogType value) { return getLabel(value); });

            observableList<LogItem>(m, "LogItem");

            py::class_<LogSystem, IBaseSystem, std::shared_ptr<LogSystem> >(m, "LogSystem")
                .def(
                    py::init(&LogSystem::create),
                    py::arg("context"))
                .def("print", &LogSystem::print)
                .def_property_readonly("observeLogItems", &LogSystem::observeLogItems);
        }
    }
}
