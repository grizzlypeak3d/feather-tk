// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IWidget.h>

#include <nanobind/nanobind.h>
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
        class PyIMouseWidget : public PyWidget<IMouseWidget>
        {
        public:
            void pyInit(
                const std::shared_ptr<Context>& context,
                const std::string& objectName,
                const std::shared_ptr<IWidget>& parent)
            {
                _init(context, objectName, parent);
            }

            // The protected helpers, so a Python subclass can turn the
            // interactions on and ask about them -- a drag source needs
            // the press and where it started.
            using IMouseWidget::_setMouseHoverEnabled;
            using IMouseWidget::_setMousePressEnabled;
            using IMouseWidget::_isMouseInside;
            using IMouseWidget::_getMousePos;
            using IMouseWidget::_isMousePressed;
            using IMouseWidget::_getMousePressPos;
        };

        void iMouseWidget(nb::module_& m)
        {
            nb::class_<
                IMouseWidget,
                IWidget,
                PyIMouseWidget>(m, "IMouseWidget")
                .def(
                    "__init__",
                    [](IMouseWidget* self,
                       const std::shared_ptr<Context>& context,
                       const std::string& objectName,
                       const std::shared_ptr<IWidget>& parent)
                    {
                        pyConstruct<PyIMouseWidget>(self,
                            [&](PyIMouseWidget& w)
                            {
                                w.pyInit(context, objectName, parent);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("objectName"),
                    nb::arg("parent") = nullptr)
                .def(
                    "_setMouseHoverEnabled",
                    &PyIMouseWidget::_setMouseHoverEnabled,
                    nb::arg("value"))
                .def(
                    "_setMousePressEnabled",
                    &PyIMouseWidget::_setMousePressEnabled,
                    nb::arg("value"),
                    nb::arg("button") = MouseButton::Left,
                    nb::arg("modifiers") = -1)
                .def("_isMouseInside", &PyIMouseWidget::_isMouseInside)
                .def(
                    "_getMousePos",
                    &PyIMouseWidget::_getMousePos,
                    nb::rv_policy::copy)
                .def("_isMousePressed", &PyIMouseWidget::_isMousePressed)
                .def(
                    "_getMousePressPos",
                    &PyIMouseWidget::_getMousePressPos,
                    nb::rv_policy::copy);
        }
    }
}
