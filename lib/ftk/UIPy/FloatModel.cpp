// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FloatModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void floatModel(py::module_& m)
        {
            py::class_<FloatModel, std::shared_ptr<FloatModel> >(m, "FloatModel")
                .def(py::init(&FloatModel::create),
                    py::arg("context"));
        }
    }
}
