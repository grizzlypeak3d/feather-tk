// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/MDIWidget.h>

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
        void mdiWidget(nb::module_& m)
        {
            FTK_ENUM_PY(m, MDIResize);
            FTK_ENUM_BIND(m, MDIResize);

            nb::class_<MDIWidget, IWidget>(m, "MDIWidget")
                .def(
                    nb::new_(&MDIWidget::create),
                    nb::arg("context"),
                    nb::arg("title"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("title", &MDIWidget::getTitle, &MDIWidget::setTitle)
                .def_prop_rw("widget", &MDIWidget::getWidget, &MDIWidget::setWidget)
                .def("setPressCallback", &MDIWidget::setPressCallback)
                .def("setMoveCallback", &MDIWidget::setMoveCallback)
                .def("setResizeCallback", &MDIWidget::setResizeCallback);
        }
    }
}
