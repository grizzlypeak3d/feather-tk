// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

class SecondaryWindow : public Window
{
protected:
    void _init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        Window::_init(context, app, "secondary", Size2I(1280, 960));

        auto layout = VerticalLayout::create(context, shared_from_this());
        layout->setMarginRole(SizeRole::MarginLarge);
    }

    SecondaryWindow() = default;

public:
    virtual ~SecondaryWindow() {}

    static std::shared_ptr<SecondaryWindow> create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        std::shared_ptr<SecondaryWindow> out(new SecondaryWindow);
        out->_init(context, app);
        return out;
    }
};

class MyMainWindow : public MainWindow
{
protected:
    void _init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        MainWindow::_init(context, app, Size2I(1280, 960));

        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::MarginLarge);
        setWidget(layout);

        _button = PushButton::create(context, "Secondary Window", layout);
        _button->setCheckable(true);
        _button->setCheckedCallback(
            [this](bool value)
            {
                if (value)
                {
                    _secondaryWindow = SecondaryWindow::create(getContext(), getApp());
                    _secondaryWindow->setCloseCallback(
                        [this]
                        {
                            _button->setChecked(false);
                            _secondaryWindow.reset();
                        });
                    _secondaryWindow->show();
                }
                else
                {
                    if (_secondaryWindow)
                    {
                        _secondaryWindow->close();
                        _secondaryWindow.reset();
                    }
                }
            });

        setCloseCallback(
            [this]
            {
                if (_secondaryWindow)
                {
                    _secondaryWindow->close();
                    _secondaryWindow.reset();
                }
            });
    }

    MyMainWindow() = default;

public:
    virtual ~MyMainWindow()
    {}

    static std::shared_ptr<MyMainWindow> create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        std::shared_ptr<MyMainWindow> out(new MyMainWindow);
        out->_init(context, app);
        return out;
    }

private:
    std::shared_ptr<PushButton> _button;
    std::shared_ptr<SecondaryWindow> _secondaryWindow;
};

FTK_MAIN()
{
    try
    {
        // Create the context and application.
        auto context = Context::create();
        auto app = App::create(context, argc, argv, "windows", "Multiple windows example");
        if (app->getExit() != 0)
            return app->getExit();

        // Create the main window.
        auto mainWindow = MyMainWindow::create(context, app);

        // Run the application.
        app->run();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

