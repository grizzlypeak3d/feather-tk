// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
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
    struct FTK_UI_API_TYPE ChildAddEvent
    {
        FTK_UI_API ChildAddEvent(const std::shared_ptr<IWidget>&);

        //! The child that was added, now the last of the parent's children.
        std::shared_ptr<IWidget> child;
    };

    //! Child removed event.
    struct FTK_UI_API_TYPE ChildRemoveEvent
    {
        FTK_UI_API ChildRemoveEvent(const std::shared_ptr<IWidget>&, int index);

        //! The child that was removed.
        std::shared_ptr<IWidget> child;
        //! The index the child had among the parent's children.
        int index = 0;
    };

    //! Tick event.
    struct FTK_UI_API_TYPE TickEvent
    {
    };

    //! Style event.
    struct FTK_UI_API_TYPE StyleEvent
    {
        //! @param changes Set all of the fields to this value.
        FTK_UI_API StyleEvent(bool changes = false);

        //! Whether the display scale has changed.
        bool displayScaleChange = false;
        //! Whether the style's size roles have changed.
        bool sizeRoleChange     = false;
        //! Whether the style's color roles have changed.
        bool colorRoleChange    = false;
        //! Whether the style's fonts have changed.
        bool fontChange         = false;

        FTK_UI_API bool hasChanges() const;
    };

    //! Size hint event.
    struct FTK_UI_API_TYPE SizeHintEvent
    {
        FTK_UI_API SizeHintEvent(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            float displayScale,
            const std::shared_ptr<Style>&);

        //! The font system, for measuring text.
        std::shared_ptr<FontSystem> fontSystem;
        //! The icon system, for getting icon images by name.
        std::shared_ptr<IconSystem> iconSystem;
        //! The display scale to size the widget for.
        float                       displayScale = 1.F;
        //! The style, for the size roles, color roles and fonts.
        std::shared_ptr<Style>      style;
    };

    //! Draw event.
    struct FTK_UI_API_TYPE DrawEvent
    {
        FTK_UI_API DrawEvent(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            float displayScale,
            const std::shared_ptr<Style>&,
            const std::shared_ptr<IRender>&);

        //! The font system, for the text to draw.
        std::shared_ptr<FontSystem> fontSystem;
        //! The icon system, for getting icon images by name.
        std::shared_ptr<IconSystem> iconSystem;
        //! The display scale to draw the widget at.
        float                       displayScale = 1.F;
        //! The style, for the size roles, color roles and fonts.
        std::shared_ptr<Style>      style;
        //! The renderer to draw with.
        std::shared_ptr<IRender>    render;
    };

    //! Base class for drag and drop data.
    class FTK_UI_API_TYPE IDragDropData : public std::enable_shared_from_this<IDragDropData>
    {
    public:
        FTK_UI_API virtual ~IDragDropData() = 0;
    };

    //! Mouse enter event.
    struct FTK_UI_API_TYPE MouseEnterEvent
    {
        FTK_UI_API MouseEnterEvent(const V2I& pos);

        //! Set by the widget to take the hover: it is then sent the mouse
        //! moves, and a leave event when the cursor goes. Left unset, the
        //! widgets it is inside are asked in turn.
        bool accept = false;
        //! The cursor position, in pixels from the top left of the window: the
        //! coordinates widget geometry is in.
        V2I  pos;
    };

    //! Mouse buttons.
    enum class FTK_UI_API_TYPE MouseButton
    {
        None,
        Left,
        Middle,
        Right,
        Extra1,
        Extra2,

        Count,
        First = None
    };
    FTK_ENUM(FTK_UI_API, MouseButton);

    //! Mouse move event.
    struct FTK_UI_API_TYPE MouseMoveEvent
    {
        FTK_UI_API MouseMoveEvent(
            const V2I&  pos,
            const V2I&  prev,
            MouseButton button = MouseButton::None,
            int         modifiers = 0);

        //! The cursor position, in pixels from the top left of the window: the
        //! coordinates widget geometry is in.
        V2I                            pos;
        //! The cursor position before the move, in the same coordinates.
        V2I                            prev;

        //! The button held down during the move, and the modifiers it was
        //! pressed with. None when nothing is held.
        //!
        //! Here because a widget that drags needs to know which button is
        //! doing it, and without this every one of them records the button in
        //! its press handler and clears it in its release handler.
        MouseButton                    button = MouseButton::None;
        int                            modifiers = 0;

        //! The keyboard modifiers held now, which can differ from those
        //! the button was pressed with: a key pressed during a drag, to
        //! hold a shape square as it is drawn, is only seen here.
        int                            keyModifiers = 0;

        //! Whether a pen made the event, and how hard it presses, zero to
        //! one. A mouse or a finger is no pen and presses with one.
        bool                           pen = false;
        float                          pressure = 1.F;
        //! Whether it is the pen's eraser end, turned over to rub out.
        bool                           eraser = false;

        //! Whether a finger on a touch screen made the event: a target
        //! for a fingertip is bigger than one for a pointer.
        bool                           touch = false;

        //! Set by a widget that handles the move. The window does not read it:
        //! a move goes to the pressed widget, or else to the hovered one,
        //! whatever this says.
        bool                           accept = false;
        //! Set by the pressed widget, as it is sent a move, to start a drag and
        //! drop of this data. The widget is then sent a release and a leave,
        //! and the widgets under the cursor are sent the drag events until the
        //! button is released.
        std::shared_ptr<IDragDropData> dragDropData;
        //! An image drawn at the cursor while the drag and drop lasts, set with
        //! the data.
        std::shared_ptr<Image>         dragDropCursor;
        //! The point in the cursor image that is put at the cursor, in pixels
        //! of the image.
        V2I                            dragDropCursorHotspot;
    };

    //! Keyboard modifiers.
    //!
    //! Note: This is a bitfield enum and is not compatible with FTK_ENUM,
    //! which requires a contiguous Count member. See to_string/from_string
    //! below for serialization support.
    enum class FTK_UI_API_TYPE KeyModifier
    {
        None    = 0,
        Shift   = 1,
        Control = 2,
        Alt     = 4,
        Super   = 8
    };

    //! Convert to a string.
    FTK_UI_API std::string to_string(KeyModifier);

    //! Convert from a string.
    FTK_UI_API bool from_string(const std::string&, KeyModifier&);

    //! Check for the given key modifier.
    FTK_UI_API bool checkKeyModifier(KeyModifier, int modifiers);

    //! OS specific command key modifier.
