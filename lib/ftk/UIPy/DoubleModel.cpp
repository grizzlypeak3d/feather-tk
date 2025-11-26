// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/DoubleModel.h>

#include <ftk/UI/DoubleModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void doubleModel(py::module_& m)
        {
            py::class_<DoubleModel, std::shared_ptr<DoubleModel> >(m, "DoubleModel")
                .def(py::init(&DoubleModel::create),
                    py::arg("context"));
        }
    }
}
