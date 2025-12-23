// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Event.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    ChildAddEvent::ChildAddEvent(const std::shared_ptr<IWidget>& child) :
        child(child)
    {}

    ChildRemoveEvent::ChildRemoveEvent(const std::shared_ptr<IWidget>& child, int index) :
        child(child),
        index(index)
    {}

    SizeHintEvent::SizeHintEvent(
        const std::shared_ptr<FontSystem>& fontSystem,
        const std::shared_ptr<IconSystem>& iconSystem,
        float                              displayScale,
        const std::shared_ptr<Style>&      style) :
        fontSystem(fontSystem),
        iconSystem(iconSystem),
        displayScale(displayScale),
        style(style)
    {}

    DrawEvent::DrawEvent(
        const std::shared_ptr<FontSystem>& fontSystem,
        const std::shared_ptr<IconSystem>& iconSystem,
        float                              displayScale,
        const std::shared_ptr<Style>&      style,
        const std::shared_ptr<IRender>&    render) :
        fontSystem(fontSystem),
        iconSystem(iconSystem),
        displayScale(displayScale),
        style(style),
        render(render)
    {}

    IDragDropData::~IDragDropData()
    {}

    MouseEnterEvent::MouseEnterEvent(const V2I & pos) :
        pos(pos)
    {}

    MouseMoveEvent::MouseMoveEvent(
        const V2I & pos,
        const V2I & prev) :
        pos(pos),
        prev(prev)
    {}

    FTK_ENUM_IMPL(
        MouseButton,
        "None",
        "Left",
        "Middle",
        "Right",
        "Extra 1",
        "Extra 2");

    MouseClickEvent::MouseClickEvent(
        MouseButton button,
        int         modifiers,
        const V2I&  pos) :
        button(button),
        modifiers(modifiers),
        pos(pos)
    {}

    ScrollEvent::ScrollEvent(
        const V2F& value,
        int        modifiers,
        const V2I& pos) :
        value(value),
        modifiers(modifiers),
        pos(pos)
    {}

    namespace
    {
        std::map<KeyModifier, std::string> keyModifierLabels =
        {
            { KeyModifier::None, "None" },
            { KeyModifier::Shift, "Shift" },
            { KeyModifier::Control, "Ctrl" },
            { KeyModifier::Alt, "Alt" },
            { KeyModifier::Super, "Cmd" }
        };
    }

    std::string to_string(KeyModifier value)
    {
        return keyModifierLabels[value];
    }

    bool from_string(const std::string& s, KeyModifier& value)
    {
        bool out = false;
        for (auto i = keyModifierLabels.begin(); i != keyModifierLabels.end(); ++i)
        {
            if (compare(s, i->second, CaseCompare::Insensitive))
            {
                value = i->first;
                out = true;
                break;
            }
        }
        return out;
    }

    bool checkKeyModifier(KeyModifier modifier, int modifiers)
    {
        return (KeyModifier::None == modifier && 0 == modifiers) ?
            true :
            (modifiers & static_cast<int>(modifier));
    }

    std::string getKeyModifierLabel(int value)
    {
        std::vector<std::string> out;
        if (value & static_cast<size_t>(KeyModifier::Shift))
        {
            out.push_back(to_string(KeyModifier::Shift));
        }
        if (value & static_cast<size_t>(KeyModifier::Control))
        {
            out.push_back(to_string(KeyModifier::Control));
        }
        if (value & static_cast<size_t>(KeyModifier::Alt))
        {
            out.push_back(to_string(KeyModifier::Alt));
        }
        if (value & static_cast<size_t>(KeyModifier::Super))
        {
            out.push_back(to_string(KeyModifier::Super));
        }
        return join(out, '+');
    }

    FTK_ENUM_IMPL(
        Key,
        "Unknown",
        "Return",
        "Escape",
        "Backspace",
        "Tab",
        "Space",
        "Exclaim",
        "Double Quote",
        "Hash",
        "Percent",
        "Dollar",
        "Ampersand",
        "Single Quote",
        "Left Paren",
        "Right Paren",
        "Asterisk",
        "Plus",
        "Comma",
        "Minus",
        "Period",
        "Slash",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "Colon",
        "Semicolon",
        "Less",
        "Equals",
        "Greater",
        "Question",
        "At",
        "Left Bracket",
        "Backslash",
        "Right Bracket",
        "Caret",
        "Underscore",
        "Back Quote",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "Caps Lock",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "Print Screen",
        "Scroll Lock",
        "Pause",
        "Insert",
        "Home",
        "Page Up",
        "Delete",
        "End",
        "Page Down",
        "Right",
        "Left",
        "Down",
        "Up",
        "Num Lock",
        "Keypad Divide",
        "Keypad Mulitply",
        "Keypad Minus",
        "Keypad Plus",
        "Keypad Enter",
        "Keypad 1",
        "Keypad 2",
        "Keypad 3",
        "Keypad 4",
        "Keypad 5",
        "Keypad 6",
        "Keypad 7",
        "Keypad 8",
        "Keypad 9",
        "Keypad 0",
        "Keypad Period",
        "Keypad Equals");

    bool isControlKey(Key key)
    {
        bool out = false;
        switch (key)
        {
        case Key::Return:
        case Key::Escape:
        case Key::Backspace:
        case Key::CapsLock:
        case Key::F1:
        case Key::F2:
        case Key::F3:
        case Key::F4:
        case Key::F5:
        case Key::F6:
        case Key::F7:
        case Key::F8:
        case Key::F9:
        case Key::F10:
        case Key::F11:
        case Key::F12:
        case Key::F13:
        case Key::F14:
        case Key::F15:
        case Key::F16:
        case Key::F17:
        case Key::F18:
        case Key::F19:
        case Key::F20:
        case Key::F21:
        case Key::F22:
        case Key::F23:
        case Key::F24:
        case Key::PrintScreen:
        case Key::ScrollLock:
        case Key::Pause:
        case Key::Insert:
        case Key::Home:
        case Key::PageUp:
        case Key::Delete:
        case Key::End:
        case Key::PageDown:
        case Key::Right:
        case Key::Left:
        case Key::Down:
        case Key::Up:
        case Key::NumLock:
        case Key::KeypadEnter:
            out = true;
            break;
        default: break;
        }
        return out;
    }

    std::string getShortcutLabel(Key key, int modifiers)
    {
        std::stringstream ss;
        if (key != Key::Unknown)
        {
            if (modifiers)
            {
                ss << getKeyModifierLabel(modifiers);
                ss << "+";
            }
            ss << key;
        }
        return ss.str();
    }

    KeyEvent::KeyEvent(
        Key        key,
        int        modifiers,
        const V2I& pos) :
        key(key),
        modifiers(modifiers),
        pos(pos)
    {}

    TextEvent::TextEvent(const std::string& text) :
        text(text)
    {}

    DragDropTextData::DragDropTextData(const std::vector<std::string>& text) :
        _text(text)
    {}

    DragDropTextData::~DragDropTextData()
    {}

    const std::vector<std::string>& DragDropTextData::getText() const
    {
        return _text;
    }

    DragDropEvent::DragDropEvent(
        const V2I& pos,
        const V2I& prev,
        const std::shared_ptr<IDragDropData>& data) :
        pos(pos),
        prev(prev),
        data(data)
    {}
}
