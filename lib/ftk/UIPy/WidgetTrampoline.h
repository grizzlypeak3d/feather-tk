// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <pybind11/pybind11.h>

//! An event that carries results back -- accept, and the drag and drop
//! fields on a mouse move -- is dispatched by hand: the PYBIND11_OVERRIDE
//! macro copies a reference argument, so Python would write to a copy and
//! the window would never hear.
#define FTK_WIDGET_EVENT_REF(NAME, TYPE) \
    void NAME(TYPE& event) override \
    { \
        pybind11::gil_scoped_acquire gil; \
        /* Through the registered base, the way PYBIND11_OVERRIDE does: */ \
        /* the template instantiation itself is no type Python knows. */ \
        pybind11::function override = pybind11::get_override( \
            static_cast<const Base*>(this), #NAME); \
        if (override) \
        { \
            override(pybind11::cast( \
                &event, pybind11::return_value_policy::reference)); \
        } \
        else \
        { \
            Base::NAME(event); \
        } \
    }

namespace ftk
{
    namespace python
    {
        //! One set of overrides for every widget base Python can subclass,
        //! so IWidget, IContainer, and IMouseWidget do not each keep their
        //! own copy.
        template<typename Base>
        class PyWidget : public Base
        {
        public:
            Size2I getSizeHint() const override
            {
                PYBIND11_OVERRIDE(Size2I, Base, getSizeHint);
            }

            void setGeometry(const Box2I& value) override
            {
                PYBIND11_OVERRIDE(void, Base, setGeometry, value);
            }

            void tickEvent(
                bool parentsVisible,
                bool parentsEnabled,
                const TickEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    Base,
                    tickEvent,
                    parentsVisible,
                    parentsEnabled,
                    event);
            }

            void styleEvent(const StyleEvent& event) override
            {
                PYBIND11_OVERRIDE(void, Base, styleEvent, event);
            }

            void sizeHintEvent(const SizeHintEvent& event) override
            {
                PYBIND11_OVERRIDE(void, Base, sizeHintEvent, event);
            }

            void drawEvent(
                const Box2I& drawRect,
                const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(void, Base, drawEvent, drawRect, event);
            }

            void drawOverlayEvent(
                const Box2I& drawRect,
                const DrawEvent& event) override
            {
                PYBIND11_OVERRIDE(
                    void,
                    Base,
                    drawOverlayEvent,
                    drawRect,
                    event);
            }

            void mouseLeaveEvent() override
            {
                PYBIND11_OVERRIDE(void, Base, mouseLeaveEvent);
            }

            void keyFocusEvent(bool value) override
            {
                PYBIND11_OVERRIDE(void, Base, keyFocusEvent, value);
            }

            FTK_WIDGET_EVENT_REF(mouseEnterEvent, MouseEnterEvent)
            FTK_WIDGET_EVENT_REF(mouseMoveEvent, MouseMoveEvent)
            FTK_WIDGET_EVENT_REF(mousePressEvent, MouseClickEvent)
            FTK_WIDGET_EVENT_REF(mouseReleaseEvent, MouseClickEvent)
            FTK_WIDGET_EVENT_REF(scrollEvent, ScrollEvent)
            FTK_WIDGET_EVENT_REF(keyPressEvent, KeyEvent)
            FTK_WIDGET_EVENT_REF(keyReleaseEvent, KeyEvent)
            FTK_WIDGET_EVENT_REF(textEvent, TextEvent)
            FTK_WIDGET_EVENT_REF(textEditingEvent, TextEditingEvent)
            FTK_WIDGET_EVENT_REF(dragEnterEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(dragLeaveEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(dragMoveEvent, DragDropEvent)
            FTK_WIDGET_EVENT_REF(dropEvent, DragDropEvent)
        };
    }
}
