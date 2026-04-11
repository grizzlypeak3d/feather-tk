// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ImageWidget.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void imageWidget(py::module_& m)
        {
            py::class_<ImageWidget, IWidget, std::shared_ptr<ImageWidget> >(m, "ImageWidget")
                .def(
                    py::init(&ImageWidget::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("image", &ImageWidget::getImage, &ImageWidget::setImage)
                .def_property("marginRole",
                    &ImageWidget::getMarginRole,
                    &ImageWidget::setMarginRole);
        }
    }
}
