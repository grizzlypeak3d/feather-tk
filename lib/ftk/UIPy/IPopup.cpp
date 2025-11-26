// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/IPopup.h>

#include <ftk/UI/IPopup.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void iPopup(py::module_& m)
        {
            py::class_<IPopup, IWidget, std::shared_ptr<IPopup> >(m, "IPopup")
                .def("close", &IPopup::close);
        }
    }
}
