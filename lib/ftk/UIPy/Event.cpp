// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/Event.h>
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
        void event(nb::module_& m)
        {
            nb::class_<ChildAddEvent>(m, "ChildAddEvent")
                .def(nb::init<const std::shared_ptr<IWidget>&>())
                .def_rw("child", &ChildAddEvent::child);

            nb::class_<ChildRemoveEvent>(m, "ChildRemoveEvent")
                .def(nb::init<const std::shared_ptr<IWidget>&, int>())
                .def_rw("child", &ChildRemoveEvent::child)
                .def_rw("index", &ChildRemoveEvent::index);

            nb::class_<TickEvent>(m, "TickEvent");

            nb::class_<StyleEvent>(m, "StyleEvent")
                .def_rw("displayScaleChange", &StyleEvent::displayScaleChange)
                .def_rw("sizeRoleChange", &StyleEvent::sizeRoleChange)
                .def_rw("colorRoleChange", &StyleEvent::colorRoleChange)
                .def_rw("fontChange", &StyleEvent::fontChange);

            nb::class_<SizeHintEvent>(m, "SizeHintEvent")
                .def(nb::init<
                    const std::shared_ptr<FontSystem>&,
                    const std::shared_ptr<IconSystem>&,
                    float,
                    const std::shared_ptr<Style>&>())
                .def_rw("fontSystem", &SizeHintEvent::fontSystem)
                .def_rw("iconSystem", &SizeHintEvent::iconSystem)
                .def_rw("displayScale", &SizeHintEvent::displayScale)
                .def_rw("style", &SizeHintEvent::style);

            nb::class_<DrawEvent>(m, "DrawEvent")
                .def(nb::init<
                    const std::shared_ptr<FontSystem>&,
                    const std::shared_ptr<IconSystem>&,
                    float,
                    const std::shared_ptr<Style>&,
                    const std::shared_ptr<IRender>& >())
                .def_rw("fontSystem", &DrawEvent::fontSystem)
                .def_rw("iconSystem", &DrawEvent::iconSystem)
                .def_rw("displayScale", &DrawEvent::displayScale)
                .def_rw("style", &DrawEvent::style)
                .def_rw("render", &DrawEvent::render);

            nb::class_<IDragDropData>(m, "IDragDropData");

            nb::class_<MouseEnterEvent>(m, "MouseEnterEvent")
                .def(nb::init<const V2I&>())
                .def_rw("pos", &MouseEnterEvent::pos)
                .def_rw("accept", &MouseEnterEvent::accept);

            nb::class_<MouseMoveEvent>(m, "MouseMoveEvent")
                .def(nb::init<const V2I&, const V2I&>())
                .def_rw("pos", &MouseMoveEvent::pos)
                .def_rw("prev", &MouseMoveEvent::prev)
                .def_rw("accept", &MouseMoveEvent::accept)
                .def_rw("dragDropData", &MouseMoveEvent::dragDropData)
                .def_rw("dragDropCursor", &MouseMoveEvent::dragDropCursor)
                .def_rw("dragDropCursorHotspot", &MouseMoveEvent::dragDropCursorHotspot);

            FTK_ENUM_PY(m, MouseButton);
            FTK_ENUM_BIND(m, MouseButton);

            nb::enum_<KeyModifier>(m, "KeyModifier")
                .value("_None", KeyModifier::None)
                .value("Shift", KeyModifier::Shift)
                .value("Control", KeyModifier::Control)
                .value("Alt", KeyModifier::Alt)
                .value("Super", KeyModifier::Super);

            m.attr("commandKeyModifier") = commandKeyModifier;

            m.def("getKeyModifierLabel", &getKeyModifierLabel);
            m.def("to_string", nanobind::overload_cast<KeyModifier>(&to_string));

            nb::class_<MouseClickEvent>(m, "MouseClickEvent")
                .def(nb::init<>())
                .def(nb::init<MouseButton, int, const V2I&>())
                .def_rw("button", &MouseClickEvent::button)
                .def_rw("modifiers", &MouseClickEvent::modifiers)
                .def_rw("pos", &MouseClickEvent::pos)
                .def_rw("accept", &MouseClickEvent::accept);

            nb::class_<ScrollEvent>(m, "ScrollEvent")
                .def(nb::init<const V2F&, int, const V2I&>())
                .def_rw("value", &ScrollEvent::value)
                .def_rw("modifiers", &ScrollEvent::modifiers)
                .def_rw("pos", &ScrollEvent::pos)
                .def_rw("accept", &ScrollEvent::accept);

            FTK_ENUM_PY(m, Key);
            FTK_ENUM_BIND(m, Key);

            m.def(
                "getShortcutLabel",
                &getShortcutLabel,
                nb::arg("key"),
                nb::arg("modifiers"));

            nb::class_<KeyEvent>(m, "KeyEvent")
                .def(nb::init<>())
                .def(nb::init<Key, int, const V2I&>())
                .def_rw("key", &KeyEvent::key)
                .def_rw("modifiers", &KeyEvent::modifiers)
                .def_rw("pos", &KeyEvent::pos)
                .def_rw("accept", &KeyEvent::accept);

            nb::class_<TextEvent>(m, "TextEvent")
                .def(nb::init<const std::string&>())
                .def_rw("text", &TextEvent::text)
                .def_rw("accept", &TextEvent::accept);

            nb::class_<DragDropTextData, IDragDropData>(m, "DragDropTextData")
                .def(nb::init<const std::vector<std::string>&>())
                .def_prop_ro("text", &DragDropTextData::getText);

            nb::class_<DragDropEvent>(m, "DragDropEvent")
                .def(nb::init<
                    const V2I&,
                    const V2I&,
                    const std::shared_ptr<IDragDropData>&>())
                .def_rw("pos", &DragDropEvent::pos)
                .def_rw("prev", &DragDropEvent::prev)
                .def_rw("data", &DragDropEvent::data)
                .def_rw("accept", &DragDropEvent::accept);
        }
    }
}
