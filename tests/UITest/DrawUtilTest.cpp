// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DrawUtilTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        DrawUtilTest::DrawUtilTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DrawUtilTest")
        {}

        DrawUtilTest::~DrawUtilTest()
        {}

        std::shared_ptr<DrawUtilTest> DrawUtilTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DrawUtilTest>(new DrawUtilTest(context));
        }

        namespace
        {
            class DrawWidget : public IWidget
            {
            protected:
                DrawWidget()
                {}

            public:
                virtual ~DrawWidget()
                {}

                static std::shared_ptr<DrawWidget> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent = nullptr)
                {
                    auto out = std::shared_ptr<DrawWidget>(new DrawWidget);
                    out->_init(context, "ftk::ui_test::DrawWidget", parent);
                    return out;
                }

                void update()
                {
                    setDrawUpdate();
                }

                void drawEvent(const Box2I& drawRect, const DrawEvent& event) override
                {
                    IWidget::drawEvent(drawRect, event);
                    const Box2I& g = getGeometry();
                    event.render->drawMesh(rect(g));
                    event.render->drawMesh(rect(g, 20));
                    event.render->drawMesh(circle(center(g), 20));
                    event.render->drawMesh(border(g, 10));
                    event.render->drawMesh(border(g, 10, 20));
                    event.render->drawColorMesh(shadow(g, 20));
                    event.render->drawColorMesh(checkers(
                        g,
                        Color4F(1.F, 1.F, 1.F),
                        Color4F(1.F, 1.F, 1.F),
                        Size2I(100, 100)));
                }
            };
        }
                
        void DrawUtilTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("DrawUtilTest");
                auto app = App::create(
                    context,
                    argv,
                    "DrawUtilTest",
                    "Draw utilities test.");
                auto window = Window::create(context, app, "DrawUtilTest");
                window->show();
                app->tick();

                auto widget = DrawWidget::create(context, window);
                for (size_t i = 0; i < 10; ++i)
                {
                    widget->update();
                    app->tick();
                }
            }
        }
    }
}

