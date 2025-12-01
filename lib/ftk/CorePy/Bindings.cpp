// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/CorePy/Box.h>
#include <ftk/CorePy/CmdLine.h>
#include <ftk/CorePy/Color.h>
#include <ftk/CorePy/Context.h>
#include <ftk/CorePy/FontSystem.h>
#include <ftk/CorePy/IApp.h>
#include <ftk/CorePy/IRender.h>
#include <ftk/CorePy/ISystem.h>
#include <ftk/CorePy/Image.h>
#include <ftk/CorePy/LogSystem.h>
#include <ftk/CorePy/Memory.h>
#include <ftk/CorePy/Observable.h>
#include <ftk/CorePy/ObservableList.h>
#include <ftk/CorePy/ObservableMap.h>
#include <ftk/CorePy/ObservableValue.h>
#include <ftk/CorePy/OS.h>
#include <ftk/CorePy/Path.h>
#include <ftk/CorePy/Random.h>
#include <ftk/CorePy/Range.h>
#include <ftk/CorePy/Size.h>
#include <ftk/CorePy/String.h>
#include <ftk/CorePy/Timer.h>
#include <ftk/CorePy/Vector.h>

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
            observableValue(m);

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
