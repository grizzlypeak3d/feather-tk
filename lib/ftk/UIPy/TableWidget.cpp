// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/TableWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void tableWidget(nb::module_& m)
        {
            nb::class_<TableCell>(m, "TableCell")
                .def(nb::init<>())
                .def(
                    nb::init<const std::string&, bool, ColorRole>(),
                    nb::arg("text"),
                    nb::arg("editable") = false,
                    nb::arg("colorRole") = ColorRole::None)
                .def_rw("text", &TableCell::text)
                .def_rw("editable", &TableCell::editable)
                .def_rw("colorRole", &TableCell::colorRole)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<TableRow>(m, "TableRow")
                .def(nb::init<>())
                .def(
                    nb::init<const std::vector<TableCell>&, bool>(),
                    nb::arg("cells"),
                    nb::arg("heading") = false)
                .def_rw("cells", &TableRow::cells)
                .def_rw("heading", &TableRow::heading)
                .def_rw("visible", &TableRow::visible)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<TableIndex>(m, "TableIndex")
                .def(nb::init<>())
                .def(
                    nb::init<int, int>(),
                    nb::arg("row"),
                    nb::arg("column"))
                .def_rw("row", &TableIndex::row)
                .def_rw("column", &TableIndex::column)
                .def("isValid", &TableIndex::isValid)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<TableWidget, IMouseWidget>(m, "TableWidget")
                .def(
                    nb::new_(&TableWidget::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("rows", &TableWidget::getRows, &TableWidget::setRows)
                .def_prop_ro("columnCount", &TableWidget::getColumnCount)
                .def("getColumnStretch", &TableWidget::getColumnStretch, nb::arg("column"))
                .def(
                    "setColumnStretch",
                    &TableWidget::setColumnStretch,
                    nb::arg("column"),
                    nb::arg("stretch"))
                .def_prop_rw("current", &TableWidget::getCurrent, &TableWidget::setCurrent)
                .def("setCallback", &TableWidget::setCallback)
                .def("getCellRect", &TableWidget::getCellRect, nb::arg("index"))
                .def_prop_ro("editor", &TableWidget::getEditor)
                .def_prop_ro("editorIndex", &TableWidget::getEditorIndex)
                .def(
                    "openEditor",
                    &TableWidget::openEditor,
                    nb::arg("index"),
                    nb::arg("widget"))
                .def("closeEditor", &TableWidget::closeEditor)
                .def_prop_rw("marginRole", &TableWidget::getMarginRole, &TableWidget::setMarginRole)
                .def_prop_rw("columnLines", &TableWidget::hasColumnLines, &TableWidget::setColumnLines)
                .def_prop_rw("headingRole", &TableWidget::getHeadingRole, &TableWidget::setHeadingRole);
        }
    }
}
