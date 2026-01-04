// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MainWindow.h>

using namespace ftk;

int main(int argc, char** argv)
{
    // Create the context and application.
    auto context = Context::create();
    auto app = App::create(context, argc, argv, "simple", "Simple example");
    if (app->getExit() != 0)
        return app->getExit();

    // Create a window.
    auto window = MainWindow::create(context, app, Size2I(1280, 960));

    // Create a label.
    auto label = Label::create(context, "Hello world");
    label->setFontRole(FontRole::Title);
    label->setAlign(HAlign::Center, VAlign::Center);
    window->setWidget(label);

    // Run the application.
    app->run();
    return 0;
}
