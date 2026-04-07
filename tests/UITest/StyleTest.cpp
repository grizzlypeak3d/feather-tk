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
                auto sizeRoles = style->getSizeRoles();
                sizeRoles[SizeRole::Border] = 10;
                style->setSizeRoles(sizeRoles);
                style->setSizeRoles(sizeRoles);
                FTK_ASSERT(10 == style->getSizeRole(SizeRole::Border, 1.F));
                app->tick();

                Color4F color(1.F, 0.F, 0.F);
                auto colorRoles = style->getColorRoles();
                colorRoles[ColorRole::Window] = color;
                style->setColorRoles(colorRoles);
                style->setColorRoles(colorRoles);
                FTK_ASSERT(color == style->getColorRole(ColorRole::Window));
                app->tick();

                auto fonts = style->getFonts();
                fonts[FontType::Regular] = "test";
                style->setFonts(fonts);
                style->setFonts(fonts);
                FTK_ASSERT("test" == style->getFont(FontType::Regular, 1.F).name);
                app->tick();
            }
        }
    }
}

