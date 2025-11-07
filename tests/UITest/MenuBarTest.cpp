// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/MenuBarTest.h>

#include <ftk/UI/App.h>
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
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("MenuBarTest");
                auto app = App::create(
                    context,
                    argv,
                    "MenuBarTest",
                    "Menu bar test.");
                auto window = Window::create(context, app, "MenuBarTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto menuBar = MenuBar::create(context, layout);

                auto menu = Menu::create(context);
                bool action1 = false;
                menu->addAction(Action::create(
                    "Action 1",
                    "FileOpen",
                    Key::O,
                    static_cast<int>(KeyModifier::Control),
                    [&action1] { action1 = true; }));
                bool action2 = false;
                auto action = Action::create(
                    "Action 2",
                    "Mute",
                    Key::M,
                    static_cast<int>(KeyModifier::Control),
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

                menu = Menu::create(context);
                bool action4 = false;
                menu->addAction(Action::create(
                    "Action 4",
                    Key::_4,
                    static_cast<int>(KeyModifier::Control),
                    [&action4] { action4 = true; }));
                bool action5 = false;
                menu->addAction(Action::create(
                    "Action 5",
                    Key::_5,
                    static_cast<int>(KeyModifier::Control),
                    [&action5](bool value) { action5 = value; }));
                menuBar->addMenu("Menu 2", menu);
                app->tick();

                menuBar->shortcut(Key::_4, static_cast<int>(KeyModifier::Control));
                FTK_ASSERT(action4);
                menuBar->shortcut(Key::_5, static_cast<int>(KeyModifier::Control));
                FTK_ASSERT(action5);
                menuBar->shortcut(Key::_5, static_cast<int>(KeyModifier::Control));
                FTK_ASSERT(!action5);

                app->setDisplayScale(2.F);
                app->tick();
                app->setDisplayScale(1.F);
                app->tick();

                menu->clear();
            }
        }
    }
}

