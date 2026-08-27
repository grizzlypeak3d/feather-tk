// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Spacer.h>

using namespace ftk;

int main(int argc, char** argv)
{
    try
    {
        // Create the context and application.
        auto context = Context::create();
        auto app = App::create(context, argc, argv, "simple", "Simple example.");
        if (app->hasCmdLineHelp())
            return 0;

        // Create a window.
        auto window = MainWindow::create(context, app, Size2I(1280, 960));

        // Create a label, centered in the window: the layout centers it
        // across, and the spacers center it along.
        auto layout = VerticalLayout::create(context);
        auto spacer = Spacer::create(context, Orientation::Vertical, layout);
        spacer->setStretch(Stretch::Expanding);
        auto label = Label::create(context, "Hello world", layout);
        label->setFontSize(32);
        label->setHAlign(HAlign::Center);
        spacer = Spacer::create(context, Orientation::Vertical, layout);
        spacer->setStretch(Stretch::Expanding);
        window->setWidget(layout);

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
