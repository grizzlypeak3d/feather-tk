// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/StyleTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Style.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        StyleTest::StyleTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::StyleTest")
        {}

        StyleTest::~StyleTest()
        {}

        std::shared_ptr<StyleTest> StyleTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<StyleTest>(new StyleTest(context));
        }
                
        void StyleTest::run()
        {
            _enums();
            _widget();
        }
        
        void StyleTest::_enums()
        {
            FTK_TEST_ENUM(SizeRole);
            FTK_TEST_ENUM(ColorRole);
            FTK_TEST_ENUM(FontRole);
        }

        void StyleTest::_widget()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("StyleTest");
                auto app = App::create(
                    context,
                    argv,
                    "StyleTest",
                    "Style test.");
                auto window = Window::create(context, app, "StyleTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                auto button = PushButton::create(context, "Button", layout);
                window->show();
                app->tick();

                auto style = app->getStyle();
                int changed = 0;
                auto observer = Observer<bool>::create(
                    style->observeChanged(),
                    [&changed](bool)
                    {
                        ++changed;
                    });
                
                style->setSizeRole(SizeRole::Border, 10);
                style->setSizeRole(SizeRole::Border, 10);
                FTK_ASSERT(10 == style->getSizeRole(SizeRole::Border, 1.F));
                FTK_ASSERT(2 == changed);
                app->tick();
                std::map<SizeRole, int> sizeRoles =
                {
                    { SizeRole::Margin, 20 },
                    { SizeRole::Spacing, 20 },
                    { SizeRole::Border, 2 }
                };
                style->setSizeRoles(sizeRoles);
                style->setSizeRoles(sizeRoles);
                FTK_ASSERT(3 == changed);
                app->tick();

                Color4F color(1.F, 0.F, 0.F);
                style->setColorRole(ColorRole::Window, color);
                style->setColorRole(ColorRole::Window, color);
                FTK_ASSERT(color == style->getColorRole(ColorRole::Window));
                FTK_ASSERT(4 == changed);
                app->tick();
                std::map<ColorRole, Color4F> colorRoles =
                {
                    { ColorRole::Window, Color4F(0.F, 1.F, 0.F) },
                    { ColorRole::Text, Color4F(0.F, 0.F, 1.F) },
                    { ColorRole::Border, Color4F(1.F, 0.F, 0.F) }
                };
                style->setColorRoles(colorRoles);
                style->setColorRoles(colorRoles);
                FTK_ASSERT(5 == changed);
                app->tick();

                FontInfo fontInfo;
                fontInfo.size = 32;
                style->setFontRole(FontRole::Label, fontInfo);
                style->setFontRole(FontRole::Label, fontInfo);
                FTK_ASSERT(fontInfo == style->getFontRole(FontRole::Label, 1.F));
                FTK_ASSERT(6 == changed);
                app->tick();
                fontInfo.size = 16;
                std::map<FontRole, FontInfo> fontRoles =
                {
                    { FontRole::Label, fontInfo }
                };
                style->setFontRoles(fontRoles);
                style->setFontRoles(fontRoles);
                FTK_ASSERT(7 == changed);
                app->tick();
            }
        }
    }
}

