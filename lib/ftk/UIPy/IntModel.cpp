// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IntModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void intModel(py::module_& m)
        {
            py::class_<IntModel, std::shared_ptr<IntModel> >(m, "IntModel")
                .def(py::init(&IntModel::create),
                    py::arg("context"));
        }
    }
}
