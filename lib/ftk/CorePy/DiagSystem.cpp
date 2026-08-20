// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void diagSystem(py::module_& m)
        {
            py::class_<DiagFormat>(m, "DiagFormat")
                .def(py::init<>())
                .def_readwrite("divisor", &DiagFormat::divisor)
                .def_readwrite("precision", &DiagFormat::precision);

            m.def(
                "diagText",
                &diagText,
                py::arg("format"),
                py::arg("diagFormat"),
                py::arg("value"));

            py::class_<DiagSystem, ISystem, std::shared_ptr<DiagSystem> >(m, "DiagSystem")
                .def(
                    py::init(&DiagSystem::create),
                    py::arg("context"))
                .def(
                    "addSampler",
                    &DiagSystem::addSampler,
                    py::arg("id"),
                    py::arg("sampler"),
                    py::arg("format") = DiagFormat())
                .def("getFormat", &DiagSystem::getFormat, py::arg("id"))
                .def_property_readonly("groups", &DiagSystem::getGroups)
                .def("getNames", &DiagSystem::getNames, py::arg("group"))
                .def("hasSampler", &DiagSystem::hasSampler, py::arg("id"))
                .def_property(
                    "samplesMax",
                    &DiagSystem::getSamplesMax,
                    &DiagSystem::setSamplesMax)
                .def_property_readonly("samples", &DiagSystem::getSamples)
                .def_property(
                    "tickTime",
                    &DiagSystem::getTickTime,
                    &DiagSystem::setTickTime);
        }
    }
}
