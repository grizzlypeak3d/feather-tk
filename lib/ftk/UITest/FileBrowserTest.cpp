// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FileBrowserTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FileBrowserPrivate.h>
#include <ftk/UI/RecentFilesModel.h>
#include <ftk/UI/PushButton.h>
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
            _floating();
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
                recentFilesModel->addRecent(Path(std::filesystem::current_path().u8string()));
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
                    [](const std::vector<Path>&)
                    {
                    });

                // Every thumbnail size, since each one changes how large the
                // rows are and what is asked of the thumbnails.
                for (auto thumbnails : getFileBrowserThumbnailsEnums())
                {
                    options.thumbnails = thumbnails;
                    model->setOptions(options);
                    app->tick();
                    FTK_CHECK(thumbnails == model->getOptions().thumbnails);

                    // And survives being written down.
                    nlohmann::json json;
                    to_json(json, options);
                    FileBrowserOptions out;
                    from_json(json, out);
                    FTK_CHECK(options == out);
                }
                options.thumbnails = FileBrowserThumbnails::Medium;
                model->setOptions(options);

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

                // And the whole way through the widget: what the view has
                // selected is what the Ok button hands back.
                {
                    auto widget = FileBrowserWidget::create(
                        _context, path, FileBrowserMode::Open, model, window);
                    widget->setMultiple(true);
                    std::vector<Path> accepted;
                    widget->setCallback(
                        [&accepted](const std::vector<Path>& value)
                        {
                            accepted = value;
                        });
                    app->tick();

                    auto widgetView = widget->getView();
                    KeyEvent k(Key::Home, 0, V2I());
                    widgetView->keyPressEvent(k);
                    k = KeyEvent(Key::End, static_cast<int>(KeyModifier::Shift), V2I());
                    widgetView->keyPressEvent(k);
                    FTK_CHECK(fileCount == widgetView->getSelection().size());

                    _click(widget, "Ok");
                    FTK_CHECK(fileCount == accepted.size());
                }

                // Chosen with the mouse rather than the keyboard, which is
                // how it is actually done: shift extends from the last click.
                {
                    auto widget = FileBrowserWidget::create(
                        _context, path, FileBrowserMode::Open, model, window);
                    widget->setMultiple(true);
                    std::vector<Path> accepted;
                    widget->setCallback(
                        [&accepted](const std::vector<Path>& value)
                        {
                            accepted = value;
                        });
                    window->setGeometry(Box2I(0, 0, 1280, 960));
                    app->tick();

                    auto widgetView = widget->getView();
                    auto pos = [widgetView](int index)
                    {
                        const Box2I r = widgetView->getRect(index);
                        return widgetView->getGeometry().min +
                            V2I(r.min.x + 1, r.min.y + 1);
                    };

                    MouseClickEvent press(MouseButton::Left, 0, pos(0));
                    widgetView->mousePressEvent(press);
                    MouseClickEvent release(MouseButton::Left, 0, pos(0));
                    widgetView->mouseReleaseEvent(release);
                    FTK_CHECK(1 == widgetView->getSelection().size());

                    const int shift = static_cast<int>(KeyModifier::Shift);
                    press = MouseClickEvent(MouseButton::Left, shift, pos(3));
                    widgetView->mousePressEvent(press);
                    release = MouseClickEvent(MouseButton::Left, shift, pos(3));
                    widgetView->mouseReleaseEvent(release);
                    FTK_CHECK(fileCount == widgetView->getSelection().size());

                    _click(widget, "Ok");
                    FTK_CHECK(fileCount == accepted.size());
                }

                // Added to one at a time, and opened with the keyboard: the
                // Ok button is not the only way out of the browser, and the
                // other ways were handing back the current item alone.
                {
                    auto widget = FileBrowserWidget::create(
                        _context, path, FileBrowserMode::Open, model, window);
                    widget->setMultiple(true);
                    std::vector<Path> accepted;
                    widget->setCallback(
                        [&accepted](const std::vector<Path>& value)
                        {
                            accepted = value;
                        });
                    window->setGeometry(Box2I(0, 0, 1280, 960));
                    app->tick();

                    auto widgetView = widget->getView();
                    auto pos = [widgetView](int index)
                    {
                        const Box2I r = widgetView->getRect(index);
                        return widgetView->getGeometry().min +
                            V2I(r.min.x + 1, r.min.y + 1);
                    };

                    // The command key adds rather than replaces.
                    const int command = static_cast<int>(commandKeyModifier);
                    for (size_t i = 0; i < fileCount; ++i)
                    {
                        MouseClickEvent press(
                            MouseButton::Left, command, pos(static_cast<int>(i)));
                        widgetView->mousePressEvent(press);
                        MouseClickEvent release(
                            MouseButton::Left, command, pos(static_cast<int>(i)));
                        widgetView->mouseReleaseEvent(release);
                    }
                    FTK_CHECK(fileCount == widgetView->getSelection().size());

                    KeyEvent k(Key::Return, 0, V2I());
                    widgetView->keyPressEvent(k);
                    FTK_CHECK(fileCount == accepted.size());

                    // One on its own is still the one opened, whichever it is.
                    accepted.clear();
                    MouseClickEvent press(MouseButton::Left, 0, pos(1));
                    widgetView->mousePressEvent(press);
                    MouseClickEvent release(MouseButton::Left, 0, pos(1));
                    widgetView->mouseReleaseEvent(release);
                    k = KeyEvent(Key::Return, 0, V2I());
                    widgetView->keyPressEvent(k);
                    FTK_CHECK(1 == accepted.size());
                }

                // And the way an application reaches it: through the system,
                // which is where the choice between this browser and the
                // platform's own is made.
                {
                    auto system = _context->getSystem<FileBrowserSystem>();
                    const bool native = system->isNativeFileDialog();
                    system->setNativeFileDialog(false);

                    // Acted on one at a time, and acting reaches back into
                    // the browser: an application that remembers what it
                    // opened is sharing this model with the browser that is
                    // still up.
                    auto recentFilesModel2 = RecentFilesModel::create(_context);
                    system->setRecentFilesModel(recentFilesModel2);
                    std::vector<Path> opened;
                    FileBrowserOpenOptions openOptions;
                    openOptions.multiple = true;
                    openOptions.path = path;
                    system->open(
                        window,
                        [&opened, recentFilesModel2](const std::vector<Path>& value)
                        {
                            for (const auto& i : value)
                            {
                                opened.push_back(i);
                                recentFilesModel2->addRecent(i);
                            }
                        },
                        openOptions);
                    app->tick();

                    auto dialog = _find<FileBrowser>(window);
                    FTK_CHECK(dialog);
                    auto dialogView = std::dynamic_pointer_cast<FileBrowserView>(
                        dialog->getKeyFocus());
                    FTK_CHECK(dialogView);
                    KeyEvent k(Key::Home, 0, V2I());
                    dialogView->keyPressEvent(k);
                    k = KeyEvent(Key::End, static_cast<int>(KeyModifier::Shift), V2I());
                    dialogView->keyPressEvent(k);
                    FTK_CHECK(fileCount == dialogView->getSelection().size());

                    _click(dialog, "Ok");
                    FTK_CHECK(fileCount == opened.size());

                    system->setNativeFileDialog(native);
                }

                std::filesystem::remove_all(path);
            }
        }

        void FileBrowserTest::_floating()
        {
            std::vector<std::string> argv;
            argv.push_back("FileBrowserTest");
            auto app = App::create(
                _context,
                argv,
                "FileBrowserTest",
                "File browser test");
            auto window = Window::create(_context, app, "FileBrowserTest");
            window->show();
            app->tick();

            const std::filesystem::path path =
                std::filesystem::temp_directory_path() / "FileBrowserFloating";
            std::filesystem::remove_all(path);
            std::filesystem::create_directory(path);
            {
                std::ofstream os(path / "file.txt");
                os << "file" << std::endl;
            }

            auto system = _context->getSystem<FileBrowserSystem>();
            const bool native = system->isNativeFileDialog();
            const bool floating = system->isFloating();
            system->setNativeFileDialog(false);
            system->setFloating(true);

            std::vector<Path> opened;
            FileBrowserOpenOptions openOptions;
            openOptions.path = path;
            system->open(
                window,
                [&opened](const std::vector<Path>& value)
                {
                    opened = value;
                },
                openOptions);
            app->tick();

            // A window of its own, rather than something covering the one it
            // was opened from: that window is untouched and there is now a
            // second one.
            FTK_CHECK(!_find<FileBrowser>(window));
            FTK_CHECK(2 == app->getWindows().size());
            auto browserWindow = app->getWindows().back();
            FTK_CHECK(browserWindow != window);
            auto widget = _find<FileBrowserWidget>(browserWindow);
            FTK_CHECK(widget);

            // Choosing reports the file and takes the window away again.
            auto view = widget->getView();
            KeyEvent k(Key::Home, 0, V2I());
            view->keyPressEvent(k);
            _click(browserWindow, "Ok");
            FTK_CHECK(1 == opened.size());
            app->tick();
            FTK_CHECK(1 == app->getWindows().size());

            // Closing it without choosing reports nothing and does the same.
            opened.clear();
            system->open(
                window,
                [&opened](const std::vector<Path>& value)
                {
                    opened = value;
                },
                openOptions);
            app->tick();
            FTK_CHECK(2 == app->getWindows().size());
            system->close();
            app->tick();
            FTK_CHECK(opened.empty());
            FTK_CHECK(1 == app->getWindows().size());

            // Closing one that is not open is not an error: an application
            // does this when its own window goes away.
            system->close();
            app->tick();
            FTK_CHECK(1 == app->getWindows().size());

            system->setFloating(floating);
            system->setNativeFileDialog(native);
            std::filesystem::remove_all(path);
        }

        void FileBrowserTest::_click(
            const std::shared_ptr<IWidget>& widget,
            const std::string& text)
        {
            if (auto button = std::dynamic_pointer_cast<PushButton>(widget))
            {
                if (text == button->getText())
                {
                    button->click();
                    return;
                }
            }
            for (const auto& child : widget->getChildren())
            {
                _click(child, text);
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

