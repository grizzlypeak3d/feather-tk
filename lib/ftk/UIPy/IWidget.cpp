// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UIPy/WidgetTrampoline.h>

#include <ftk/UI/IWidget.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/Menu.h>

#include <nanobind/stl/function.h>
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
        class PyIWidget : public PyWidget<IWidget>
        {
        public:
            void pyInit(
                const std::shared_ptr<Context>& context,
                const std::string& objectName,
                const std::shared_ptr<IWidget>& parent)
            {
                _init(context, objectName, parent);
            }
        };

        void iWidget(nb::module_& m)
        {
            nb::class_<IWidget, PyIWidget>(m, "IWidget")
                .def(
                    "__init__",
                    [](IWidget* self,
                       const std::shared_ptr<Context>& context,
                       const std::string& objectName,
                       const std::shared_ptr<IWidget>& parent)
                    {
                        pyConstruct<PyIWidget>(self,
                            [&](PyIWidget& w)
                            {
                                w.pyInit(context, objectName, parent);
                            });
                    },
                    nb::arg("context"),
                    nb::arg("objectName"),
                    nb::arg("parent") = nullptr)

                .def_prop_ro("context", &IWidget::getContext)

                .def_prop_rw("objectName", &IWidget::getObjectName, &IWidget::setObjectName)
                .def_prop_ro(
                    "objectPath",
                    // These take self as a shared_ptr: they reach
                    // shared_from_this(), which only works while a C++
                    // shared_ptr for the instance exists.
                    [](const std::shared_ptr<IWidget>& self)
                    {
                        return self->getObjectPath();
                    })
                .def_prop_rw("backgroundRole", &IWidget::getBackgroundRole, &IWidget::setBackgroundRole)

                .def_prop_rw(
                    "parent",
                    &IWidget::getParent,
                    [](const std::shared_ptr<IWidget>& self,
                       const std::shared_ptr<IWidget>& value)
                    {
                        self->setParent(value);
                    },
                    // None unparents.
                    nb::for_setter(nb::arg("value").none()))
                .def("getChildren", &IWidget::getChildren)
                .def("moveToFront", &IWidget::moveToFront)
                .def("moveToBack", &IWidget::moveToBack)
                .def_prop_ro(
                    "window",
                    [](const std::shared_ptr<IWidget>& self)
                    {
                        return self->getWindow();
                    })

                .def("getSizeHint", &IWidget::getSizeHint)
                .def_prop_rw("hStretch", &IWidget::getHStretch, &IWidget::setHStretch)
                .def_prop_rw("vStretch", &IWidget::getVStretch, &IWidget::setVStretch)
                .def(
                    "setStretch",
                    nb::overload_cast<Stretch, Stretch>(&IWidget::setStretch),
                    nb::arg("horizontal"),
                    nb::arg("vertical"))
                .def(
                    "setStretch",
                    nb::overload_cast<Stretch>(&IWidget::setStretch))
                .def_prop_rw("hAlign", &IWidget::getHAlign, &IWidget::setHAlign)
                .def_prop_rw("vAlign", &IWidget::getVAlign, &IWidget::setVAlign)
                .def(
                    "setAlign",
                    &IWidget::setAlign,
                    nb::arg("horizontal"),
                    nb::arg("vertical"))
                .def("setGeometry", &IWidget::setGeometry)
                .def_prop_ro("geometry", &IWidget::getGeometry, nb::rv_policy::copy)
                .def("setPos", &IWidget::setPos)
                .def("setSize", &IWidget::setSize)

                .def(
                    "isVisible",
                    &IWidget::isVisible,
                    nb::arg("andParentsVisible") = true)
                .def("setVisible", &IWidget::setVisible)
                .def("show", &IWidget::show)
                .def("hide", &IWidget::hide)
                .def_prop_ro("clipped", &IWidget::isClipped)
                .def_prop_rw("clipChildren", &IWidget::doesClipChildren, &IWidget::setClipChildren)
                .def_prop_ro("childrenClipRect", &IWidget::getChildrenClipRect)

                .def_prop_rw(
                    "enabled",
                    // The getter has a defaulted argument, which a bound
                    // property cannot fill in.
                    [](const IWidget& widget)
                    {
                        return widget.isEnabled();
                    },
                    &IWidget::setEnabled)
                .def(
                    "isEnabled",
                    &IWidget::isEnabled,
                    nb::arg("andParentsEnabled") = true)

                .def_prop_rw("acceptsKeyFocus", &IWidget::acceptsKeyFocus, &IWidget::setAcceptsKeyFocus)
                .def("moveToIndex", &IWidget::moveToIndex)
                .def_prop_ro("keyFocus", &IWidget::hasKeyFocus)
                .def(
                    "takeKeyFocus",
                    [](const std::shared_ptr<IWidget>& self)
                    {
                        self->takeKeyFocus();
                    })
                .def("releaseKeyFocus", &IWidget::releaseKeyFocus)

                .def_prop_rw("tooltip", &IWidget::getTooltip, &IWidget::setTooltip)
                .def(
                    "setContextMenuCallback",
                    &IWidget::setContextMenuCallback)

                .def("childAddEvent", &IWidget::childAddEvent, nb::arg("event"))
                .def("childRemoveEvent", &IWidget::childRemoveEvent, nb::arg("event"))
                .def(
                    "tickEvent",
                    &IWidget::tickEvent,
                    nb::arg("parentsVisible"),
                    nb::arg("parentsEnabled"),
                    nb::arg("event"))
                .def("styleEvent", &IWidget::styleEvent, nb::arg("event"))
                .def("sizeHintEvent", &IWidget::sizeHintEvent, nb::arg("event"))
                .def(
                    "clipEvent",
                    &IWidget::clipEvent,
                    nb::arg("clipRect"),
                    nb::arg("clipped"))
                .def(
                    "drawEvent",
                    &IWidget::drawEvent,
                    nb::arg("drawRect"),
                    nb::arg("event"))
                .def(
                    "drawOverlayEvent",
                    &IWidget::drawOverlayEvent,
                    nb::arg("drawRect"),
                    nb::arg("event"))
                .def("mouseEnterEvent", &IWidget::mouseEnterEvent)
                .def("mouseLeaveEvent", &IWidget::mouseLeaveEvent)
                .def("mouseMoveEvent", &IWidget::mouseMoveEvent, nb::arg("event"))
                .def("mousePressEvent", &IWidget::mousePressEvent, nb::arg("event"))
                .def("mouseReleaseEvent", &IWidget::mouseReleaseEvent, nb::arg("event"))
                .def("scrollEvent", &IWidget::scrollEvent, nb::arg("event"))
                .def("keyFocusEvent", &IWidget::keyFocusEvent, nb::arg("focus"))
                .def("keyPressEvent", &IWidget::keyPressEvent, nb::arg("event"))
                .def("keyReleaseEvent", &IWidget::keyReleaseEvent, nb::arg("event"))
                .def("textEvent", &IWidget::textEvent, nb::arg("event"))
                .def(
                    "setSizeUpdate",
                    &IWidget::setSizeUpdate,
                    nb::arg("value") = true)
                .def(
                    "setDrawUpdate",
                    &IWidget::setDrawUpdate,
                    nb::arg("value") = true)
                .def("dragEnterEvent", &IWidget::dragEnterEvent, nb::arg("event"))
                .def("dragLeaveEvent", &IWidget::dragLeaveEvent, nb::arg("event"))
                .def("dragMoveEvent", &IWidget::dragMoveEvent, nb::arg("event"))
                .def("dropEvent", &IWidget::dropEvent, nb::arg("event"));
        }
    }
}
