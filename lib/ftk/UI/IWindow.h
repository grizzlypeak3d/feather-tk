// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    class App;

    //! Base class for windows.
    class IWindow : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title);

        IWindow();

    public:
        virtual ~IWindow() = 0;

        //! Get the application.
        std::shared_ptr<App> getApp() const;

        //! Get the window ID.
        virtual uint32_t getID() const = 0;

        //! Get which screen the window is on.
        virtual int getScreen() const = 0;

        //! Window Title
        ///@{

        const std::string& getTitle() const;
        virtual void setTitle(const std::string&);

        ///@}

        //! Window Size
        ///@{

        const Size2I& getSize() const;
        virtual void setSize(const Size2I&);

        Size2I getMinSize() const;
        virtual void setMinSize(const Size2I&);

        ///@}

        //! Fullscreen
        ///@{

        bool isFullScreen() const;
        std::shared_ptr<IObservable<bool> > observeFullScreen() const;
        virtual void setFullScreen(bool);

        ///@}

        //! Float On Top
        ///@{

        bool isFloatOnTop() const;
        std::shared_ptr<IObservable<bool> > observeFloatOnTop() const;
        virtual void setFloatOnTop(bool);

        ///@}

        //! Frame Buffer
        ///@{

        const Size2I& getFrameBufferSize() const;

        ImageType getFrameBufferType() const;
        std::shared_ptr<IObservable<ImageType> > observeFrameBufferType() const;
        void setFrameBufferType(ImageType);

        ///@}

        //! Display Scale
        ///@{

        float getDisplayScale() const;
        std::shared_ptr<IObservable<float> > observeDisplayScale() const;
        void setDisplayScale(float);

        float getContentScale() const;

        ///@}

        //! Key Focus
        ///@{

        std::shared_ptr<IWidget> getKeyFocus() const;
        void setKeyFocus(const std::shared_ptr<IWidget>&);
        std::shared_ptr<IWidget> getNextKeyFocus(const std::shared_ptr<IWidget>&);
        std::shared_ptr<IWidget> getPrevKeyFocus(const std::shared_ptr<IWidget>&);

        ///@}

        //! Tooltips
        ///@{

        bool getTooltipsEnabled() const;
        void setTooltipsEnabled(bool);

        ///@}

        //! Set the window icon.
        //! 
        //! Icon images should be of type ImageType::RGBA_U8, with no
        //! mirroring, memory alignment of one, and LSB memory endian.
        //!
        //! Window icons are not supported on macOS.
        virtual void setIcon(const std::shared_ptr<Image>&);

        //! Capture a screenshot.
        virtual std::shared_ptr<Image> screenshot(const Box2I & = Box2I(0, 0, -1, -1));

        //! Close the window.
        virtual void close();

        //! Set the window close callback.
        void setCloseCallback(const std::function<void(void)>&);

        void setVisible(bool) override;
        void tickEvent(
            bool parentsVisible,
            bool parentsEnabled,
            const TickEvent&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawOverlayEvent(const Box2I&, const DrawEvent&) override;

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

        bool _key(Key, bool press, int modifiers);
        void _text(const std::string&);
        void _cursorEnter(bool enter);
        void _cursorPos(const V2I&);
        void _mouseButton(int button, bool press, int modifiers);
        void _scroll(const V2F&, int modifiers);

        void _clipEventRecursive(
            const std::shared_ptr<IWidget>&,
            const Box2I&,
            bool clipped);

        virtual void _drop(const std::vector<std::string>&);

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
