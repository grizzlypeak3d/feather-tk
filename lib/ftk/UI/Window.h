// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWindow.h>

namespace ftk
{
    //! Window.
    class FTK_API_TYPE Window : public IWindow
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title,
            const Size2I&);

        Window();

    public:
        FTK_API virtual ~Window();

        //! Create a new window.
        FTK_API static std::shared_ptr<Window> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title,
            const Size2I& = Size2I(1280, 960));

        FTK_API uint32_t getID() const override;
        FTK_API int getScreen() const override;
        FTK_API void setTitle(const std::string&) override;
        FTK_API void setSize(const Size2I&) override;
        FTK_API void setMinSize(const Size2I&) override;
        FTK_API void setFullScreen(bool) override;
        FTK_API void setFloatOnTop(bool) override;
        FTK_API void setIcon(const std::shared_ptr<Image>&) override;
        FTK_API std::shared_ptr<Image> screenshot(const Box2I& = Box2I(0, 0, -1, -1)) override;
        FTK_API std::vector<std::pair<std::string, std::string> > getWindowInfo() const override;

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void setVisible(bool) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

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