#if defined(__APPLE__)
    const KeyModifier commandKeyModifier = KeyModifier::Super;
#else // __APPLE__
    const KeyModifier commandKeyModifier = KeyModifier::Control;
#endif // __APPLE__

    //! Get a keyboard modifiers label.
    FTK_UI_API std::string getKeyModifierLabel(int);

    //! Mouse click event.
    struct FTK_UI_API_TYPE MouseClickEvent
    {
        FTK_UI_API MouseClickEvent() = default;
        FTK_UI_API MouseClickEvent(
            MouseButton button,
            int         modifiers,
            const V2I&  pos);

        //! The button pressed or released.
        MouseButton button    = MouseButton::None;
        //! The keyboard modifiers held, as KeyModifier flags.
        int         modifiers = 0;
        //! The cursor position, in pixels from the top left of the window: the
        //! coordinates widget geometry is in.
        V2I         pos;
        //! Whether a pen made the event, and how hard it presses; see
        //! MouseMoveEvent.
        bool        pen       = false;
        float       pressure  = 1.F;
        bool        eraser    = false;
        //! Whether a finger made the event; see MouseMoveEvent.
        bool        touch     = false;
        //! Set by a widget to take the press: it becomes the pressed widget,
        //! which is sent the mouse moves and the release wherever the cursor
        //! goes. Left unset, the widgets under the cursor are asked in turn,
        //! deepest first, and if none takes it a right button can open a
        //! context menu. The release does not read it.
        bool        accept    = false;

        //! The press was taken away before it was released: a touch
        //! gesture started, or a finger rested long enough to be the
        //! right button (longPress). The release ends it without acting
        //! on it, so a button under the first finger does not click.
        bool        cancel    = false;
        //! The press became a long press: cancelled, as above, but what
        //! the press did stands -- the frame it went to, the item it
        //! chose -- since the context menu that follows is about where
        //! the finger rests. A gesture undoes it instead: the first finger
        //! was not choosing anything.
        bool        longPress = false;
    };

    //! Scroll event (mouse wheel or touch pad).
    struct FTK_UI_API_TYPE ScrollEvent
    {
        FTK_UI_API ScrollEvent(
            const V2F& value,
            int        modifiers,
            const V2I& pos);

        //! The amount scrolled, in mouse wheel steps rather than pixels: x
        //! across and y up and down.
        V2F  value;
        //! The keyboard modifiers held, as KeyModifier flags.
        int  modifiers = 0;
        //! The cursor position, in pixels from the top left of the window: the
        //! coordinates widget geometry is in.
        V2I  pos;
        //! Set by a widget that takes the scroll. Left unset, the widgets under
        //! the cursor are asked in turn, deepest first, as far as a dialog.
        bool accept    = false;
    };

    //! Touch gesture event: two fingers dragging and pinching.
    struct FTK_UI_API_TYPE GestureEvent
    {
        FTK_UI_API GestureEvent(
            const V2I& pos,
            const V2F& pan,
            float      zoom);

        //! The point between the fingers.
        V2I   pos;

        //! How far that point moved since the last event.
        V2F   pan;

        //! How much the distance between the fingers changed since the
        //! last event, as a ratio.
        float zoom   = 1.F;

        //! Set by a widget to take the gesture as it starts: the rest of it
        //! goes to that widget, wherever the fingers go. Left unset, the
        //! widgets under the fingers are asked in turn, deepest first, as far
        //! as a dialog.
        bool  accept = false;
    };

    //! Keys.
    enum class FTK_UI_API_TYPE Key
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
        KeypadMultiply,
        KeypadMinus,
        KeypadPlus,
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
    FTK_ENUM(FTK_UI_API, Key);

    //! Get whether the given key is a control key.
    bool isControlKey(Key);

    //! Get a keyboard shortcut label.
    FTK_UI_API std::string getShortcutLabel(Key, int modifiers = 0);

    //! Key event.
    struct FTK_UI_API_TYPE KeyEvent
    {
        FTK_UI_API KeyEvent() = default;
        FTK_UI_API KeyEvent(
            Key        key,
            int        modifiers,
            const V2I& pos);

        //! The key.
        Key  key       = Key::Unknown;
        //! The keyboard modifiers held, as KeyModifier flags.
        int  modifiers = 0;
        //! The cursor position, in pixels from the top left of the window: the
        //! coordinates widget geometry is in.
        V2I  pos;
        //! Set by a widget that takes the key: it is sent the release, and no
        //! other widget is asked. Left unset, the key focus widget and its
        //! parents are asked in turn, as far as a dialog, then the widgets
        //! under the cursor, deepest first; a Tab that none takes moves the key
        //! focus.
        bool accept    = false;
    };

    //! Text event.
    struct FTK_UI_API_TYPE TextEvent
    {
        FTK_UI_API TextEvent(const std::string& text);

        //! The text entered, in UTF-8.
        std::string text;
        //! Set by a widget that takes the text. The text goes to the key focus
        //! widget and, while this is unset, on to its parents in turn.
        bool        accept = false;
    };

    //! Text editing event: the in-progress composition an input method
    //! is building, shown inline before it is committed as text input.
    struct FTK_UI_API_TYPE TextEditingEvent
    {
        FTK_UI_API TextEditingEvent(const std::string& text, int cursor);

        std::string text;
        int         cursor = 0;   //!< Cursor position within the text, in code points.
        bool        accept = false;
    };

    //! Drag and drop text data.
    class FTK_UI_API_TYPE DragDropTextData : public IDragDropData
    {
    public:
        FTK_UI_API DragDropTextData(const std::vector<std::string>&);

        FTK_UI_API virtual ~DragDropTextData();

        //! Get the text. For files dropped on the application from outside,
        //! their paths.
        FTK_UI_API const std::vector<std::string>& getText() const;

    private:
        std::vector<std::string> _text;
    };

    //! Drag and drop event.
    struct FTK_UI_API_TYPE DragDropEvent
    {
        FTK_UI_API DragDropEvent(
            const V2I&                            pos,
            const V2I&                            prev,
            const std::shared_ptr<IDragDropData>& data);

        //! The cursor position, in pixels from the top left of the window: the
        //! coordinates widget geometry is in.
        V2I                            pos;
        //! The previous cursor position, in the same coordinates; for files
        //! dropped from outside the application, the same as pos.
        V2I                            prev;
        //! The data being dragged.
        std::shared_ptr<IDragDropData> data;
        //! Set by a widget, as it is sent the drag enter event, to be the drop
        //! target: it is then sent the drag moves, the drop and a leave. For
        //! files dropped from outside the application, set in the drop event to
        //! stop the widgets further out being asked.
        bool                           accept = false;
    };
        
    ///@}
}
