// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScreenshotTag.h>

#include <nanobind/nanobind.h>
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
        void screenshotTag(nb::module_& m)
        {
            m.def(
                "setScreenshotTag",
                &setScreenshotTag,
                nb::arg("widget"),
                nb::arg("id"),
                "Tag a widget so a documentation screenshot tool can find it and emit its bounding box.");
            m.def(
                "hasScreenshotTag",
                &hasScreenshotTag,
                nb::arg("widget"),
                "Returns true if the widget carries a screenshot tag (including an empty one).");
            m.def(
                "getScreenshotTag",
                &getScreenshotTag,
                nb::arg("widget"),
                "Get a widget's screenshot tag, or an empty string if it has none.");
        }
    }
}
