// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ftk-test.h"

#include <UITest/ActionTest.h>
#include <UITest/AppTest.h>
#include <UITest/BellowsTest.h>
#include <UITest/ButtonGroupTest.h>
#include <UITest/ButtonTest.h>
#include <UITest/ColorWidgetTest.h>
#include <UITest/ComboBoxTest.h>
#include <UITest/ConfirmDialogTest.h>
#include <UITest/DoubleEditTest.h>
#include <UITest/DoubleEditSliderTest.h>
#include <UITest/DoubleSliderTest.h>
#include <UITest/DoubleModelTest.h>
#include <UITest/DragDropTest.h>
#include <UITest/DrawUtilTest.h>
#include <UITest/EventTest.h>
#include <UITest/FileBrowserTest.h>
#include <UITest/FileEditTest.h>
#include <UITest/FloatEditTest.h>
#include <UITest/FloatEditSliderTest.h>
#include <UITest/FloatSliderTest.h>
#include <UITest/FloatModelTest.h>
#include <UITest/GridLayoutTest.h>
#include <UITest/GroupBoxTest.h>
#include <UITest/IWidgetTest.h>
#include <UITest/IconTest.h>
#include <UITest/IntEditTest.h>
#include <UITest/IntEditSliderTest.h>
#include <UITest/IntSliderTest.h>
#include <UITest/IntModelTest.h>
#include <UITest/LabelTest.h>
#include <UITest/LayoutUtilTest.h>
#include <UITest/LineEditModelTest.h>
#include <UITest/LineEditTest.h>
#include <UITest/ListWidgetTest.h>
#include <UITest/MDIWidgetTest.h>
#include <UITest/MenuBarTest.h>
#include <UITest/MessageDialogTest.h>
#include <UITest/PieChartTest.h>
#include <UITest/ProgressDialogTest.h>
#include <UITest/RecentFilesModelTest.h>
#include <UITest/RowLayoutTest.h>
#include <UITest/ScrollAreaTest.h>
#include <UITest/ScrollBarTest.h>
#include <UITest/ScrollWidgetTest.h>
#include <UITest/SearchBoxTest.h>
#include <UITest/SplitterTest.h>
#include <UITest/StackLayoutTest.h>
#include <UITest/StyleTest.h>
#include <UITest/TabWidgetTest.h>
#include <UITest/TextEditModelTest.h>
#include <UITest/WidgetOptionsTest.h>

#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
#include <GLTest/MeshTest.h>
#include <GLTest/OffscreenBufferTest.h>
#include <GLTest/TextureAtlasTest.h>
#include <GLTest/TextureTest.h>
#include <GLTest/RenderTest.h>
#include <GLTest/ShaderTest.h>
#include <GLTest/UtilTest.h>
#include <GLTest/WindowTest.h>
#endif // FTK_API_GL_4_1

#include <CoreTest/AppTest.h>
#include <CoreTest/BoxPackTest.h>
#include <CoreTest/BoxTest.h>
#include <CoreTest/CmdLineTest.h>
#include <CoreTest/ColorTest.h>
#include <CoreTest/CommandTest.h>
#include <CoreTest/ErrorTest.h>
#include <CoreTest/FileIOTest.h>
#include <CoreTest/FontSystemTest.h>
#include <CoreTest/FormatTest.h>
#include <CoreTest/ImageIOTest.h>
#include <CoreTest/ImageTest.h>
#include <CoreTest/LRUCacheTest.h>
#include <CoreTest/MathTest.h>
#include <CoreTest/MatrixTest.h>
#include <CoreTest/MemoryTest.h>
#include <CoreTest/MeshTest.h>
#include <CoreTest/NoiseTest.h>
#include <CoreTest/OSTest.h>
#include <CoreTest/ObservableTest.h>
#include <CoreTest/PNGTest.h>
#include <CoreTest/PathTest.h>
#include <CoreTest/RandomTest.h>
#include <CoreTest/RangeTest.h>
#include <CoreTest/RenderOptionsTest.h>
#include <CoreTest/RenderUtilTest.h>
#include <CoreTest/SizeTest.h>
#include <CoreTest/StringTest.h>
#include <CoreTest/SystemTest.h>
#include <CoreTest/TimeTest.h>
#include <CoreTest/TimerTest.h>
#include <CoreTest/VectorTest.h>

