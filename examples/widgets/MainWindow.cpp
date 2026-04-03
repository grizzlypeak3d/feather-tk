// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "MainWindow.h"

#include "App.h"
#include "Bellows.h"
#include "Buttons.h"
#include "Charts.h"
#include "Dialogs.h"
#include "DragDrop.h"
#include "Fonts.h"
#include "Graphs.h"
#include "Icons.h"
#include "Images.h"
#include "Layouts.h"
#include "Lists.h"
#include "MDI.h"
#include "Offscreen.h"
#include "Popups.h"
#include "Sliders.h"
#include "Splitters.h"
#include "Stack.h"
#include "SysLog.h"

#include <ftk/UI/Divider.h>
#include <ftk/UI/MenuBar.h>

using namespace ftk;

namespace widgets
{
    void MainWindow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        ftk::MainWindow::_init(context, app, Size2I(1920, 1080));

        _app = app;

        _widgets["Bellows"] = &Bellows::create;
        _widgets["Buttons"] = &Buttons::create;
        _widgets["Charts"] = &Charts::create;
        _widgets["Dialogs"] = &Dialogs::create;
        _widgets["DragDrop"] = &DragDrop::create;
        _widgets["Fonts"] = &Fonts::create;
        _widgets["Graphs"] = &Graphs::create;
        _widgets["Icons"] = &Icons::create;
        _widgets["Images"] = &Images::create;
        _widgets["Layouts"] = &Layouts::create;
        _widgets["Lists"] = &Lists::create;
        _widgets["MDI"] = &MDI::create;
        _widgets["Offscreen"] = &Offscreen::create;
        _widgets["Popups"] = &Popups::create;
        _widgets["Sliders"] = &Sliders::create;
        _widgets["Splitters"] = &Splitters::create;
        _widgets["Stack"] = &Stack::create;
        _widgets["SysLog"] = &SysLog::create;

        // Create test menus.
        auto menuBar = getMenuBar();
        auto menu = Menu::create(context);
        menu->addAction(Action::create(
            "Action 1",
            [] { std::cout << "Action 1" << std::endl; }));
        menu->addAction(Action::create(
            "Action 2",
            "FileOpen",
            [] { std::cout << "Action 2" << std::endl; }));
        menu->addDivider();
        menu->addAction(Action::create(
            "Action 3",
            KeyShortcut(Key::_3, KeyModifier::Control),
            [] { std::cout << "Action 3" << std::endl; }));
        menu->addDivider();
        menu->addAction(Action::create(
            "Action 4",
            "FileClose",
            KeyShortcut(Key::_4, KeyModifier::Control),
            [] { std::cout << "Action 4" << std::endl; }));
        menuBar->addMenu("Menu 1", menu);
        menu = Menu::create(context);
        menu->addAction(Action::create(
            "Action 5",
            [](bool value) { std::cout << "Action 5: " << value << std::endl; }));
        menu->addAction(Action::create(
            "Action 6",
            "Next",
            [](bool value) { std::cout << "Action 6: " << value << std::endl; }));
        menu->addAction(Action::create(
            "Action 7",
            KeyShortcut(Key::_7),
            [](bool value) { std::cout << "Action 7: " << value << std::endl; }));
        menu->addAction(Action::create(
            "Action 8",
            "Prev",
            KeyShortcut(Key::_8),
            [](bool value) { std::cout << "Action 8: " << value << std::endl; }));
        menuBar->addMenu("Menu 2", menu);

        menu = Menu::create(context);
        menu->addAction(Action::create(
            "Action 9",
            [] { std::cout << "Action 9" << std::endl; }));
        auto subMenu = menu->addSubMenu("Sub Menu 1");
        subMenu->addAction(Action::create(
            "Action 10",
            [] { std::cout << "Action 10" << std::endl; }));
        subMenu = menu->addSubMenu("Sub Menu 2");
        subMenu->addAction(Action::create(
            "Action 11",
            [] { std::cout << "Action 11" << std::endl; }));
        subMenu->addAction(Action::create(
            "Action 12",
            [] { std::cout << "Action 12" << std::endl; }));
        subMenu = menu->addSubMenu("Sub Menu 3");
        subMenu->addAction(Action::create(
            "Action 13",
            [] { std::cout << "Action 13" << std::endl; }));
        subMenu->addAction(Action::create(
            "Action 14",
            [] { std::cout << "Action 14" << std::endl; }));
        subMenu->addAction(Action::create(
            "Action 15",
            [] { std::cout << "Action 15" << std::endl; }));
        subMenu = menu->addSubMenu("Sub Menu 4");
        menu->addAction(Action::create(
            "Action 16",
            [] { std::cout << "Action 16" << std::endl; }));
        menuBar->addMenu("Menu 3", menu);

        menu = Menu::create(context);
        menuBar->addMenu("Menu 4", menu);

        _tabBar = TabBar::create(context);
        for (const auto& i : _widgets)
        {
            _tabBar->addTab(i.first);
        }

        auto layout = VerticalLayout::create(context);
        layout->setSpacingRole(SizeRole::None);
        _tabBar->setParent(layout);
        Divider::create(context, Orientation::Vertical, layout);
        _layout = VerticalLayout::create(context, layout);
        _layout->setSpacingRole(SizeRole::None);
        _layout->setVStretch(Stretch::Expanding);
        setWidget(layout);

        _tabUpdate(0);

        _tabBar->setCallback(
            [this](int index)
            {
                _tabUpdate(index);
            });
    }

    MainWindow::~MainWindow()
    {}

    std::shared_ptr<MainWindow> MainWindow::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        auto out = std::shared_ptr<MainWindow>(new MainWindow);
        out->_init(context, app);
        return out;
    }

    void MainWindow::_tabUpdate(int index)
    {
        _layout->clear();
        std::vector<std::string> tmp;
        for (const auto& i : _widgets)
        {
            tmp.push_back(i.first);
        }
        if (index >= 0 && index < tmp.size())
        {
            auto widget = _widgets[tmp[index]](getContext(), _app.lock(), _layout);
            widget->setVStretch(Stretch::Expanding);
        }
    }
}
