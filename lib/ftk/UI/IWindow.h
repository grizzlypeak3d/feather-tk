// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

#include <vector>

#include <ftk/Core/Util.h>

namespace ftk
{
    class App;

    //! Window offscreen buffer types.
    enum class FTK_UI_API_TYPE WindowBufferType
    {
        U8,
        F16,
        F32,

        Count,
        First = U8
    };
    FTK_ENUM(FTK_UI_API, WindowBufferType);

    //! Default window buffer type.
#if defined(FTK_API_GL_4_1)
    const WindowBufferType windowBufferTypeDefault = WindowBufferType::F16;
#elif defined(FTK_API_GLES_3)
    const WindowBufferType windowBufferTypeDefault = WindowBufferType::U8;
#endif // FTK_API_GL_4_1

    //! Base class for windows.
    class FTK_UI_API_TYPE IWindow : public IWidget
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title);

        IWindow();

    public:
        FTK_UI_API virtual ~IWindow() = 0;

        //! Get the application.
        FTK_UI_API std::shared_ptr<App> getApp() const;

        //! Get the window ID.
        FTK_UI_API virtual uint32_t getID() const = 0;

        //! Get which screen the window is on.
        FTK_UI_API virtual int getScreen() const = 0;

        //! Window Title
        ///@{

        FTK_UI_API const std::string& getTitle() const;
        FTK_UI_API virtual void setTitle(const std::string&);

        ///@}

        //! Window Size
        ///@{

        FTK_UI_API const Size2I& getSize() const;
        FTK_UI_API virtual void setSize(const Size2I&);

        FTK_UI_API Size2I getMinSize() const;
        FTK_UI_API virtual void setMinSize(const Size2I&);

        ///@}

        //! Fullscreen
        ///@{

        FTK_UI_API bool isFullScreen() const;
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeFullScreen() const;
        FTK_UI_API virtual void setFullScreen(bool);

        ///@}

        //! Float On Top
        ///@{

        FTK_UI_API bool isFloatOnTop() const;
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeFloatOnTop() const;
        FTK_UI_API virtual void setFloatOnTop(bool);

        //! Raise the window above the others and give it the input focus.
        //! Does nothing where there is no window manager to ask.
        FTK_UI_API virtual void raise();

        ///@}

        //! Buffer
        ///@{

        FTK_UI_API const Size2I& getBufferSize() const;

        FTK_UI_API WindowBufferType getBufferType() const;
        FTK_UI_API std::shared_ptr<IObservable<WindowBufferType> > observeBufferType() const;
        FTK_UI_API void setBufferType(WindowBufferType);

        ///@}

        //! Display Scale
        ///@{

        FTK_UI_API float getDisplayScale() const;
        FTK_UI_API std::shared_ptr<IObservable<float> > observeDisplayScale() const;
        FTK_UI_API void setDisplayScale(float);

        FTK_UI_API float getContentScale() const;

        ///@}

        //! Key Focus and Text Input
        ///@{

        FTK_UI_API std::shared_ptr<IWidget> getKeyFocus() const;
        FTK_UI_API void setKeyFocus(const std::shared_ptr<IWidget>&);
        FTK_UI_API std::shared_ptr<IWidget> getNextKeyFocus(const std::shared_ptr<IWidget>&);
        FTK_UI_API std::shared_ptr<IWidget> getPrevKeyFocus(const std::shared_ptr<IWidget>&);

        //! Get whether the key focus is shown. The focus follows every
        //! interaction, but it is only drawn once the keyboard is used --
        //! a mouse user is not shown focus rings they never asked for.
        //! Using the keyboard shows the focus, and pressing a mouse
        //! button hides it again.
        FTK_UI_API bool isKeyFocusVisible() const;

        FTK_UI_API bool hasTextInput() const;
        FTK_UI_API virtual void setTextInput(bool);

        //! Set the text input area: where an input method places its
        //! candidate window, in framebuffer coordinates.
        FTK_UI_API virtual void setTextInputArea(const Box2I&);

        ///@}

        //! Tooltips
        ///@{

        FTK_UI_API bool getTooltipsEnabled() const;
        FTK_UI_API void setTooltipsEnabled(bool);

        ///@}

        //! Cursor and Drag and Drop
        ///@{

        //! Get the cursor position.
        FTK_UI_API const V2I& getCursorPos() const;

        //! Get whether a drag and drop is in progress. Drag events only
        //! reach the widget under the cursor, so a widget that reacts to a
        //! drag being anywhere near it -- a scroll area moving a drop target
        //! into reach -- asks here.
        FTK_UI_API bool isDragDropActive() const;

        ///@}

        //! Automation
        //!
        //! Working the window without a person at it: tests, screenshot
        //! harnesses, demos, anything that has to press a button and see what
        //! happens. Public rather than protected because the callers are not
        //! all tests -- an application that captures its own screenshots
        //! needs exactly this.
        ///@{

        //! Lay the window out at a size, as a resize from the window system
        //! would.
        //!
        //! Until this happens the window and everything in it has an empty
        //! geometry, and nothing is under the cursor -- so a test that reads a
        //! geometry, or aims at one, is reading and aiming at nothing. It does
        //! not fail; it passes, having checked nothing.
        FTK_UI_API void layout(const Size2I&);

        //! Move the cursor there, press, release.
        FTK_UI_API void click(
            const V2I&,
            MouseButton = MouseButton::Left,
            int modifiers = 0);

        //! Press at the first point, move through the rest, release.
        //!
        //! Moved in steps between the points rather than jumped, because a
        //! widget that treats a drag as one gesture and a widget that treats
        //! every move as a separate edit look identical from a single move --
        //! and the difference between them is the whole question for undo.
        //! Points beyond the second are how a drag that goes somewhere and
        //! comes back is written.
        FTK_UI_API void drag(
            const std::vector<V2I>&,
            int modifiers = 0,
            bool release = true);

        //! Press a key and release it.
        FTK_UI_API void keyPress(Key, int modifiers = 0);

        //! Enter text, the way typing does after the key events: letters
        //! reach a widget as text input, not as key presses.
        FTK_UI_API void text(const std::string&);

        //! Send an input method composition, the way an IME does while
        //! text is being composed; committing is text().
        FTK_UI_API void textEditing(const std::string&, int cursor = 0);

        ///@}

        //! Set the window icon.
        //! 
        //! Icon images should be of type ImageType::RGBA_U8, with no
        //! mirroring, memory alignment of one, and LSB memory endian.
        //!
        //! Window icons are not supported on macOS.
        FTK_UI_API virtual void setIcon(const std::shared_ptr<Image>&);

        //! Get whether the window is drawn without being shown.
        FTK_UI_API bool isOffscreen() const;

        //! Set whether the window is drawn without being shown.
        //!
        //! The widgets still tick and draw into the offscreen buffer, so
        //! screenshot() returns the same picture as it would from a window on
        //! screen. What changes is that there is no window to be clicked,
        //! hovered or raised over, which would otherwise put a highlight or a
        //! tooltip into an automated capture.
        FTK_UI_API void setOffscreen(bool);

        //! Capture a screenshot.
        FTK_UI_API virtual std::shared_ptr<Image> screenshot(const Box2I & = Box2I(0, 0, -1, -1));

        //! Close the window.
        FTK_UI_API virtual void close();

        //! Set the window close callback.
        FTK_UI_API void setCloseCallback(const std::function<void(void)>&);

        //! Get the window information.
        FTK_UI_API virtual std::vector<std::pair<std::string, std::string> > getWindowInfo() const;

        FTK_UI_API void setVisible(bool) override;
        FTK_UI_API void childAddEvent(const ChildAddEvent&) override;
        FTK_UI_API void tickEvent(
            bool parentsVisible,
            bool parentsEnabled,
            const TickEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawOverlayEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_UI_API virtual void _setSize(
            const Size2I& windowSize,
            const Size2I& bufferSize);

        //! The platform reported the window shown or hidden: update the
        //! widgets without commanding the platform window. The window
        //! flags trail the event queue, so answering these events with
        //! show or hide acts on stale state and feeds back -- hiding the
        //! application on macOS turned into an endless shown/hidden loop.
        FTK_UI_API void _setVisibleFromEvent(bool);

        virtual void _update(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            const std::shared_ptr<Style>&);

        bool _hasDrawUpdate(const std::shared_ptr<IWidget>&) const;
        bool _hasSizeUpdate(const std::shared_ptr<IWidget>&) const;

        bool _key(Key, bool press, int modifiers);
        void _text(const std::string&);
        void _textEditing(const std::string&, int cursor);
        FTK_UI_API void _cursorEnter(bool enter);
        FTK_UI_API void _cursorPos(const V2I&);
        FTK_UI_API void _mouseButton(MouseButton, bool press, int modifiers);
        void _scroll(const V2F&, int modifiers);
        void _drop(const V2I& pos, const std::shared_ptr<IDragDropData>&);

        void _drawEventRecursive(
            const std::shared_ptr<IWidget>&,
            const Box2I&,
            const DrawEvent&);
        void _styleEventRecursive(
            const std::shared_ptr<IWidget>&,
            const StyleEvent&);
        void _sizeHintEventRecursive(
            const std::shared_ptr<IWidget>&,
            const SizeHintEvent&);
        void _clipEventRecursive(
            const std::shared_ptr<IWidget>&,
            const Box2I&,
            bool clipped);

    private:
        enum class UnderCursor
        {
            Hover,
            Tooltip
        };
        std::list<std::shared_ptr<IWidget> > _getUnderCursor(
            UnderCursor,
            const V2I&);
        void _getUnderCursor(
            UnderCursor,
            const std::shared_ptr<IWidget>&,
            const V2I&,
            std::list<std::shared_ptr<IWidget> >&);

        void _hoverUpdate(MouseMoveEvent&);

        bool _contextMenu(const std::list<std::shared_ptr<IWidget> >&);

        std::vector<std::shared_ptr<IWidget> > _getKeyFocusOrder();
        static std::shared_ptr<IWidget> _stepKeyFocus(
            const std::vector<std::shared_ptr<IWidget> >&,
            const std::shared_ptr<IWidget>&,
            bool prev,
            size_t steps);
        void _getKeyFocus(
            const std::shared_ptr<IWidget>&,
            std::list<std::shared_ptr<IWidget> >&);

        void _trace(const std::string&);
        void _closeTooltip();

        friend class App;

        FTK_PRIVATE();
    };
}
