// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IconSystem.h>
#include <ftk/UI/Style.h>

#include <ftk/Core/FontSystem.h>
#include <ftk/Core/IRender.h>

namespace ftk
{
    class IWidget;

    //! \name Events
    ///@{
        
    //! Child added event.
    struct FTK_API_TYPE ChildAddEvent
    {
        FTK_API ChildAddEvent(const std::shared_ptr<IWidget>&);

        std::shared_ptr<IWidget> child;
    };

    //! Child removed event.
    struct FTK_API_TYPE ChildRemoveEvent
    {
        FTK_API ChildRemoveEvent(const std::shared_ptr<IWidget>&, int index);

        std::shared_ptr<IWidget> child;
        int index = 0;
    };

    //! Tick event.
    struct FTK_API_TYPE TickEvent
    {
    };

    //! Size hint event.
    struct FTK_API_TYPE SizeHintEvent
    {
        FTK_API SizeHintEvent(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            float displayScale,
            const std::shared_ptr<Style>&);

        std::shared_ptr<FontSystem> fontSystem;
        std::shared_ptr<IconSystem> iconSystem;
        float                       displayScale = 1.F;
        std::shared_ptr<Style>      style;
    };

    //! Draw event.
    struct FTK_API_TYPE DrawEvent
    {
        FTK_API DrawEvent(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            float displayScale,
            const std::shared_ptr<Style>&,
            const std::shared_ptr<IRender>&);

        std::shared_ptr<FontSystem> fontSystem;
        std::shared_ptr<IconSystem> iconSystem;
        float                       displayScale = 1.F;
        std::shared_ptr<Style>      style;
        std::shared_ptr<IRender>    render;
    };

    //! Drag and drop data.
    class FTK_API_TYPE DragAndDropData : public std::enable_shared_from_this<DragAndDropData>
    {
    public:
        FTK_API virtual ~DragAndDropData() = 0;
    };

    //! Mouse enter event.
    struct FTK_API_TYPE MouseEnterEvent
    {
        FTK_API MouseEnterEvent(const V2I& pos);

        bool accept = false;
        V2I  pos;
    };

    //! Mouse move event.
    struct FTK_API_TYPE MouseMoveEvent
    {
        FTK_API MouseMoveEvent(
            const V2I& pos,
            const V2I& prev);

        V2I                              pos;
        V2I                              prev;
        bool                             accept = false;
        std::shared_ptr<DragAndDropData> dndData;
        std::shared_ptr<Image>           dndCursor;
        V2I                              dndCursorHotspot;
    };

    //! Keyboard modifiers.
    enum class FTK_API_TYPE KeyModifier
    {
        None    = 0,
        Shift   = 1,
        Control = 2,
        Alt     = 4,
        Super   = 8
    };

    //! Convert to a string.
    FTK_API std::string to_string(KeyModifier);

    //! Convert from a string.
    FTK_API bool from_string(const std::string&, KeyModifier&);

    //! Check for the given key modifier.
    FTK_API bool checkKeyModifier(KeyModifier, int modifiers);

    //! OS specific command key modifier.
#if defined(__APPLE__)
    const KeyModifier commandKeyModifier = KeyModifier::Super;
#else // __APPLE__
    const KeyModifier commandKeyModifier = KeyModifier::Control;
#endif // __APPLE__

    //! Get a keyboard modifiers label.
    FTK_API std::string getKeyModifierLabel(int);

    //! Mouse click event.
    struct FTK_API_TYPE MouseClickEvent
    {
        FTK_API MouseClickEvent() = default;
        FTK_API MouseClickEvent(
            int              button,
            int              modifiers,
            const V2I& pos);

        int  button    = 0;
        int  modifiers = 0;
        V2I  pos;
        bool accept    = false;
    };

    //! Scroll event (mouse wheel or touch pad).
    struct FTK_API_TYPE ScrollEvent
    {
        FTK_API ScrollEvent(
            const V2F& value,
            int              modifiers,
            const V2I& pos);

        V2F  value;
        int  modifiers = 0;
        V2I  pos;
        bool accept    = false;
    };

    //! Keys.
    enum class FTK_API_TYPE Key
    {
        Unknown,

        Return,
        Escape,
        Backspace,
        Tab,
        Space,
        Exclaim,
        DoubleQuote,
        Hash,
        Percent,
        Dollar,
        Ampersand,
        SingleQuote,
        LeftParen,
        RightParen,
        Asterisk,
        Plus,
        Comma,
        Minus,
        Period,
        Slash,
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        Colon,
        Semicolon,
        Less,
        Equals,
        Greater,
        Question,
        At,
        LeftBracket,
        Backslash,
        RightBracket,
        Caret,
        Underscore,
        BackQuote,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        CapsLock,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        PrintScreen,
        ScrollLock,
        Pause,
        Insert,
        Home,
        PageUp,
        Delete,
        End,
        PageDown,
        Right,
        Left,
        Down,
        Up,
        NumLock,
        KeypadDivide,
        KeypadMulitply,
        KeypadMinus,
        KeypadPlus,
        KeypadEnter,
        Keypad_1,
        Keypad_2,
        Keypad_3,
        Keypad_4,
        Keypad_5,
        Keypad_6,
        Keypad_7,
        Keypad_8,
        Keypad_9,
        Keypad_0,
        KeypadPeriod,
        KeypadEquals,

        Count,
        First = Unknown
    };
    FTK_ENUM(Key);

    //! Get a keyboard shortcut label.
    FTK_API std::string getShortcutLabel(Key, int modifiers = 0);

    //! Key event.
    struct FTK_API_TYPE KeyEvent
    {
        FTK_API KeyEvent() = default;
        FTK_API KeyEvent(
            Key        key,
            int        modifiers,
            const V2I& pos);

        Key  key       = Key::Unknown;
        int  modifiers = 0;
        V2I  pos;
        bool accept    = false;
    };

    //! Text event.
    struct FTK_API_TYPE TextEvent
    {
        FTK_API TextEvent(const std::string& text);

        std::string text;
        bool        accept = false;
    };

    //! Drag and drop text data.
    class FTK_API_TYPE TextDragAndDropData : public DragAndDropData
    {
    public:
        FTK_API TextDragAndDropData(const std::string& text);

        FTK_API virtual ~TextDragAndDropData();

        FTK_API const std::string& getText() const;

    private:
        std::string _text;
    };

    //! Drag and drop event.
    struct FTK_API_TYPE DragAndDropEvent
    {
        FTK_API DragAndDropEvent(
            const V2I&                              pos,
            const V2I&                              prev,
            const std::shared_ptr<DragAndDropData>& data);

        V2I                              pos;
        V2I                              prev;
        std::shared_ptr<DragAndDropData> data;
        bool                             accept = false;
    };
        
    ///@}
}
