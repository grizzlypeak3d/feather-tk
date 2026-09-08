// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ProgressDialog.h>
#include <ftk/UI/Window.h>

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
        void progressDialog(nb::module_& m)
        {
            nb::class_<ProgressDialog, IDialog>(m, "ProgressDialog")
                .def(
                    nb::new_(&ProgressDialog::create),
                    nb::arg("context"),
                    nb::arg("title"),
                    nb::arg("text"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("text", &ProgressDialog::getText, &ProgressDialog::setText)
                .def_prop_rw("message", &ProgressDialog::getMessage, &ProgressDialog::setMessage)
                .def_prop_rw("range", &ProgressDialog::getRange, nb::overload_cast<const RangeD&>(&ProgressDialog::setRange), nb::rv_policy::copy)
                .def("setRange", nb::overload_cast<double, double>(&ProgressDialog::setRange))
                .def_prop_rw("value", &ProgressDialog::getValue, &ProgressDialog::setValue);
        }
    }
}
