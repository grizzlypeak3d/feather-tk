// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ImageWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void imageWidget(nb::module_& m)
        {
            nb::class_<ImageWidget, IWidget>(m, "ImageWidget")
                .def(
                    nb::new_(&ImageWidget::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("image", &ImageWidget::getImage, &ImageWidget::setImage)
                .def_prop_rw("marginRole",
                    &ImageWidget::getMarginRole,
                    &ImageWidget::setMarginRole);
        }
    }
}
