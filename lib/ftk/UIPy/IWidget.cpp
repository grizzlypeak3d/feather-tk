// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IWidget.h>
#include <ftk/UI/IWindow.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        class PyIWidget : public IWidget
        {
        public:
            static std::shared_ptr<PyIWidget> create(
                const std::shared_ptr<Context>& context,
                const std::string& objectName,
                const std::shared_ptr<IWidget>& parent = nullptr)
            {
                auto out = std::shared_ptr<PyIWidget>(new PyIWidget);
                out->_init(context, objectName, parent);
                return out;
            }
            
            Size2I getSizeHint() const override
            {
                PYBIND11_OVERRIDE(
                    Size2I,
                    IWidget,
                    getSizeHint);
            }

            void setGeometry(const Box2I& value) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IWidget,
                    setGeometry,
                    value);
            }
            
            void tickEvent(
                bool parentsVisible,
                bool parentsEnabled,
                const TickEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IWidget,
                    tickEvent,
                    parentsVisible,
                    parentsEnabled,
                    event);
            }
            
            void sizeHintEvent(const SizeHintEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IWidget,
                    sizeHintEvent,
                    event);
            }
            
            void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    IWidget,
                    drawEvent,
                    drawRect,
                    event);
            }
        };

        void iWidget(py::module_& m)
        {
            py::class_<IWidget, std::shared_ptr<IWidget>, PyIWidget>(m, "IWidget")
                .def(
                    py::init(&PyIWidget::create),
                    py::arg("context"),
                    py::arg("objectName"),
                    py::arg("parent") = nullptr)

                .def_property_readonly("context", &IWidget::getContext)

                .def_property("objectName", &IWidget::getObjectName, &IWidget::setObjectName)
                .def_property_readonly("objectPath", &IWidget::getObjectPath)
                .def_property("backgroundColor", &IWidget::getBackgroundRole, &IWidget::setBackgroundRole)

                .def_property("parent", &IWidget::getParent, &IWidget::setParent)
                .def("getChildren", &IWidget::getChildren)
                .def("moveToFront", &IWidget::moveToFront)
                .def("moveToBack", &IWidget::moveToBack)
                .def_property_readonly("window", &IWidget::getWindow)

                .def("getSizeHint", &IWidget::getSizeHint)
                .def_property("hStretch", &IWidget::getHStretch, &IWidget::setHStretch)
                .def_property("vStretch", &IWidget::getVStretch, &IWidget::setVStretch)
                .def(
                    "setStretch",
                    py::overload_cast<Stretch, Stretch>(&IWidget::setStretch),
                    py::arg("horizontal"),
                    py::arg("vertical"))
                .def(
                    "setStretch",
                    py::overload_cast<Stretch>(&IWidget::setStretch))
                .def_property("hAlign", &IWidget::getHAlign, &IWidget::setHAlign)
                .def_property("vAlign", &IWidget::getVAlign, &IWidget::setHAlign)
                .def(
                    "setAlign",
                    &IWidget::setAlign,
                    py::arg("horizontal"),
                    py::arg("vertical"))
                .def("setGeometry", &IWidget::setGeometry)
                .def_property_readonly("geometry", &IWidget::getGeometry)
                .def("setPos", &IWidget::setPos)
                .def("setSize", &IWidget::setSize)

                .def(
                    "isVisible",
                    &IWidget::isVisible,
                    py::arg("andParentsVisible") = true)
                .def("setVisible", &IWidget::setVisible)
                .def("show", &IWidget::show)
                .def("hide", &IWidget::hide)
                .def_property_readonly("clipped", &IWidget::isClipped)
                .def_property_readonly("childrenClipRect", &IWidget::getChildrenClipRect)

                .def_property("enabled", &IWidget::isEnabled, &IWidget::setEnabled)

                .def_property("acceptsKeyFocus", &IWidget::acceptsKeyFocus, &IWidget::setAcceptsKeyFocus)
                .def_property_readonly("keyFocus", &IWidget::hasKeyFocus)
                .def("takeKeyFocus", &IWidget::takeKeyFocus)
                .def("releaseKeyFocus", &IWidget::releaseKeyFocus)

                .def_property("tooltip", &IWidget::getTooltip, &IWidget::setTooltip)

                .def("childAddEvent", &IWidget::childAddEvent, py::arg("event"))
                .def("childRemoveEvent", &IWidget::childRemoveEvent, py::arg("event"))
                .def(
                    "tickEvent",
                    &IWidget::tickEvent,
                    py::arg("parentsVisible"),
                    py::arg("parentsEnabled"),
                    py::arg("event"))
                .def("sizeHintEvent", &IWidget::sizeHintEvent, py::arg("event"))
                .def(
                    "clipEvent",
                    &IWidget::clipEvent,
                    py::arg("clipRect"),
                    py::arg("clipped"))
                .def(
                    "drawEvent",
                    &IWidget::drawEvent,
                    py::arg("drawRect"),
                    py::arg("event"))
                .def(
                    "drawOverlayEvent",
                    &IWidget::drawOverlayEvent,
                    py::arg("drawRect"),
                    py::arg("event"))
                .def("mouseEnterEvent", &IWidget::mouseEnterEvent)
                .def("mouseLeaveEvent", &IWidget::mouseLeaveEvent)
                .def("mouseMoveEvent", &IWidget::mouseMoveEvent, py::arg("event"))
                .def("mousePressEvent", &IWidget::mousePressEvent, py::arg("event"))
                .def("mouseReleaseEvent", &IWidget::mouseReleaseEvent, py::arg("event"))
                .def("scrollEvent", &IWidget::scrollEvent, py::arg("event"))
                .def("keyFocusEvent", &IWidget::keyFocusEvent, py::arg("focus"))
                .def("keyPressEvent", &IWidget::keyPressEvent, py::arg("event"))
                .def("keyReleaseEvent", &IWidget::keyReleaseEvent, py::arg("event"))
                .def("textEvent", &IWidget::textEvent, py::arg("event"))
                .def("dragEnterEvent", &IWidget::dragEnterEvent, py::arg("event"))
                .def("dragLeaveEvent", &IWidget::dragLeaveEvent, py::arg("event"))
                .def("dragMoveEvent", &IWidget::dragMoveEvent, py::arg("event"))
                .def("dropEvent", &IWidget::dropEvent, py::arg("event"));
        }
    }
}
