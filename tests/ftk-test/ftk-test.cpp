// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ftk-test.h"

#include <ftk/UITest/ActionTest.h>
#include <ftk/UITest/AppTest.h>
#include <ftk/UITest/BellowsTest.h>
#include <ftk/UITest/ButtonGroupTest.h>
#include <ftk/UITest/ButtonTest.h>
#include <ftk/UITest/ColorWidgetTest.h>
#include <ftk/UITest/ComboBoxTest.h>
#include <ftk/UITest/ConfirmDialogTest.h>
#include <ftk/UITest/DoubleEditTest.h>
#include <ftk/UITest/DoubleEditSliderTest.h>
#include <ftk/UITest/DoubleSliderTest.h>
#include <ftk/UITest/DoubleModelTest.h>
#include <ftk/UITest/DragDropTest.h>
#include <ftk/UITest/DrawUtilTest.h>
#include <ftk/UITest/EventTest.h>
#include <ftk/UITest/FileBrowserTest.h>
#include <ftk/UITest/FileEditTest.h>
#include <ftk/UITest/FloatEditTest.h>
#include <ftk/UITest/FloatEditSliderTest.h>
#include <ftk/UITest/FloatSliderTest.h>
#include <ftk/UITest/FloatModelTest.h>
#include <ftk/UITest/GridLayoutTest.h>
#include <ftk/UITest/GroupBoxTest.h>
#include <ftk/UITest/IWidgetTest.h>
#include <ftk/UITest/IconTest.h>
#include <ftk/UITest/IntEditTest.h>
#include <ftk/UITest/IntEditSliderTest.h>
#include <ftk/UITest/IntSliderTest.h>
#include <ftk/UITest/IntModelTest.h>
#include <ftk/UITest/LabelTest.h>
#include <ftk/UITest/LayoutUtilTest.h>
#include <ftk/UITest/LineEditModelTest.h>
#include <ftk/UITest/LineEditTest.h>
#include <ftk/UITest/ListWidgetTest.h>
#include <ftk/UITest/MDIWidgetTest.h>
#include <ftk/UITest/MenuBarTest.h>
#include <ftk/UITest/MessageDialogTest.h>
#include <ftk/UITest/PieChartTest.h>
#include <ftk/UITest/ProgressDialogTest.h>
#include <ftk/UITest/RecentFilesModelTest.h>
#include <ftk/UITest/RowLayoutTest.h>
#include <ftk/UITest/ScrollAreaTest.h>
#include <ftk/UITest/ScrollBarTest.h>
#include <ftk/UITest/ScrollWidgetTest.h>
#include <ftk/UITest/SearchBoxTest.h>
#include <ftk/UITest/SplitterTest.h>
#include <ftk/UITest/StackLayoutTest.h>
#include <ftk/UITest/StyleTest.h>
#include <ftk/UITest/TabWidgetTest.h>
#include <ftk/UITest/TextEditModelTest.h>
#include <ftk/UITest/WidgetOptionsTest.h>

#if defined(FTK_API_GL_4_1) || defined(FTK_API_GLES_2)
#include <ftk/GLTest/MeshTest.h>
#include <ftk/GLTest/OffscreenBufferTest.h>
#include <ftk/GLTest/TextureAtlasTest.h>
#include <ftk/GLTest/TextureTest.h>
#include <ftk/GLTest/RenderTest.h>
#include <ftk/GLTest/ShaderTest.h>
#include <ftk/GLTest/UtilTest.h>
#include <ftk/GLTest/WindowTest.h>
#endif // FTK_API_GL_4_1

#include <ftk/CoreTest/AppTest.h>
#include <ftk/CoreTest/BoxPackTest.h>
#include <ftk/CoreTest/BoxTest.h>
#include <ftk/CoreTest/CmdLineTest.h>
#include <ftk/CoreTest/ColorTest.h>
#include <ftk/CoreTest/CommandTest.h>
#include <ftk/CoreTest/ErrorTest.h>
#include <ftk/CoreTest/FileIOTest.h>
#include <ftk/CoreTest/FontSystemTest.h>
#include <ftk/CoreTest/FormatTest.h>
#include <ftk/CoreTest/ImageIOTest.h>
#include <ftk/CoreTest/ImageTest.h>
#include <ftk/CoreTest/LRUCacheTest.h>
#include <ftk/CoreTest/MathTest.h>
#include <ftk/CoreTest/MatrixTest.h>
#include <ftk/CoreTest/MemoryTest.h>
#include <ftk/CoreTest/MeshTest.h>
#include <ftk/CoreTest/NoiseTest.h>
#include <ftk/CoreTest/OSTest.h>
#include <ftk/CoreTest/ObservableTest.h>
#include <ftk/CoreTest/PNGTest.h>
#include <ftk/CoreTest/PathTest.h>
#include <ftk/CoreTest/RandomTest.h>
#include <ftk/CoreTest/RangeTest.h>
#include <ftk/CoreTest/RenderOptionsTest.h>
#include <ftk/CoreTest/RenderUtilTest.h>
#include <ftk/CoreTest/SizeTest.h>
#include <ftk/CoreTest/StringTest.h>
#include <ftk/CoreTest/SystemTest.h>
#include <ftk/CoreTest/TimeTest.h>
#include <ftk/CoreTest/TimerTest.h>
#include <ftk/CoreTest/VectorTest.h>

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
            std::shared_ptr<CmdLineListArg<std::string> > testNames;
            std::vector<std::shared_ptr<test::ITest> > tests;
            std::chrono::steady_clock::time_point startTime;
        };
        
        void App::_init(
            const std::shared_ptr<Context>& context,
            std::vector<std::string>& argv)
        {
            FTK_P();
            p.testNames = CmdLineListArg<std::string>::create(
                "Test",
                "Names of the tests to run.",
                true);
            IApp::_init(
                context,
                argv,
                "ftk-test",
                "Test application",
                { p.testNames });
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
            const auto& cmdLineTests = p.testNames->getList();
            if (!cmdLineTests.empty())
            {
                for (const auto& test : cmdLineTests)
                {
                    const auto i = std::find_if(
                        p.tests.begin(),
                        p.tests.end(),
                        [test](const std::shared_ptr<test::ITest>& other)
                        {
                            return contains(other->getName(), test, CaseCompare::Insensitive);
                        });
                    if (i != p.tests.end())
                    {
                        runTests.push_back(*i);
                    }
                }
            }
            else
            {
                for (const auto& test : p.tests)
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

