// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Observable.h>

#include <ftk/Core/Observable.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void observable(py::module_& m)
        {
            py::enum_<ObserverAction>(m, "ObserverAction")
                .value("Trigger", ObserverAction::Trigger)
                .value("Suppress", ObserverAction::Suppress);

            observable<int>(m, "Int");
            observable<float>(m, "Float");
            observable<double>(m, "Double");
            observable<bool>(m, "Bool");
            observable<std::string>(m, "String");
        }
    }
}
