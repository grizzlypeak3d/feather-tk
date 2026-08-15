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

#include <fstream>

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
            _selection();
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
                FTK_CHECK(path == model->getPath());
                view->reload();
                FileBrowserOptions options;
                options.dirList.sortReverse = true;
                model->setOptions(options);
                model->setOptions(options);
                FTK_CHECK(options == model->getOptions());
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

        void FileBrowserTest::_selection()
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

                // A directory of our own, so that what is selected can be
                // counted against what is there.
                const std::filesystem::path path =
                    std::filesystem::temp_directory_path() / "ftkFileBrowserSelectionTest";
                std::filesystem::remove_all(path);
                std::filesystem::create_directories(path);
                // Named rather than numbered: files that differ only by a
                // number are one image sequence to the listing, and would be
                // one thing to select rather than four.
                const std::vector<std::string> fileNames =
                {
                    "alpha.txt",
                    "beta.txt",
                    "gamma.txt",
                    "delta.txt"
                };
                const size_t fileCount = fileNames.size();
                for (const auto& fileName : fileNames)
                {
                    std::ofstream(path / fileName);
                }

                auto model = FileBrowserModel::create(_context);
                auto view = FileBrowserView::create(
                    _context, FileBrowserMode::Open, model, window);
                model->setPath(path);
                view->reload();
                app->tick();

                std::vector<Path> selection;
                view->setSelectCallback(
                    [&selection](const std::vector<Path>& value)
                    {
                        selection = value;
                    });

                FTK_CHECK(!view->isMultiple());
                view->setMultiple(true);
                FTK_CHECK(view->isMultiple());

                // The first one.
                KeyEvent key(Key::Home, 0, V2I());
                view->keyPressEvent(key);
                FTK_CHECK(1 == view->getSelection().size());
                FTK_CHECK(1 == selection.size());

                // Shift extends from it rather than replacing it.
                key = KeyEvent(Key::Down, static_cast<int>(KeyModifier::Shift), V2I());
                view->keyPressEvent(key);
                FTK_CHECK(2 == view->getSelection().size());
                FTK_CHECK(2 == selection.size());

                // And to the end of the listing.
                key = KeyEvent(Key::End, static_cast<int>(KeyModifier::Shift), V2I());
                view->keyPressEvent(key);
                FTK_CHECK(fileCount == view->getSelection().size());

                // Without the shift it is one again.
                key = KeyEvent(Key::Home, 0, V2I());
                view->keyPressEvent(key);
                FTK_CHECK(1 == view->getSelection().size());

                // What was selected while several were allowed cannot be
                // handed to something that takes one.
                key = KeyEvent(Key::End, static_cast<int>(KeyModifier::Shift), V2I());
                view->keyPressEvent(key);
                FTK_CHECK(fileCount == view->getSelection().size());
                view->setMultiple(false);
                FTK_CHECK(1 == view->getSelection().size());

                std::filesystem::remove_all(path);
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
                    path,
                    FileBrowserMode::Open,
                    model,
                    window);
                FileBrowserOptions options;
                options.dirList.sortReverse = true;
                model->setOptions(options);
                model->setOptions(options);
                FTK_CHECK(options == model->getOptions());
                auto recentFilesModel = RecentFilesModel::create(_context);
                fileBrowserWidget->setRecentFilesModel(recentFilesModel);
                fileBrowserWidget->setCallback(
                    [](const std::vector<Path>&)
                    {
                    });
                fileBrowserWidget->setMultiple(true);
                FTK_CHECK(fileBrowserWidget->isMultiple());
                fileBrowserWidget->setMultiple(false);
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
                FTK_CHECK(model->getOptions() == options);
                auto recentFilesModel = RecentFilesModel::create(_context);
                fileBrowser->setRecentFilesModel(recentFilesModel);
                FTK_CHECK(recentFilesModel == fileBrowser->getRecentFilesModel());
                fileBrowser->setCallback(
                    [](const std::vector<Path>&)
                    {
                    });
                fileBrowser->setMultiple(true);
                FTK_CHECK(fileBrowser->isMultiple());
                fileBrowser->setMultiple(false);
                bool close = false;
                fileBrowser->setCloseCallback(
                    [&close]
                    {
                        close = true;
                    });

                fileBrowser->open(window);
                app->tick();
                FTK_CHECK(fileBrowser->isOpen());
                fileBrowser->close();
                app->tick();
                FTK_CHECK(!fileBrowser->isOpen());
            }
        }
    }
}

