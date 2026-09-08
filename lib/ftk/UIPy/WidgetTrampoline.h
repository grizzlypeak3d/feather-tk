// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/trampoline.h>

//! An event that carries results back -- accept, and the drag and drop
//! fields on a mouse move -- is dispatched by hand: the NB_OVERRIDE
//! macro would copy a reference argument, so Python would write to a
//! copy and the window would never hear.
#define FTK_WIDGET_EVENT_REF(NAME, TYPE) \
    void NAME(TYPE& event) override \
    { \
        nanobind::detail::ticket nb_ticket( \
            nb_trampoline, #NAME, nanobind::detail::str_hash(#NAME), false); \
        if (nb_ticket.key.is_valid()) \
        { \
            nb_trampoline.base().attr(nb_ticket.key)(nanobind::cast( \
                &event, nanobind::rv_policy::reference)); \
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
        //! Construct a Python-owned instance of a trampolined class, for a
        //! custom __init__: placement-new the trampoline into nanobind's
        //! (alias-sized) storage, mark the instance constructed, and take a
        //! shared_ptr so enable_shared_from_this is armed before _init runs
        //! -- _init parents widgets and registers windows through
        //! shared_from_this. The trampoline requires Python to own the
        //! storage, so these classes cannot use the create() factories the
        //! way everything else does.
        //! The owner shared_ptr must outlive init: shared_from_this stops
        //! working the moment the last C++ shared_ptr goes away (see the
        //! nanobind ownership documentation), and _init is exactly where
        //! parenting needs it.
        template<typename Py, typename Base, typename F>
        void pyConstruct(Base* self, F&& init)
        {
            auto out = new (static_cast<void*>(self)) Py;
            nanobind::handle h = nanobind::find(self);
            nanobind::inst_mark_ready(h);
            auto owner = nanobind::cast<std::shared_ptr<Base> >(h);
            init(*out);
        }

        //! One set of overrides for every widget base Python can subclass,
        //! so IWidget, IContainer, and IWindow do not each keep their own
        //! copy.
        template<typename Base>
        class PyWidget : public Base
        {
        public:
            NB_TRAMPOLINE(Base);

            Size2I getSizeHint() const override
            {
                NB_OVERRIDE(getSizeHint);
            }

            void setGeometry(const Box2I& value) override
            {
                NB_OVERRIDE(setGeometry, value);
            }

            void tickEvent(
                bool parentsVisible,
                bool parentsEnabled,
                const TickEvent& event) override
            {
                NB_OVERRIDE(tickEvent, parentsVisible, parentsEnabled, event);
            }

            void styleEvent(const StyleEvent& event) override
            {
                NB_OVERRIDE(styleEvent, event);
            }

            void sizeHintEvent(const SizeHintEvent& event) override
            {
                NB_OVERRIDE(sizeHintEvent, event);
            }

            void drawEvent(
                const Box2I& drawRect,
                const DrawEvent& event) override
            {
                NB_OVERRIDE(drawEvent, drawRect, event);
            }

            void drawOverlayEvent(
                const Box2I& drawRect,
                const DrawEvent& event) override
            {
                NB_OVERRIDE(drawOverlayEvent, drawRect, event);
            }

            void mouseLeaveEvent() override
            {
                NB_OVERRIDE(mouseLeaveEvent);
            }

            void keyFocusEvent(bool value) override
            {
                NB_OVERRIDE(keyFocusEvent, value);
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
