// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWindow.h>

namespace ftk
{
    //! Window.
    class Window : public IWindow
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title,
            const Size2I&);

        Window();

    public:
        virtual ~Window();

        //! Create a new window.
        static std::shared_ptr<Window> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title,
            const Size2I& = Size2I(1280, 960));

        uint32_t getID() const override;
        int getScreen() const override;
        void setTitle(const std::string&) override;
        void setSize(const Size2I&) override;
        void setMinSize(const Size2I&) override;
        void setFullScreen(bool) override;
        void setFloatOnTop(bool) override;
        void setIcon(const std::shared_ptr<Image>&) override;
        std::shared_ptr<Image> screenshot(const Box2I& = Box2I(0, 0, -1, -1)) override;

        void setGeometry(const Box2I&) override;
        void setVisible(bool) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        void _makeCurrent();
        void _clearCurrent();

        void _update(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            const std::shared_ptr<Style>&) override;

    private:
        friend class App;

        FTK_PRIVATE();
    };
}
