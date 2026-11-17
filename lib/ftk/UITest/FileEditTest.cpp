// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FileEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/FileEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Path.h>

#include <tuple>

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
            {
                std::vector<std::string> argv;
                argv.push_back("FileEditTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FileEditTest",
                    "File edit test.");
                auto window = Window::create(_context, app, "FileEditTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = FileEdit::create(_context, layout);
                Path path(fromFileSystem(std::filesystem::current_path()));
                edit->setPath(path);
                edit->setPath(path);
                FTK_CHECK(path == edit->getPath());
                edit->setCallback(
                    [&path](const Path& value)
                    {
                        path = value;
                    });

                // The name until the field is edited, then the whole path; the
                // whole path in the tooltip throughout. A directory as well
                // as a file.
                for (const auto& [mode, value, name] : {
                    std::make_tuple(FileBrowserMode::Open, "/tmp/renders/shot010.exr", "shot010.exr"),
                    std::make_tuple(FileBrowserMode::Dir, "/tmp/renders", "renders"),
                    std::make_tuple(FileBrowserMode::Dir, "/tmp/renders/", "renders"),
                    std::make_tuple(FileBrowserMode::Dir, "/", "/") })
                {
                    auto edit2 = FileEdit::create(_context, mode, layout);
                    const Path path2(value);
                    edit2->setPath(path2);
                    app->tick();
                    FTK_CHECK(std::string(name) == edit2->getLineEdit()->getText());
                    FTK_CHECK(path2.get() == edit2->getLineEdit()->getTooltip());
                    edit2->getLineEdit()->takeKeyFocus();
                    app->tick();
                    FTK_CHECK(path2.get() == edit2->getLineEdit()->getText());
                    edit2->getLineEdit()->releaseKeyFocus();
                    app->tick();
                    FTK_CHECK(std::string(name) == edit2->getLineEdit()->getText());
                }

                auto system = _context->getSystem<FileBrowserSystem>();
                system->setNativeFileDialog(false);
                FTK_CHECK(!system->isNativeFileDialog());
            }
        }
    }
}

