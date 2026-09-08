// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/chrono.h>
#include <nanobind/stl/function.h>
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
        void diagSystem(nb::module_& m)
        {
            nb::class_<DiagFormat>(m, "DiagFormat")
                .def(nb::init<>())
                .def_rw("divisor", &DiagFormat::divisor)
                .def_rw("precision", &DiagFormat::precision);

            m.def(
                "diagText",
                &diagText,
                nb::arg("format"),
                nb::arg("diagFormat"),
                nb::arg("value"));

            nb::class_<DiagSystem, ISystem>(m, "DiagSystem")
                .def(
                    nb::new_(&DiagSystem::create),
                    nb::arg("context"))
                .def(
                    "addSampler",
                    &DiagSystem::addSampler,
                    nb::arg("id"),
                    nb::arg("sampler"),
                    nb::arg("format") = DiagFormat())
                .def("getFormat", &DiagSystem::getFormat, nb::arg("id"))
                .def_prop_ro("groups", &DiagSystem::getGroups)
                .def("getNames", &DiagSystem::getNames, nb::arg("group"))
                .def("hasSampler", &DiagSystem::hasSampler, nb::arg("id"))
                .def_prop_rw(
                    "samplesMax",
                    &DiagSystem::getSamplesMax,
                    &DiagSystem::setSamplesMax)
                .def("observeSamplesMax", &DiagSystem::observeSamplesMax)
                .def_prop_ro("samples", &DiagSystem::getSamples)
                .def_prop_rw(
                    "tickTime",
                    &DiagSystem::getTickTime,
                    &DiagSystem::setTickTime);
        }
    }
}
