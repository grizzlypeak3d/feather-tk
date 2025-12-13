// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Event.h>

#include <ftk/CorePy/Util.h>

#include <ftk/UI/Event.h>
#include <ftk/UI/IWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void event(py::module_& m)
        {
            py::class_<ChildAddEvent>(m, "ChildAddEvent")
                .def(py::init<const std::shared_ptr<IWidget>&>())
                .def_readwrite("child", &ChildAddEvent::child);

            py::class_<ChildRemoveEvent>(m, "ChildRemoveEvent")
                .def(py::init<const std::shared_ptr<IWidget>&, int>())
                .def_readwrite("child", &ChildRemoveEvent::child)
                .def_readwrite("index", &ChildRemoveEvent::index);

            py::class_<TickEvent>(m, "TickEvent");

            py::class_<SizeHintEvent>(m, "SizeHintEvent")
                .def(py::init<
                    const std::shared_ptr<FontSystem>&,
                    const std::shared_ptr<IconSystem>&,
                    float,
                    const std::shared_ptr<Style>&>())
                .def_readwrite("fontSystem", &SizeHintEvent::fontSystem)
                .def_readwrite("iconSystem", &SizeHintEvent::iconSystem)
                .def_readwrite("displayScale", &SizeHintEvent::displayScale)
                .def_readwrite("style", &SizeHintEvent::style);

            py::class_<DrawEvent>(m, "DrawEvent")
                .def(py::init<
                    const std::shared_ptr<FontSystem>&,
                    const std::shared_ptr<IconSystem>&,
                    float,
                    const std::shared_ptr<Style>&,
                    const std::shared_ptr<IRender>& >())
                .def_readwrite("fontSystem", &DrawEvent::fontSystem)
                .def_readwrite("iconSystem", &DrawEvent::iconSystem)
                .def_readwrite("displayScale", &DrawEvent::displayScale)
                .def_readwrite("style", &DrawEvent::style)
                .def_readwrite("render", &DrawEvent::render);

            py::class_<IDragDropData, std::shared_ptr<IDragDropData> >(m, "IDragDropData");

            py::class_<MouseEnterEvent>(m, "MouseEnterEvent")
                .def(py::init<const V2I&>())
                .def_readwrite("pos", &MouseEnterEvent::pos)
                .def_readwrite("accept", &MouseEnterEvent::accept);

            py::class_<MouseMoveEvent>(m, "MouseMoveEvent")
                .def(py::init<const V2I&, const V2I&>())
                .def_readwrite("pos", &MouseMoveEvent::pos)
                .def_readwrite("prev", &MouseMoveEvent::prev)
                .def_readwrite("accept", &MouseMoveEvent::accept)
                .def_readwrite("dragDropData", &MouseMoveEvent::dragDropData)
                .def_readwrite("dragDropCursor", &MouseMoveEvent::dragDropCursor)
                .def_readwrite("dragDropCursorHotspot", &MouseMoveEvent::dragDropCursorHotspot);

            py::enum_<KeyModifier>(m, "KeyModifier")
                .value("_None", KeyModifier::None)
                .value("Shift", KeyModifier::Shift)
                .value("Control", KeyModifier::Control)
                .value("Alt", KeyModifier::Alt)
                .value("Super", KeyModifier::Super);

            m.attr("commandKeyModifier") = commandKeyModifier;

            m.def("getKeyModifierLabel", &getKeyModifierLabel);

            py::class_<MouseClickEvent>(m, "MouseClickEvent")
                .def(py::init<>())
                .def(py::init<int, int, const V2I&>())
                .def_readwrite("button", &MouseClickEvent::button)
                .def_readwrite("modifiers", &MouseClickEvent::modifiers)
                .def_readwrite("pos", &MouseClickEvent::pos)
                .def_readwrite("accept", &MouseClickEvent::accept);

            py::class_<ScrollEvent>(m, "ScrollEvent")
                .def(py::init<const V2F&, int, const V2I&>())
                .def_readwrite("value", &ScrollEvent::value)
                .def_readwrite("modifiers", &ScrollEvent::modifiers)
                .def_readwrite("pos", &ScrollEvent::pos)
                .def_readwrite("accept", &ScrollEvent::accept);

            py::enum_<Key>(m, "Key")
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
                .value("KeypadMulitply", Key::KeypadMulitply)
                .value("KeypadMinus", Key::KeypadMinus)
                .value("KeypadPlus", Key::KeypadPlus)
                .value("KeypadEnter", Key::KeypadEnter)
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
                py::arg("key"),
                py::arg("modifiers"));

            py::class_<KeyEvent>(m, "KeyEvent")
                .def(py::init<>())
                .def(py::init<Key, int, const V2I&>())
                .def_readwrite("key", &KeyEvent::key)
                .def_readwrite("modifiers", &KeyEvent::modifiers)
                .def_readwrite("pos", &KeyEvent::pos)
                .def_readwrite("accept", &KeyEvent::accept);

            py::class_<TextEvent>(m, "TextEvent")
                .def(py::init<const std::string&>())
                .def_readwrite("text", &TextEvent::text)
                .def_readwrite("accept", &TextEvent::accept);

            py::class_<DragDropTextData, IDragDropData, std::shared_ptr<DragDropTextData> >(m, "DragDropTextData")
                .def(py::init<const std::vector<std::string>&>())
                .def_property_readonly("text", &DragDropTextData::getText);

            py::class_<DragDropEvent>(m, "DragDropEvent")
                .def(py::init<
                    const V2I&,
                    const V2I&,
                    const std::shared_ptr<IDragDropData>&>())
                .def_readwrite("pos", &DragDropEvent::pos)
                .def_readwrite("prev", &DragDropEvent::prev)
                .def_readwrite("data", &DragDropEvent::data)
                .def_readwrite("accept", &DragDropEvent::accept);
        }
    }
}
