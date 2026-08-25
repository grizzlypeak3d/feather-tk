// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/WidgetJsonTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/IntEditSlider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/WidgetDump.h>
#include <ftk/UI/WidgetJson.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        WidgetJsonTest::WidgetJsonTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::WidgetJsonTest")
        {}

        WidgetJsonTest::~WidgetJsonTest()
        {}

        std::shared_ptr<WidgetJsonTest> WidgetJsonTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<WidgetJsonTest>(new WidgetJsonTest(context));
        }

        void WidgetJsonTest::run()
        {
            std::vector<std::string> argv;
            argv.push_back("WidgetJsonTest");
            auto app = App::create(
                _context,
                argv,
                "WidgetJsonTest",
                "Widget JSON test.");
            auto window = Window::create(_context, app, "WidgetJsonTest");

            const nlohmann::json json = nlohmann::json::parse(R"({
                "type": "VerticalLayout",
                "marginRole": "Margin",
                "children": [
                    { "type": "Label", "text": "Hello" },
                    { "type": "FormLayout", "rows": [
                        { "label": "Size:", "widget": {
                            "type": "IntEditSlider", "id": "size",
                            "range": [ 1, 100 ], "value": 50, "default": 10 } },
                        { "label": "Mode:", "widget": {
                            "type": "ComboBox", "id": "mode",
                            "items": [ "One", "Two" ], "currentIndex": 1 } }
                    ] },
                    { "type": "Divider" },
                    { "type": "PushButton", "id": "apply", "text": "Apply",
                      "tooltip": "Apply the settings" }
                ]
            })");
            auto result = widgetLoad(_context, json, window);
            for (const auto& error : result.errors)
            {
                _print("Error: " + error);
            }
            FTK_CHECK(result.errors.empty());
            FTK_CHECK(result.widget);
            window->show();
            app->tick();

            // The ids find the widgets, with their properties applied.
            auto slider = std::dynamic_pointer_cast<IntEditSlider>(
                findWidget(result.widget, "size"));
            FTK_CHECK(slider);
            FTK_CHECK(RangeI(1, 100) == slider->getRange());
            FTK_CHECK(50 == slider->getValue());
            FTK_CHECK(10 == slider->getDefault());
            auto comboBox = std::dynamic_pointer_cast<ComboBox>(
                findWidget(result.widget, "mode"));
            FTK_CHECK(comboBox);
            FTK_CHECK(1 == comboBox->getCurrentIndex());
            auto button = std::dynamic_pointer_cast<PushButton>(
                findWidget(result.widget, "apply"));
            FTK_CHECK(button);
            FTK_CHECK("Apply" == button->getText());
            FTK_CHECK(!findWidget(result.widget, "missing"));

            // Behavior stays in code: the id is how a callback finds its
            // widget.
            bool clicked = false;
            button->setClickedCallback([&clicked] { clicked = true; });
            window->layout(Size2I(1280, 960));
            app->tick();
            const Box2I& g = button->getGeometry();
            window->click(V2I(g.min.x + g.w() / 2, g.min.y + g.h() / 2));
            app->tick();
            FTK_CHECK(clicked);

            // Round trip: a loaded tree dumps back out and loads again,
            // keeping its structure, ids, and text. The dump is a
            // diagnostic, so it records what is on screen rather than
            // how to rebuild it -- a form's rows dump as the widgets
            // inside it -- which is why the round trip is checked on the
            // composition vocabulary the two formats share. This is what
            // keeps the loader from drifting behind the widget set.
            const nlohmann::json simple = nlohmann::json::parse(R"({
                "type": "VerticalLayout",
                "children": [
                    { "type": "Label", "text": "Hello" },
                    { "type": "PushButton", "id": "apply", "text": "Apply" }
                ]
            })");
            const auto loaded = widgetLoad(_context, simple);
            FTK_CHECK(loaded.errors.empty());
            const nlohmann::json dump = widgetDump(loaded.widget);
            FTK_CHECK("ftk::VerticalLayout" == dump.at("type").get<std::string>());
            const auto reloaded = widgetLoad(_context, dump);
            for (const auto& error : reloaded.errors)
            {
                _print("Error: " + error);
            }
            FTK_CHECK(reloaded.errors.empty());
            FTK_CHECK(reloaded.widget);
            FTK_CHECK(2 == reloaded.widget->getChildren().size());
            auto button2 = std::dynamic_pointer_cast<PushButton>(
                findWidget(reloaded.widget, "apply"));
            FTK_CHECK(button2);
            FTK_CHECK("Apply" == button2->getText());

            // Errors report and do not stop the rest of the interface.
            const nlohmann::json badJson = nlohmann::json::parse(R"({
                "type": "VerticalLayout",
                "marginRole": "Bogus",
                "children": [
                    { "type": "Bogus" },
                    { "type": "Label", "text": "Still here" }
                ]
            })");
            const auto badResult = widgetLoad(_context, badJson);
            FTK_CHECK(2 == badResult.errors.size());
            FTK_CHECK(badResult.widget);
            FTK_CHECK(1 == badResult.widget->getChildren().size());

            // Applications can register their own types.
            widgetLoadRegister(
                "TestLabel",
                [](const std::shared_ptr<Context>& context,
                    const nlohmann::json& json,
                    WidgetLoadResult&)
                {
                    return Label::create(context, "Registered");
                });
            const auto custom = widgetLoad(
                _context,
                nlohmann::json::parse(R"({ "type": "TestLabel" })"));
            FTK_CHECK(custom.errors.empty());
            FTK_CHECK(std::dynamic_pointer_cast<Label>(custom.widget));
        }
    }
}
