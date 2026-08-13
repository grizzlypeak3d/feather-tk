// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/MenuBarTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ActionGroup.h>
#include <ftk/UI/MenuBar.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Time.h>

namespace ftk
{
    namespace ui_test
    {
        MenuBarTest::MenuBarTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::MenuBarTest")
        {}

        MenuBarTest::~MenuBarTest()
        {}

        std::shared_ptr<MenuBarTest> MenuBarTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<MenuBarTest>(new MenuBarTest(context));
        }
                
        void MenuBarTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("MenuBarTest");
                auto app = App::create(
                    _context,
                    argv,
                    "MenuBarTest",
                    "Menu bar test.");
                auto window = Window::create(_context, app, "MenuBarTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto menuBar = MenuBar::create(_context, layout);

                auto menu = Menu::create(_context);
                bool action1 = false;
                menu->addAction(Action::create(
                    "Action 1",
                    "FileOpen",
                    KeyShortcut(Key::O, KeyModifier::Control),
                    [&action1] { action1 = true; }));
                bool action2 = false;
                auto action = Action::create(
                    "Action 2",
                    "Mute",
                    KeyShortcut(Key::M, KeyModifier::Control),
                    [&action2](bool value) { action2 = value; });
                menu->addAction(action);
                menu->setChecked(action, true);
                menu->setEnabled(action, false);
                auto subMenu = menu->addSubMenu("Sub Menu");
                bool action3 = false;
                subMenu->addAction(Action::create(
                    "Action 3",
                    [&action3] { action3 = true; }));
                menuBar->addMenu("Menu 1", menu);
                app->tick();

                menu = Menu::create(_context);
                bool action4 = false;
                menu->addAction(Action::create(
                    "Action 4",
                    KeyShortcut(Key::_4, KeyModifier::Control),
                    [&action4] { action4 = true; }));
                bool action5 = false;
                menu->addAction(Action::create(
                    "Action 5",
                    KeyShortcut(Key::_5, KeyModifier::Control),
                    [&action5](bool value) { action5 = value; }));
                menuBar->addMenu("Menu 2", menu);
                app->tick();

                // A shortcut has to do what a click does. IButton::_click()
                // runs the clicked callback and then, if the button is
                // checkable, the checked one -- so an action carrying both
                // has to see both, whichever way it was reached. Given only
                // one of them, an action with a plain callback in a radio
                // group did nothing at all from the keyboard.
                {
                    auto radioMenu = Menu::create(_context);
                    auto group = ActionGroup::create(ActionGroupType::Radio);
                    int plain = 0;
                    int checkedCount = 0;
                    auto radio1 = Action::create(
                        "Radio 1",
                        KeyShortcut(Key::F1),
                        [&plain] { ++plain; });
                    auto radio2 = Action::create(
                        "Radio 2",
                        KeyShortcut(Key::F2),
                        [&plain] { ++plain; });
                    group->addAction(radio1);
                    group->addAction(radio2);
                    radioMenu->addAction(radio1);
                    radioMenu->addAction(radio2);
                    group->setCheckedCallback(
                        [&checkedCount](int, bool) { ++checkedCount; });
                    menuBar->addMenu("Radio", radioMenu);
                    app->tick();

                    FTK_CHECK(radio1->isCheckable());
                    menuBar->shortcut(Key::F2, 0);
                    // The plain callback ran, which is the whole report: it
                    // used to be skipped for anything checkable.
                    FTK_CHECK(1 == plain);
                    FTK_CHECK(radio2->isChecked());
                    FTK_CHECK(!radio1->isChecked());

                    // And again on the one already selected. The shortcut
                    // toggles it off, exactly as the button would, and
                    // ActionGroup turns it straight back on -- so it ends up
                    // selected either way, and the group reports nothing.
                    const int before = checkedCount;
                    menuBar->shortcut(Key::F2, 0);
                    FTK_CHECK(2 == plain);
                    FTK_CHECK(radio2->isChecked());
                    FTK_CHECK(before == checkedCount);
                }

                menuBar->shortcut(Key::_4, static_cast<int>(KeyModifier::Control));
                FTK_CHECK(action4);
                menuBar->shortcut(Key::_5, static_cast<int>(KeyModifier::Control));
                FTK_CHECK(action5);
                menuBar->shortcut(Key::_5, static_cast<int>(KeyModifier::Control));
                FTK_CHECK(!action5);

                app->setDisplayScale(2.F);
                app->tick();
                app->setDisplayScale(1.F);
                app->tick();

                menu->clear();
            }
        }
    }
}

