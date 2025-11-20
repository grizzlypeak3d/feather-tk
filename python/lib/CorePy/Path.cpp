// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CorePy/Color.h>

#include <ftk/Core/Path.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace ftk
{
    void path(py::module_& m)
    {
        py::class_<Path>(m, "Path")
            .def(py::init<>())
            .def(py::init<std::string>())
            .def_property_readonly("get", &Path::get)
            .def_property_readonly("isEmpty", &Path::isEmpty)
            .def_property_readonly("hasProtocol", &Path::hasProtocol)
            .def_property_readonly("hasDir", &Path::hasDir)
            .def_property_readonly("hasBase", &Path::hasBase)
            .def_property_readonly("hasNum", &Path::hasNum)
            .def_property_readonly("hasExt", &Path::hasExt)
            .def_property_readonly("hasRequest", &Path::hasRequest)
            .def_property_readonly("protocol", &Path::getProtocol)
            .def_property_readonly("dir", &Path::getDir)
            .def_property_readonly("base", &Path::getBase)
            .def_property_readonly("num", &Path::getNum)
            .def_property_readonly("pad", &Path::getPad)
            .def_property_readonly("ext", &Path::getExt)
            .def_property_readonly("request", &Path::getRequest)
            .def_property_readonly("fileName", &Path::getFileName);
    }
}
