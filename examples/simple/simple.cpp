// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>
#include <ftk/UI/TextEdit.h>
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
    auto textEdit = TextEdit::create(context);
    textEdit->setText({ "Hello world" });
    textEdit->setMarginRole(SizeRole::MarginSmall);
    textEdit->setTextCallback(
        [textEdit](const std::vector<std::string>& text)
        {
            std::string s = ftk::join(text, '\n');
            textEdit->setText(ftk::split(s, '\n', SplitOptions::KeepEmpty));
        });
    window->setWidget(textEdit);

    // Run the application.
    app->run();
    return 0;
}
