// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/MDIWidgetTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/MDICanvas.h>
#include <ftk/UI/MDIWidget.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        MDIWidgetTest::MDIWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::MDIWidgetTest")
        {}

        MDIWidgetTest::~MDIWidgetTest()
        {}

        std::shared_ptr<MDIWidgetTest> MDIWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<MDIWidgetTest>(new MDIWidgetTest(context));
        }
                
        void MDIWidgetTest::run()
        {
            _enums();
            _widget();
        }

        void MDIWidgetTest::_enums()
        {
            FTK_TEST_ENUM(MDIResize);
        }

        void MDIWidgetTest::_widget()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("MDIWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "MDIWidgetTest",
                    "MDI widget test.");
                auto window = Window::create(context, app, "MDIWidgetTest");
                window->show();
                app->tick();

                auto canvas = MDICanvas::create(context, window);
                canvas->setCanvasSize(Size2I(100, 100));
                canvas->setCanvasSize(Size2I(100, 100));
                FTK_ASSERT(Size2I(100, 100) == canvas->getCanvasSize());
                canvas->setCanvasSize(Size2I(1000, 1000));
                canvas->setGridSize(Size2I(2, 2));
                canvas->setGridSize(Size2I(2, 2));
                FTK_ASSERT(Size2I(2, 2) == canvas->getGridSize());
                canvas->setGridSize(Size2I(20, 20));

                auto label = Label::create(context, "Label");
                auto widget = canvas->addWidget("Widget 0", V2I(100, 100), label);
                FTK_ASSERT("Widget 0" == widget->getTitle());
                widget->setTitle("Widget");
                FTK_ASSERT("Widget" == widget->getTitle());
                app->tick();
                label.reset();
                widget->setParent(nullptr);
                widget.reset();
                label = Label::create(context, "Label 1");
                widget = canvas->addWidget("Widget 1", V2I(100, 100), label);
                label = Label::create(context, "Label 2");
                widget->setWidget(label);
                widget->setWidget(label);
                FTK_ASSERT(label == widget->getWidget());
                widget->setPos(V2I(200, 200));
                widget->setSize(Size2I(1000, 1000));
            }
        }
    }
}

