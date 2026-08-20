// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScreenshotTag.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void screenshotTag(py::module_& m)
        {
            m.def(
                "setScreenshotTag",
                &setScreenshotTag,
                py::arg("widget"),
                py::arg("id"),
                "Tag a widget so a documentation screenshot tool can find it and emit its bounding box.");
            m.def(
                "hasScreenshotTag",
                &hasScreenshotTag,
                py::arg("widget"),
                "Returns true if the widget carries a screenshot tag (including an empty one).");
            m.def(
                "getScreenshotTag",
                &getScreenshotTag,
                py::arg("widget"),
                "Get a widget's screenshot tag, or an empty string if it has none.");
        }
    }
}