#include <TestLib/ITest.h>

#include <ftk/UI/Init.h>

#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
#include <ftk/GL/Init.h>
#endif // FTK_API_GL_4_1

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Time.h>

#include <iostream>

namespace ftk
{
    namespace tests
    {
        struct App::Private
        {
            std::shared_ptr<CmdLineArg<std::string> > testName;
            std::vector<std::shared_ptr<test::ITest> > tests;
            std::chrono::steady_clock::time_point startTime;
        };
        
        void App::_init(
            const std::shared_ptr<Context>& context,
            std::vector<std::string>& argv)
        {
            FTK_P();
            p.testName = CmdLineArg<std::string>::create(
                "Test",
                "Name of the test to run.",
                true);
            IApp::_init(
                context,
                argv,
                "ftk-test",
                "Test application",
                { p.testName });
            p.startTime = std::chrono::steady_clock::now();                
#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
            gl::init(context);
#endif // FTK_API_GL_4_1
            uiInit(context);

            p.tests.push_back(core_test::AppTest::create(context));
            p.tests.push_back(core_test::BoxPackTest::create(context));
            p.tests.push_back(core_test::BoxTest::create(context));
            p.tests.push_back(core_test::CmdLineTest::create(context));
            p.tests.push_back(core_test::ColorTest::create(context));
            p.tests.push_back(core_test::CommandTest::create(context));
            p.tests.push_back(core_test::ErrorTest::create(context));
            p.tests.push_back(core_test::FileIOTest::create(context));
            p.tests.push_back(core_test::FontSystemTest::create(context));
            p.tests.push_back(core_test::FormatTest::create(context));
            p.tests.push_back(core_test::ImageIOTest::create(context));
            p.tests.push_back(core_test::ImageTest::create(context));
            p.tests.push_back(core_test::LRUCacheTest::create(context));
            p.tests.push_back(core_test::MathTest::create(context));
            p.tests.push_back(core_test::MatrixTest::create(context));
            p.tests.push_back(core_test::MemoryTest::create(context));
            p.tests.push_back(core_test::MeshTest::create(context));
            p.tests.push_back(core_test::NoiseTest::create(context));
            p.tests.push_back(core_test::OSTest::create(context));
            p.tests.push_back(core_test::ObservableTest::create(context));
            p.tests.push_back(core_test::PNGTest::create(context));
            p.tests.push_back(core_test::PathTest::create(context));
            p.tests.push_back(core_test::RandomTest::create(context));
            p.tests.push_back(core_test::RangeTest::create(context));
            p.tests.push_back(core_test::RenderOptionsTest::create(context));
            p.tests.push_back(core_test::RenderUtilTest::create(context));
            p.tests.push_back(core_test::SizeTest::create(context));
            p.tests.push_back(core_test::StringTest::create(context));
            p.tests.push_back(core_test::SystemTest::create(context));
            p.tests.push_back(core_test::TimeTest::create(context));
            p.tests.push_back(core_test::TimerTest::create(context));
            p.tests.push_back(core_test::VectorTest::create(context));

#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
            p.tests.push_back(gl_test::MeshTest::create(context));
            p.tests.push_back(gl_test::OffscreenBufferTest::create(context));
            p.tests.push_back(gl_test::TextureAtlasTest::create(context));
            p.tests.push_back(gl_test::TextureTest::create(context));
            p.tests.push_back(gl_test::RenderTest::create(context));
            p.tests.push_back(gl_test::ShaderTest::create(context));
            p.tests.push_back(gl_test::UtilTest::create(context));
            p.tests.push_back(gl_test::WindowTest::create(context));
#endif // FTK_API_GL_4_1

            p.tests.push_back(ui_test::ActionTest::create(context));
            p.tests.push_back(ui_test::AppTest::create(context));
            p.tests.push_back(ui_test::BellowsTest::create(context));
            p.tests.push_back(ui_test::ButtonTest::create(context));
            p.tests.push_back(ui_test::ButtonGroupTest::create(context));
            p.tests.push_back(ui_test::ColorWidgetTest::create(context));
            p.tests.push_back(ui_test::ComboBoxTest::create(context));
            p.tests.push_back(ui_test::ConfirmDialogTest::create(context));
            p.tests.push_back(ui_test::DoubleEditTest::create(context));
            p.tests.push_back(ui_test::DoubleEditSliderTest::create(context));
            p.tests.push_back(ui_test::DoubleSliderTest::create(context));
            p.tests.push_back(ui_test::DoubleModelTest::create(context));
            p.tests.push_back(ui_test::DragDropTest::create(context));
            p.tests.push_back(ui_test::DrawUtilTest::create(context));
            p.tests.push_back(ui_test::EventTest::create(context));
            p.tests.push_back(ui_test::FileBrowserTest::create(context));
            p.tests.push_back(ui_test::FileEditTest::create(context));
            p.tests.push_back(ui_test::FloatEditTest::create(context));
            p.tests.push_back(ui_test::FloatEditSliderTest::create(context));
            p.tests.push_back(ui_test::FloatSliderTest::create(context));
            p.tests.push_back(ui_test::FloatModelTest::create(context));
            p.tests.push_back(ui_test::GridLayoutTest::create(context));
            p.tests.push_back(ui_test::GroupBoxTest::create(context));
            p.tests.push_back(ui_test::IWidgetTest::create(context));
            p.tests.push_back(ui_test::IconTest::create(context));
            p.tests.push_back(ui_test::IntEditTest::create(context));
            p.tests.push_back(ui_test::IntEditSliderTest::create(context));
            p.tests.push_back(ui_test::IntSliderTest::create(context));
            p.tests.push_back(ui_test::IntModelTest::create(context));
            p.tests.push_back(ui_test::LabelTest::create(context));
            p.tests.push_back(ui_test::LayoutUtilTest::create(context));
            p.tests.push_back(ui_test::LineEditModelTest::create(context));
            p.tests.push_back(ui_test::LineEditTest::create(context));
            p.tests.push_back(ui_test::ListWidgetTest::create(context));
            p.tests.push_back(ui_test::MDIWidgetTest::create(context));
            p.tests.push_back(ui_test::MenuBarTest::create(context));
            p.tests.push_back(ui_test::MessageDialogTest::create(context));
            p.tests.push_back(ui_test::PieChartTest::create(context));
            p.tests.push_back(ui_test::ProgressDialogTest::create(context));
            p.tests.push_back(ui_test::RecentFilesModelTest::create(context));
            p.tests.push_back(ui_test::RowLayoutTest::create(context));
            p.tests.push_back(ui_test::ScrollAreaTest::create(context));
            p.tests.push_back(ui_test::ScrollBarTest::create(context));
            p.tests.push_back(ui_test::ScrollWidgetTest::create(context));
            p.tests.push_back(ui_test::SearchBoxTest::create(context));
            p.tests.push_back(ui_test::SplitterTest::create(context));
            p.tests.push_back(ui_test::StackLayoutTest::create(context));
            p.tests.push_back(ui_test::StyleTest::create(context));
            p.tests.push_back(ui_test::TabWidgetTest::create(context));
            p.tests.push_back(ui_test::TextEditModelTest::create(context));
            p.tests.push_back(ui_test::WidgetOptionsTest::create(context));
        }

