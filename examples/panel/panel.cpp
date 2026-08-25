// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

// A tool panel from JSON, wired up: the pattern for an application
// that keeps its layouts in data. The structure and the properties
// live in panel.json, and the behavior lives here, attached to the
// widgets by their ids -- the way markup and script divide a web page.
//
// The JSON is embedded in the binary at build time with
// ftk_embed_text(), so the file is the source of truth during
// development -- iterate on it live with the preview example -- and
// part of the executable when it ships.

#include <ftk/UI/App.h>
#include <ftk/UI/IntEditSlider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/WidgetJson.h>

#include <ftk/Core/Format.h>

#include <PanelJson.h>

using namespace ftk;

FTK_MAIN()
{
    try
    {
        auto context = Context::create();
        auto app = App::create(
            context,
            convert(argc, argv),
            "panel",
            "Tool panel from JSON example.");
        if (app->hasCmdLineHelp())
            return 0;

        auto window = MainWindow::create(context, app, Size2I(640, 480));

        // Build the panel from the embedded JSON. The application's own
        // data is trusted, so errors here are programming mistakes and
        // fail loudly instead of showing a broken panel.
        auto result = widgetLoad(context, nlohmann::json::parse(panelJson));
        for (const auto& error : result.errors)
        {
            throw std::runtime_error(error);
        }
        window->setWidget(result.widget);

        // Find the widgets by id and attach the behavior. The casts are
        // to the concrete types the callbacks live on.
        auto widthSlider = std::dynamic_pointer_cast<IntEditSlider>(
            findWidget(result.widget, "width"));
        auto samplesSlider = std::dynamic_pointer_cast<IntEditSlider>(
            findWidget(result.widget, "samples"));
        auto outputEdit = std::dynamic_pointer_cast<LineEdit>(
            findWidget(result.widget, "output"));
        auto statusLabel = std::dynamic_pointer_cast<Label>(
            findWidget(result.widget, "status"));
        auto renderButton = std::dynamic_pointer_cast<PushButton>(
            findWidget(result.widget, "render"));

        auto status = [widthSlider, samplesSlider, statusLabel]
        {
            statusLabel->setText(Format("{0} pixels wide, {1} samples").
                arg(widthSlider->getValue()).
                arg(samplesSlider->getValue()));
        };
        widthSlider->setCallback([status](int) { status(); });
        samplesSlider->setCallback([status](int) { status(); });
        renderButton->setClickedCallback(
            [widthSlider, samplesSlider, outputEdit, statusLabel]
            {
                std::cout << Format("Render {0} wide, {1} samples, to {2}").
                    arg(widthSlider->getValue()).
                    arg(samplesSlider->getValue()).
                    arg(outputEdit->getText()) << std::endl;
                statusLabel->setText("Rendered " + outputEdit->getText());
            });

        app->run();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
