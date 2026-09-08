// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/MDICanvas.h>

#include <nanobind/nanobind.h>
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
        void mdiCanvas(nb::module_& m)
        {
            nb::class_<MDICanvas, IWidget>(m, "MDICanvas")
                .def(
                    nb::new_(&MDICanvas::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("canvasSize", &MDICanvas::getCanvasSize, &MDICanvas::setCanvasSize, nb::rv_policy::copy)
                .def_prop_rw("gridSize", &MDICanvas::getGridSize, &MDICanvas::setGridSize, nb::rv_policy::copy)
                .def("addWidget", &MDICanvas::addWidget)
                .def("setChildGeometryCallback", &MDICanvas::setChildGeometryCallback);
        }
    }
}
