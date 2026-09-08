// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void coreBind(nb::module_& m)
        {
            iSystem(m);
            observable(m);
            observableList(m);
            observableMap(m);

            box(m);
            cmdLine(m);
            color(m);
            command(m);
            context(m);
            diagSystem(m);
            fileLogSystem(m);
            fontSystem(m);
            iApp(m);
            iRender(m);
            image(m);
            logSystem(m);
            math(m);
            matrix(m);
            memory(m);
            mesh(m);
            noise(m);
            os(m);
            path(m);
            random(m);
            range(m);
            renderOptions(m);
            size(m);
            string(m);
            time(m);
            timer(m);
            vector(m);
            version(m);
        }
    }
}
