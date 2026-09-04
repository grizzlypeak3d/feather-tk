// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

// Live preview of a widget layout from JSON. Point it at a file and
// edit the file in any editor: the window reloads when the file
// changes, and problems with the data show in the window instead of
// tearing it down.
//
//     preview panel.json
//
// The JSON holds the structure and the properties; see
// ftk::widgetLoad() for the format. Behavior stays in code, so a
// preview is a preview: the widgets work -- sliders slide, bellows
// open -- but nothing is wired to anything. The -screenshot and
// -widgetDump options work here like any application, which makes this
// a way to render a layout from a script as well as to iterate on one.

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/WidgetJson.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/Timer.h>

#include <PreviewJson.h>

#include <filesystem>
#include <fstream>

using namespace ftk;

namespace
{
    // Build the window contents from the file: the loaded widgets, with
    // any errors as labels above them. A file that cannot be read or
    // parsed at all still produces a window that says so, since an
    // empty window gives no clue what went wrong.
    std::shared_ptr<IWidget> load(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path)
    {
        std::vector<std::string> errors;
        nlohmann::json json;
        std::ifstream file(path);
        if (file)
        {
            try
            {
                file >> json;
            }
            catch (const std::exception& e)
            {
                errors.push_back(e.what());
            }
        }
        else
        {
            errors.push_back("Cannot read: " + ftk::fromFileSystem(path));
        }

        std::shared_ptr<IWidget> widget;
        if (errors.empty())
        {
            auto result = widgetLoad(context, json);
            widget = result.widget;
            errors = result.errors;
        }

        auto out = VerticalLayout::create(context);
        for (const auto& error : errors)
        {
            auto label = Label::create(context, error, out);
            label->setTextRole(ColorRole::Red);
            label->setMarginRole(SizeRole::MarginSmall);
        }
        if (widget)
        {
            widget->setParent(out);
        }
        return out;
    }
}

FTK_MAIN()
{
    try
    {
        auto context = Context::create();
        auto args = convert(argc, argv);
        auto fileArg = CmdLineArg<std::string>::create(
            "input",
            "The JSON file to preview. Without one, a built in sample "
            "is shown.",
            true);
        auto app = App::create(
            context,
            args,
            "preview",
            "Preview a widget layout from JSON, reloading when the file "
            "changes.",
            { fileArg });
        if (app->hasCmdLineHelp())
            return 0;

        auto window = MainWindow::create(context, app, Size2I(960, 720));
        auto scrollWidget = ScrollWidget::create(context, ScrollType::Both);
        scrollWidget->setBorder(false);
        window->setWidget(scrollWidget);

        std::shared_ptr<Timer> timer;
        if (fileArg->hasValue())
        {
            const std::filesystem::path path =
                ftk::toFileSystem(fileArg->getValue());
            scrollWidget->setWidget(load(context, path));

            // Watch the file by polling its write time; when it changes
            // the contents are rebuilt from scratch, which is what makes
            // the edit and look loop instant.
            std::error_code ec;
            auto writeTime = std::filesystem::last_write_time(path, ec);
            timer = Timer::create(context);
            timer->setRepeating(true);
            timer->start(
                std::chrono::milliseconds(250),
                [context, scrollWidget, path, writeTime]() mutable
                {
                    std::error_code ec;
                    const auto time = std::filesystem::last_write_time(path, ec);
                    if (!ec && time != writeTime)
                    {
                        writeTime = time;
                        scrollWidget->setWidget(load(context, path));
                    }
                });
        }
        else
        {
            // The sample panel is embedded at build time from
            // preview.json with ftk_embed_text(): the same file is the
            // live document during development and part of the binary
            // when it ships, which is the pattern for applications that
            // bundle their panels.
            auto result = widgetLoad(
                context,
                nlohmann::json::parse(previewJson));
            scrollWidget->setWidget(result.widget);
        }

        app->run();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
