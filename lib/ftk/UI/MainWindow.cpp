// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MainWindow.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/App.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/MenuBar.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    struct MainWindow::Private
    {
        std::shared_ptr<MenuBar> menuBar;
        std::map<std::string, std::shared_ptr<Menu> > menus;
        std::vector<float> displayScales = { 1.F, 1.5F, 2.F, 2.5F, 3.F, 3.5F, 4.F };
        std::vector<std::shared_ptr<Action> > displayScaleActions;
        std::map<ColorStyle, std::shared_ptr<Action> > colorStyleActions;
        std::shared_ptr<Action> tooltipsAction;
        std::shared_ptr<Divider> menuBarDivider;
        std::shared_ptr<IWidget> centralWidget;
        std::shared_ptr<VerticalLayout> layout;

        std::shared_ptr<Observer<float> > displayScaleObserver;
        std::shared_ptr<Observer<ColorStyle> > colorStyleObserver;
        std::shared_ptr<Observer<bool> > tooltipsObserver;
    };

    void MainWindow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const Size2I& size)
    {
        Window::_init(context, app, app->getName(), size);
        FTK_P();

        setIcon(context->getSystem<IconSystem>()->get("feather_tk_512", 1.F));

        p.menuBar = MenuBar::create(context);
        auto fileMenu = Menu::create(context);
        std::weak_ptr<App> appWeak(app);
        fileMenu->addAction(Action::create(
            "Exit",
            Key::Q,
            static_cast<int>(commandKeyModifier),
            [appWeak]
            {
                if (auto app = appWeak.lock())
                {
                    app->exit();
                }
            }));
        p.menuBar->addMenu("File", fileMenu);

        p.menus["Window"] = Menu::create(context);
        p.menus["Window"]->addAction(Action::create(
            "Full Screen",
            Key::U,
            static_cast<int>(commandKeyModifier),
            [this](bool value)
            {
                setFullScreen(value);
            }));

        p.menus["ColorStyle"] = p.menus["Window"]->addSubMenu("Color Style");
        for (auto colorStyle : getColorStyleEnums())
        {
            auto action = Action::create(
                getLabel(colorStyle),
                [appWeak, colorStyle]
                {
                    if (auto app = appWeak.lock())
                    {
                        app->setColorStyle(colorStyle);
                    }
                });
            p.colorStyleActions[colorStyle] = action;
            p.menus["ColorStyle"]->addAction(action);
        }

        p.menus["DisplayScale"] = p.menus["Window"]->addSubMenu("Display Scale");
        for (size_t i = 0; i < p.displayScales.size(); ++i)
        {
            const float displayScale = p.displayScales[i];
            auto action = Action::create(
                Format("{0}").arg(displayScale).str(),
                [appWeak, displayScale](bool)
                {
                    if (auto app = appWeak.lock())
                    {
                        app->setDisplayScale(displayScale);
                    }
                });
            p.displayScaleActions.push_back(action);
            p.menus["DisplayScale"]->addAction(action);
        }

        p.tooltipsAction = Action::create(
            "Tooltips",
            [appWeak](bool value)
            {
                if (auto app = appWeak.lock())
                {
                    app->setTooltipsEnabled(value);
                }
            });
        p.menus["Window"]->addAction(p.tooltipsAction);

        p.menuBar->addMenu("Window", p.menus["Window"]);

        p.menuBarDivider = Divider::create(context, Orientation::Vertical);

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::None);
        p.menuBar->setParent(p.layout);
        p.menuBarDivider->setParent(p.layout);

        p.colorStyleObserver = Observer<ColorStyle>::create(
            app->observeColorStyle(),
            [this](ColorStyle value)
            {
                FTK_P();
                for (auto colorStyle : getColorStyleEnums())
                {
                    p.menus["ColorStyle"]->setChecked(
                        p.colorStyleActions[colorStyle],
                        colorStyle == value);
                }
            });

        p.displayScaleObserver = Observer<float>::create(
            app->observeDisplayScale(),
            [this](float value)
            {
                FTK_P();
                for (size_t i = 0; i < p.displayScales.size() && i < p.displayScaleActions.size(); ++i)
                {
                    p.menus["DisplayScale"]->setChecked(
                        p.displayScaleActions[i],
                        p.displayScales[i] == value);
                }
            });

        p.tooltipsObserver = Observer<bool>::create(
            app->observeTooltipsEnabled(),
            [this](bool value)
            {
                FTK_P();
                p.menus["Window"]->setChecked(p.tooltipsAction, value);
            });
    }

    MainWindow::MainWindow() :
        _p(new Private)
    {}

    MainWindow::~MainWindow()
    {}

    std::shared_ptr<MainWindow> MainWindow::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const Size2I& size)
    {
        auto out = std::shared_ptr<MainWindow>(new MainWindow);
        out->_init(context, app, size);
        return out;
    }

    const std::shared_ptr<MenuBar>& MainWindow::getMenuBar() const
    {
        return _p->menuBar;
    }

    void MainWindow::setMenuBar(const std::shared_ptr<MenuBar>& value)
    {
        FTK_P();
        if (p.menuBar)
        {
            p.menuBar->setParent(nullptr);
            p.menuBar.reset();
        }
        p.menuBar = value;
        if (p.menuBar)
        {
            p.menuBar->setParent(p.layout);
            p.layout->moveToBack(p.menuBar);
        }
    }

    const std::shared_ptr<IWidget>& MainWindow::getWidget() const
    {
        return _p->centralWidget;
    }

    void MainWindow::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        if (p.centralWidget)
        {
            p.centralWidget->setParent(nullptr);
        }
        p.centralWidget = value;
        if (p.centralWidget)
        {
            p.centralWidget->setStretch(Stretch::Expanding);
            p.centralWidget->setParent(p.layout);
        }
    }

    void MainWindow::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        event.accept = p.menuBar->shortcut(event.key, event.modifiers);
    }

    void MainWindow::keyReleaseEvent(KeyEvent& event)
    {
        event.accept = true;
    }
}
