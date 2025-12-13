// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void coreBind(py::module_& m)
        {
            iSystem(m);
            observable(m);
            observableList(m);
            observableMap(m);

            box(m);
            cmdLine(m);
            color(m);
            context(m);
            fontSystem(m);
            iApp(m);
            iRender(m);
            image(m);
            logSystem(m);
            memory(m);
            os(m);
            path(m);
            random(m);
            range(m);
            size(m);
            string(m);
            timer(m);
            vector(m);
        }
    }
}
