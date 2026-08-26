// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        class PyIMouseWidget : public PyWidget<IMouseWidget>
        {
        public:
            static std::shared_ptr<PyIMouseWidget> create(
                const std::shared_ptr<Context>& context,
                const std::string& objectName,
                const std::shared_ptr<IWidget>& parent = nullptr)
            {
                auto out = std::shared_ptr<PyIMouseWidget>(new PyIMouseWidget);
                out->_init(context, objectName, parent);
                return out;
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

        void iMouseWidget(py::module_& m)
        {
            py::class_<
                IMouseWidget,
                IWidget,
                std::shared_ptr<IMouseWidget>,
                PyIMouseWidget>(m, "IMouseWidget")
                .def(
                    py::init(&PyIMouseWidget::create),
                    py::arg("context"),
                    py::arg("objectName"),
                    py::arg("parent") = nullptr)
                .def(
                    "_setMouseHoverEnabled",
                    &PyIMouseWidget::_setMouseHoverEnabled,
                    py::arg("value"))
                .def(
                    "_setMousePressEnabled",
                    &PyIMouseWidget::_setMousePressEnabled,
                    py::arg("value"),
                    py::arg("button") = MouseButton::Left,
                    py::arg("modifiers") = -1)
                .def("_isMouseInside", &PyIMouseWidget::_isMouseInside)
                .def(
                    "_getMousePos",
                    &PyIMouseWidget::_getMousePos,
                    py::return_value_policy::copy)
                .def("_isMousePressed", &PyIMouseWidget::_isMousePressed)
                .def(
                    "_getMousePressPos",
                    &PyIMouseWidget::_getMousePressPos,
                    py::return_value_policy::copy);
        }
    }
}
