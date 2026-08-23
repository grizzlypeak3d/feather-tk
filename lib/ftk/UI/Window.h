// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWindow.h>

namespace ftk
{
    //! Window.
    class FTK_UI_API_TYPE Window : public IWindow
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title,
            const Size2I&);

        FTK_UI_API Window();

    public:
        FTK_UI_API virtual ~Window();

        //! Create a new window.
        FTK_UI_API static std::shared_ptr<Window> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::string& title,
            const Size2I& = Size2I(1280, 960));

        FTK_UI_API uint32_t getID() const override;
        FTK_UI_API int getScreen() const override;
        FTK_UI_API void setTitle(const std::string&) override;
        FTK_UI_API void setSize(const Size2I&) override;
        FTK_UI_API void setMinSize(const Size2I&) override;
        FTK_UI_API void setFullScreen(bool) override;
        FTK_UI_API void setFloatOnTop(bool) override;
        FTK_UI_API void raise() override;
        FTK_UI_API void setTextInput(bool) override;
        FTK_UI_API void setIcon(const std::shared_ptr<Image>&) override;
        FTK_UI_API std::shared_ptr<Image> screenshot(const Box2I& = Box2I(0, 0, -1, -1)) override;
        FTK_UI_API std::vector<std::pair<std::string, std::string> > getWindowInfo() const override;

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void setVisible(bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    protected:
        FTK_UI_API void _makeCurrent();
        void _clearCurrent();

        FTK_UI_API void _update(
            const std::shared_ptr<FontSystem>&,
            const std::shared_ptr<IconSystem>&,
            const std::shared_ptr<Style>&) override;

    private:
        friend class App;

        FTK_PRIVATE();
    };
}
