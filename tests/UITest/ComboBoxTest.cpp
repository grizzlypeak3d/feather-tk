// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ComboBoxTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ComboBoxTest::ComboBoxTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ComboBoxTest")
        {}

        ComboBoxTest::~ComboBoxTest()
        {}

        std::shared_ptr<ComboBoxTest> ComboBoxTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ComboBoxTest>(new ComboBoxTest(context));
        }
                
        void ComboBoxTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ComboBoxTest");
                auto app = App::create(
                    context,
                    argv,
                    "ComboBoxTest",
                    "Combo box test.");
                auto window = Window::create(context, app, "ComboBoxTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                std::vector<ComboBoxItem> items =
                {
                    ComboBoxItem("Stop", "PlaybackStop"),
                    ComboBoxItem("Forward", "PlaybackForward"),
                    ComboBoxItem("Reverse", "PlaybackReverse")
                };
                auto widget = ComboBox::create(context, items, layout);
                FTK_ASSERT(items[0] != ComboBoxItem());
                widget->setItems(items);
                widget->setItems(items);
                FTK_ASSERT(items == widget->getItems());
                FTK_ASSERT(0 == widget->getCurrentIndex());
                widget->setCurrentIndex(1);
                widget->setCurrentIndex(1);
                FTK_ASSERT(1 == widget->getCurrentIndex());
                int index = -1;
                widget->setIndexCallback(
                    [&index](int value)
                    {
                        index = value;
                    });
                ComboBoxItem item;
                widget->setItemCallback(
                    [&item](const ComboBoxItem& value)
                    {
                        item = value;
                    });
                widget->setFontRole(FontRole::Mono);
                widget->setFontRole(FontRole::Mono);
                FTK_ASSERT(FontRole::Mono == widget->getFontRole());
                widget->setFontRole(FontRole::Label);

                app->setDisplayScale(2.F);
                app->tick();
                app->setDisplayScale(1.F);
                app->tick();
            }
        }
    }
}

