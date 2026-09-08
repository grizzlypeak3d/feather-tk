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

            nb::enum_<MouseButton>(m, "MouseButton")
                .value("_None", MouseButton::None)
                .value("Left", MouseButton::Left)
                .value("Middle", MouseButton::Middle)
                .value("Right", MouseButton::Right)
                .value("Extra1", MouseButton::Extra1)
                .value("Extra2", MouseButton::Extra2);
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

            nb::enum_<Key>(m, "Key")
                .value("Unknown", Key::Unknown)
                .value("Return", Key::Return)
                .value("Escape", Key::Escape)
                .value("Backspace", Key::Backspace)
                .value("Tab", Key::Tab)
                .value("Space", Key::Space)
                .value("Exclaim", Key::Exclaim)
                .value("DoubleQuote", Key::DoubleQuote)
                .value("Hash", Key::Hash)
                .value("Percent", Key::Percent)
                .value("Dollar", Key::Dollar)
                .value("Ampersand", Key::Ampersand)
                .value("SingleQuote", Key::SingleQuote)
                .value("LeftParen", Key::LeftParen)
                .value("RightParen", Key::RightParen)
                .value("Asterisk", Key::Asterisk)
                .value("Plus", Key::Plus)
                .value("Comma", Key::Comma)
                .value("Minus", Key::Minus)
                .value("Period", Key::Period)
                .value("Slash", Key::Slash)
                .value("_0", Key::_0)
                .value("_1", Key::_1)
                .value("_2", Key::_2)
                .value("_3", Key::_3)
                .value("_4", Key::_4)
                .value("_5", Key::_5)
                .value("_6", Key::_6)
                .value("_7", Key::_7)
                .value("_8", Key::_8)
                .value("_9", Key::_9)
                .value("Colon", Key::Colon)
                .value("Semicolon", Key::Semicolon)
                .value("Less", Key::Less)
                .value("Equals", Key::Equals)
                .value("Greater", Key::Greater)
                .value("Question", Key::Question)
                .value("At", Key::At)
                .value("LeftBracket", Key::LeftBracket)
                .value("Backslash", Key::Backslash)
                .value("RightBracket", Key::RightBracket)
                .value("Caret", Key::Caret)
                .value("Underscore", Key::Underscore)
                .value("BackQuote", Key::BackQuote)
                .value("A", Key::A)
                .value("B", Key::B)
                .value("C", Key::C)
                .value("D", Key::D)
                .value("E", Key::E)
                .value("F", Key::F)
                .value("G", Key::G)
                .value("H", Key::H)
                .value("I", Key::I)
                .value("J", Key::J)
                .value("K", Key::K)
                .value("L", Key::L)
                .value("M", Key::M)
                .value("N", Key::N)
                .value("O", Key::O)
                .value("P", Key::P)
                .value("Q", Key::Q)
                .value("R", Key::R)
                .value("S", Key::S)
                .value("T", Key::T)
                .value("U", Key::U)
                .value("V", Key::V)
                .value("W", Key::W)
                .value("X", Key::X)
                .value("Y", Key::Y)
                .value("Z", Key::Z)
                .value("CapsLock", Key::CapsLock)
                .value("F1", Key::F1)
                .value("F2", Key::F2)
                .value("F3", Key::F3)
                .value("F4", Key::F4)
                .value("F5", Key::F5)
                .value("F6", Key::F6)
                .value("F7", Key::F7)
                .value("F8", Key::F8)
                .value("F9", Key::F9)
                .value("F10", Key::F10)
                .value("F11", Key::F11)
                .value("F12", Key::F12)
                .value("F13", Key::F13)
                .value("F14", Key::F14)
                .value("F15", Key::F15)
                .value("F16", Key::F16)
                .value("F17", Key::F17)
                .value("F18", Key::F18)
                .value("F19", Key::F19)
                .value("F20", Key::F20)
                .value("F21", Key::F21)
                .value("F22", Key::F22)
                .value("F23", Key::F23)
                .value("F24", Key::F24)
                .value("PrintScreen", Key::PrintScreen)
                .value("ScrollLock", Key::ScrollLock)
                .value("Pause", Key::Pause)
                .value("Insert", Key::Insert)
                .value("Home", Key::Home)
                .value("PageUp", Key::PageUp)
                .value("Delete", Key::Delete)
                .value("End", Key::End)
                .value("PageDown", Key::PageDown)
                .value("Right", Key::Right)
                .value("Left", Key::Left)
                .value("Down", Key::Down)
                .value("Up", Key::Up)
                .value("NumLock", Key::NumLock)
                .value("KeypadDivide", Key::KeypadDivide)
                .value("KeypadMulitply", Key::KeypadMultiply)
                .value("KeypadMinus", Key::KeypadMinus)
                .value("KeypadPlus", Key::KeypadPlus)
                .value("Keypad_1", Key::Keypad_1)
                .value("Keypad_2", Key::Keypad_2)
                .value("Keypad_3", Key::Keypad_3)
                .value("Keypad_4", Key::Keypad_4)
                .value("Keypad_5", Key::Keypad_5)
                .value("Keypad_6", Key::Keypad_6)
                .value("Keypad_7", Key::Keypad_7)
                .value("Keypad_8", Key::Keypad_8)
                .value("Keypad_9", Key::Keypad_9)
                .value("Keypad_0", Key::Keypad_0)
                .value("KeypadPeriod", Key::KeypadPeriod)
                .value("KeypadEquals", Key::KeypadEquals);
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
