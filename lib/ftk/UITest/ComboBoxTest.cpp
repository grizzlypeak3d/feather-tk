// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ComboBoxTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/ItemButton.h>
#include <ftk/UI/ItemButtonList.h>
#include <ftk/UI/Label.h>
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
            {
                std::vector<std::string> argv;
                argv.push_back("ComboBoxTest");
                auto app = App::create(
                    _context,
                    argv,
                    "ComboBoxTest",
                    "Combo box test.");
                auto window = Window::create(_context, app, "ComboBoxTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                std::vector<ComboBoxItem> items =
                {
                    ComboBoxItem("Stop", "PlaybackStop"),
                    ComboBoxItem("Forward", "PlaybackForward"),
                    ComboBoxItem("Reverse", "PlaybackReverse")
                };
                auto widget = ComboBox::create(_context, items, layout);
                FTK_CHECK(items[0] != ComboBoxItem());
                widget->setItems(items);
                widget->setItems(items);
                FTK_CHECK(items == widget->getItems());
                FTK_CHECK(0 == widget->getCurrentIndex());
                widget->setCurrentIndex(1);
                widget->setCurrentIndex(1);
                FTK_CHECK(1 == widget->getCurrentIndex());
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
                widget->setFont(FontType::Mono);
                widget->setFont(FontType::Mono);
                FTK_CHECK(FontType::Mono == widget->getFont());
                widget->setFont(FontType::Regular);

                app->setDisplayScale(2.F);
                app->tick();
                app->setDisplayScale(1.F);
                app->tick();
            }

            // A clicked combo box takes the key focus, and the focus used to
            // stay parked there -- invisibly, since the focus only draws for
            // the keyboard -- answering every arrow key meant for whatever
            // the cursor was over. A press now moves the focus with it, so
            // clicking anything else sends the focus away and the arrows
            // fall through to the list under the cursor.
            {
                std::vector<std::string> argv;
                argv.push_back("ComboBoxTest");
                auto app = App::create(
                    _context,
                    argv,
                    "ComboBoxTest",
                    "Combo box test.");
                auto window = Window::create(_context, app, "ComboBoxTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                std::vector<ComboBoxItem> items =
                {
                    ComboBoxItem("One"),
                    ComboBoxItem("Two"),
                    ComboBoxItem("Three")
                };
                auto combo = ComboBox::create(_context, items, layout);
                auto list = ItemButtonList::create(_context, layout);
                for (int i = 0; i < 3; ++i)
                {
                    auto item = ItemButton::create(_context, list);
                    item->setAcceptsKeyFocus(false);
                    item->setWidget(Label::create(_context, "Row"));
                }
                window->show();
                window->layout(Size2I(1280, 960));
                app->tick();

                // Click the combo box, which opens its menu and takes the
                // key focus; click again to close the menu.
                const Box2I& comboG = combo->getGeometry();
                const V2I comboCenter(
                    comboG.min.x + comboG.w() / 2,
                    comboG.min.y + comboG.h() / 2);
                window->click(comboCenter);
                app->tick();
                window->click(comboCenter);
                app->tick();
                FTK_CHECK(combo->hasKeyFocus());

                // Click a row: the row accepts the press without wanting
                // the focus, and the focus goes away rather than staying
                // on the combo box.
                const auto rows = list->getItems();
                const Box2I& rowG = rows[0]->getGeometry();
                window->click(V2I(
                    rowG.min.x + rowG.w() / 2,
                    rowG.min.y + rowG.h() / 2));
                app->tick();
                FTK_CHECK(!combo->hasKeyFocus());

                // The arrows over the list now reach the list, not the
                // combo box.
                const int comboIndex = combo->getCurrentIndex();
                window->keyPress(Key::Down);
                app->tick();
                FTK_CHECK(comboIndex == combo->getCurrentIndex());
                FTK_CHECK(0 == list->getCurrent());
            }
        }
    }
}

