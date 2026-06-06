// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FileBrowserTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FileBrowserPrivate.h>
#include <ftk/UI/RecentFilesModel.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Time.h>

namespace ftk
{
    namespace ui_test
    {
        FileBrowserTest::FileBrowserTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FileBrowserTest")
        {}

        FileBrowserTest::~FileBrowserTest()
        {}

        std::shared_ptr<FileBrowserTest> FileBrowserTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FileBrowserTest>(new FileBrowserTest(context));
        }
                
        void FileBrowserTest::run()
        {
            _shortcuts();
            _view();
            _widget();
            _dialog();
        }

        void FileBrowserTest::_shortcuts()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("FileBrowserTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FileBrowserTest",
                    "File browser test.");
                auto window = Window::create(_context, app, "FileBrowserTest");
                window->show();
                app->tick();

                auto model = FileBrowserModel::create(_context);
                auto panel = FileBrowserPanel::create(_context, model, window);
                auto recentFilesModel = RecentFilesModel::create(_context);
                recentFilesModel->addRecent(std::filesystem::current_path());
                panel->setRecentFilesModel(recentFilesModel);
            }
        }

        void FileBrowserTest::_view()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("FileBrowserTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FileBrowserTest",
                    "File browser test.");
                auto window = Window::create(_context, app, "FileBrowserTest");
                window->show();
                app->tick();

                auto model = FileBrowserModel::create(_context);
                auto view = FileBrowserView::create(_context, FileBrowserMode::Open, model, window);
                auto path = std::filesystem::current_path();
                model->setPath(path);
                model->setPath(path);
                FTK_ASSERT(path == model->getPath());
                view->reload();
                FileBrowserOptions options;
                options.dirList.sortReverse = true;
                model->setOptions(options);
                model->setOptions(options);
                FTK_ASSERT(options == model->getOptions());
                view->setCallback(
                    [](const Path&)
                    {
                    });

                options.dirList.sort = DirListSort::Extension;
                model->setOptions(options);
                app->tick();
                options.dirList.sort = DirListSort::Size;
                model->setOptions(options);
                app->tick();
                options.dirList.sort = DirListSort::Time;
                model->setOptions(options);
                app->tick();
            }
        }

        void FileBrowserTest::_widget()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("FileBrowserTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FileBrowserTest",
                    "File browser test.");
                auto window = Window::create(_context, app, "FileBrowserTest");
                window->show();
                app->tick();

                auto path = std::filesystem::current_path();
                auto model = FileBrowserModel::create(_context);
                auto fileBrowserWidget = FileBrowserWidget::create(
                    _context,
                    "Open",
                    path,
                    FileBrowserMode::Open,
                    model,
                    window);
                FileBrowserOptions options;
                options.dirList.sortReverse = true;
                model->setOptions(options);
                model->setOptions(options);
                FTK_ASSERT(options == model->getOptions());
                auto recentFilesModel = RecentFilesModel::create(_context);
                fileBrowserWidget->setRecentFilesModel(recentFilesModel);
                fileBrowserWidget->setCallback(
                    [](const Path&)
                    {
                    });
                bool cancel = false;
                fileBrowserWidget->setCancelCallback(
                    [&cancel]
                    {
                        cancel = true;
                    });
            }
        }

        void FileBrowserTest::_dialog()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("FileBrowserTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FileBrowserTest",
                    "File browser test.");
                auto window = Window::create(_context, app, "FileBrowserTest");
                window->show();
                app->tick();

                auto path = std::filesystem::current_path();
                auto model = FileBrowserModel::create(_context);
                auto fileBrowser = FileBrowser::create(
                    _context,
                    "Open",
                    path,
                    FileBrowserMode::Open,
                    model);
                FileBrowserOptions options;
                options.dirList.sortReverse = true;
                model->setOptions(options);
                FTK_ASSERT(model->getOptions() == options);
                auto recentFilesModel = RecentFilesModel::create(_context);
                fileBrowser->setRecentFilesModel(recentFilesModel);
                FTK_ASSERT(recentFilesModel == fileBrowser->getRecentFilesModel());
                fileBrowser->setCallback(
                    [](const Path&)
                    {
                    });
                bool close = false;
                fileBrowser->setCloseCallback(
                    [&close]
                    {
                        close = true;
                    });

                fileBrowser->open(window);
                app->tick();
                FTK_ASSERT(fileBrowser->isOpen());
                fileBrowser->close();
                app->tick();
                FTK_ASSERT(!fileBrowser->isOpen());
            }
        }
    }
}

