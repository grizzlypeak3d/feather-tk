// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>

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
    /*auto label = Label::create(context, "Hello world");
    label->setMarginRole(SizeRole::MarginSmall);
    label->setHAlign(HAlign::Center);
    window->setWidget(label);*/

    auto layout = VerticalLayout::create(context);
    layout->setMarginRole(SizeRole::MarginDialog);
    auto lineEdit = LineEdit::create(context, layout);
    auto intEdit = IntEdit::create(context, layout);
    auto floatEdit = FloatEdit::create(context, layout);
    window->setWidget(layout);

    // Run the application.
    app->run();
    return 0;
}
