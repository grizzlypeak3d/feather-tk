// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ColorWidgetTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ColorPopup.h>
#include <ftk/UI/ColorSwatch.h>
#include <ftk/UI/ColorWidget.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ColorWidgetTest::ColorWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ColorWidgetTest")
        {}

        ColorWidgetTest::~ColorWidgetTest()
        {}

        std::shared_ptr<ColorWidgetTest> ColorWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ColorWidgetTest>(new ColorWidgetTest(context));
        }
                
        void ColorWidgetTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ColorWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "ColorWidgetTest",
                    "Color widget test.");
                auto window = Window::create(context, app, "ColorWidgetTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget = ColorSwatch::create(context, layout);
                Color4F color(1.F, 1.F, 1.F, 1.F);
                widget->setColor(color);
                widget->setColor(color);
                FTK_ASSERT(color == widget->getColor());
                FTK_ASSERT(!widget->isEditable());
                widget->setEditable(true);
                widget->setEditable(true);
                FTK_ASSERT(widget->isEditable());
                widget->setCallback(
                    [&color](const Color4F& value)
                    {
                        color = value;
                    });
                widget->setSizeRole(SizeRole::Margin);
                widget->setSizeRole(SizeRole::Margin);
                FTK_ASSERT(SizeRole::Margin == widget->getSizeRole());
                widget->setSizeRole(SizeRole::Swatch);

                auto popup = ColorPopup::create(context, color);
                popup->setPopupRole(ColorRole::Red);
                popup->setPopupRole(ColorRole::Red);
                FTK_ASSERT(ColorRole::Red == popup->getPopupRole());
                popup->open(window, widget->getGeometry());
                app->tick();
                FTK_ASSERT(popup->isOpen());
                popup->close();
                app->tick();
                FTK_ASSERT(!popup->isOpen());
            }
        }
    }
}

