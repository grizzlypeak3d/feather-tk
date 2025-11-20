// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CorePy/Bindings.h>

#include <CorePy/Box.h>
#include <CorePy/CmdLine.h>
#include <CorePy/Color.h>
#include <CorePy/Context.h>
#include <CorePy/FontSystem.h>
#include <CorePy/IApp.h>
#include <CorePy/IRender.h>
#include <CorePy/ISystem.h>
#include <CorePy/Image.h>
#include <CorePy/Memory.h>
#include <CorePy/Observable.h>
#include <CorePy/ObservableList.h>
#include <CorePy/ObservableMap.h>
#include <CorePy/ObservableValue.h>
#include <CorePy/OS.h>
#include <CorePy/Path.h>
#include <CorePy/Random.h>
#include <CorePy/Range.h>
#include <CorePy/Size.h>
#include <CorePy/String.h>
#include <CorePy/Timer.h>
#include <CorePy/Vector.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    void coreBind(py::module_& m)
    {
        iSystem(m);

        box(m);
        cmdLine(m);
        color(m);
        context(m);
        fontSystem(m);
        iApp(m);
        iRender(m);
        image(m);
        memory(m);
        observable(m);
        observableList(m);
        observableMap(m);
        observableValue(m);
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