        App::App() :
            _p(new Private)
        {}

        App::~App()
        {}

        std::shared_ptr<App> App::create(
            const std::shared_ptr<Context>& context,
            std::vector<std::string>& argv)
        {
            auto out = std::shared_ptr<App>(new App);
            out->_init(context, argv);
            return out;
        }
        
        void App::run()
        {
            FTK_P();

            // Get the tests to run.
            std::vector<std::shared_ptr<test::ITest> > runTests;
            for (const auto& test : p.tests)
            {
                if (!p.testName->hasValue() ||
                    (p.testName->hasValue() &&
                        contains(test->getName(), p.testName->getValue())))
                {
                    runTests.push_back(test);
                }
            }     

            // Run the tests.
            for (const auto& test : runTests)
            {
                _context->tick();
                _print(Format("Running test: {0}").arg(test->getName()));
                test->run();
            }
                
            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<float> diff = now - p.startTime;
            _print(Format("Seconds elapsed: {0}").arg(diff.count(), 2));
        }
    }
}

FTK_MAIN()
{
    int r = 0;
    try
    {
        auto context = ftk::Context::create();
        auto args = ftk::convert(argc, argv);
        auto app = ftk::tests::App::create(context, args);
        r = app->getExit();
        if (0 == r)
        {
            app->run();
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return r;
}

