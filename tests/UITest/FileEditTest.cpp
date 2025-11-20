// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/FileEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/FileEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        FileEditTest::FileEditTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FileEditTest")
        {}

        FileEditTest::~FileEditTest()
        {}

        std::shared_ptr<FileEditTest> FileEditTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FileEditTest>(new FileEditTest(context));
        }

        void FileEditTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("FileEditTest");
                auto app = App::create(
                    context,
                    argv,
                    "FileEditTest",
                    "File edit test.");
                auto window = Window::create(context, app, "FileEditTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = FileEdit::create(context, layout);
                Path path(std::filesystem::current_path().u8string());
                edit->setPath(path);
                edit->setPath(path);
                FTK_ASSERT(path == edit->getPath());
                edit->setCallback(
                    [&path](const Path& value)
                    {
                        path = value;
                    });

                auto system = context->getSystem<FileBrowserSystem>();
                system->setNativeFileDialog(false);
                FTK_ASSERT(!system->isNativeFileDialog());
            }
        }
    }
}

