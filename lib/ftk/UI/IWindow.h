// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    class App;

    //! Base class for windows.
    class FTK_API_TYPE IWindow : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title);

        IWindow();

    public:
        FTK_API virtual ~IWindow() = 0;

        //! Get the application.
        FTK_API std::shared_ptr<App> getApp() const;

        //! Get the window ID.
        FTK_API virtual uint32_t getID() const = 0;

        //! Get which screen the window is on.
        FTK_API virtual int getScreen() const = 0;

        //! Window Title
        ///@{

        FTK_API const std::string& getTitle() const;
        FTK_API virtual void setTitle(const std::string&);

        ///@}

        //! Window Size
        ///@{

        FTK_API const Size2I& getSize() const;
        FTK_API virtual void setSize(const Size2I&);

        FTK_API Size2I getMinSize() const;
        FTK_API virtual void setMinSize(const Size2I&);

        ///@}

        //! Fullscreen
        ///@{

        FTK_API bool isFullScreen() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeFullScreen() const;
        FTK_API virtual void setFullScreen(bool);

        ///@}

        //! Float On Top
        ///@{

        FTK_API bool isFloatOnTop() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeFloatOnTop() const;
        FTK_API virtual void setFloatOnTop(bool);

        ///@}

        //! Frame Buffer
        ///@{

        FTK_API const Size2I& getFrameBufferSize() const;

        FTK_API ImageType getFrameBufferType() const;
        FTK_API std::shared_ptr<IObservable<ImageType> > observeFrameBufferType() const;
        FTK_API void setFrameBufferType(ImageType);

        ///@}

        //! Display Scale
        ///@{

        FTK_API float getDisplayScale() const;
        FTK_API std::shared_ptr<IObservable<float> > observeDisplayScale() const;
        FTK_API void setDisplayScale(float);

        FTK_API float getContentScale() const;

        ///@}

        //! Key Focus
        ///@{

        FTK_API std::shared_ptr<IWidget> getKeyFocus() const;
        FTK_API void setKeyFocus(const std::shared_ptr<IWidget>&);
        FTK_API std::shared_ptr<IWidget> getNextKeyFocus(const std::shared_ptr<IWidget>&);
        FTK_API std::shared_ptr<IWidget> getPrevKeyFocus(const std::shared_ptr<IWidget>&);

        ///@}

        //! Tooltips
        ///@{

        FTK_API bool getTooltipsEnabled() const;
        FTK_API void setTooltipsEnabled(bool);

        ///@}

        //! Set the window icon.
        //! 
        //! Icon images should be of type ImageType::RGBA_U8, with no
        //! mirroring, memory alignment of one, and LSB memory endian.
        //!
        //! Window icons are not supported on macOS.
        FTK_API virtual void setIcon(const std::shared_ptr<Image>&);

        //! Capture a screenshot.
        FTK_API virtual std::shared_ptr<Image> screenshot(const Box2I & = Box2I(0, 0, -1, -1));

        //! Close the window.
        FTK_API virtual void close();

        //! Set the window close callback.
        FTK_API void setCloseCallback(const std::function<void(void)>&);

        //! Get the window information.
        FTK_API virtual std::vector<std::pair<std::string, std::string> > getWindowInfo() const;

        FTK_API void setVisible(bool) override;
        FTK_API void tickEvent(
            bool parentsVisible,
            bool parentsEnabled,
            const TickEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawOverlayEvent(const Box2I&, const DrawEvent&) override;

    protected:
        virtual void _sizeUpdate(
            const Size2I& windowSize, 
            const Size2I& frameBufferSize);
        virtual void _update(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            const std::shared_ptr<Style>&) = 0;

        bool _hasSizeUpdate(const std::shared_ptr<IWidget>&) const;
        void _sizeHintEventRecursive(
            const std::shared_ptr<IWidget>&,
            const SizeHintEvent&);

        bool _hasDrawUpdate(const std::shared_ptr<IWidget>&) const;
        void _drawEventRecursive(
            const std::shared_ptr<IWidget>&,
            const Box2I&,
            const DrawEvent&);

        void _key(Key, bool press, int modifiers);
        void _text(const std::string&);
        void _cursorEnter(bool enter);
        void _cursorPos(const V2I&);
        void _mouseButton(MouseButton, bool press, int modifiers);
        void _scroll(const V2F&, int modifiers);

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

        void _getKeyFocus(
            const std::shared_ptr<IWidget>&,
            std::list<std::shared_ptr<IWidget> >&);

        void _closeTooltip();

        friend class App;

        FTK_PRIVATE();
    };
}
