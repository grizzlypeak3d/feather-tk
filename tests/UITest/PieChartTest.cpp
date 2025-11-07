// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/PieChartTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/PieChart.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        PieChartTest::PieChartTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::PieChartTest")
        {}

        PieChartTest::~PieChartTest()
        {}

        std::shared_ptr<PieChartTest> PieChartTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<PieChartTest>(new PieChartTest(context));
        }
                
        void PieChartTest::run()
        {
            {
                PieChartData a;
                PieChartData b;
                b.percentage = 50.F;
                FTK_ASSERT(a == a);
                FTK_ASSERT(a != b);
            }
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("PieChartTest");
                auto app = App::create(
                    context,
                    argv,
                    "PieChartTest",
                    "Pie chart test.");
                auto window = Window::create(context, app, "PieChartTest");
                auto layout = HorizontalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget = PieChart::create(context, layout);
                std::vector<PieChartData> data;
                data.push_back(PieChartData(60.F, Color4F(1.F, 0.F, 0.F)));
                data.push_back(PieChartData(30.F, Color4F(1.F, .8F, 0.F)));
                data.push_back(PieChartData(10.F, Color4F(.8F, 1.F, 0.F)));
                widget->setData(data);
                widget->setData(data);
                FTK_ASSERT(data == widget->getData());
                app->tick();
                widget->setSizeMult(2);
                widget->setSizeMult(2);
                FTK_ASSERT(2 == widget->getSizeMult());
                app->tick();
            }
        }
    }
}

