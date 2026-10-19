// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/IDialog.h>
#include <ftk/UI/Window.h>

#include <nanobind/nanobind.h>
#include <ftk/CorePy/Function.h>
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
        void iDialog(nb::module_& m)
        {
            FTK_ENUM_PY(m, DialogSize);
            FTK_ENUM_BIND(m, DialogSize);

            nb::class_<IDialog, IPopup>(m, "IDialog")
                .def("open", &IDialog::open)
                .def("isOpen", &IDialog::isOpen)
                .def("setCloseCallback", &IDialog::setCloseCallback)
                .def_prop_ro("dialogSize", &IDialog::getDialogSize)
                .def_prop_ro("textWidth", &IDialog::getTextWidth)
                .def("setDialogSize", &IDialog::setDialogSize,
                    nb::arg("size"), nb::arg("textWidth") = 100);
        }
    }
}
