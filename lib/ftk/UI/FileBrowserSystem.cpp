// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FileBrowserWidgets.h>
#include <ftk/UI/RecentFilesModel.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Path.h>
#include <ftk/Core/Timer.h>

#if defined(FTK_NFD)
#include <nfd.hpp>
#endif // FTK_NFD

#include <nlohmann/json.hpp>

#include <sstream>

namespace ftk
{
    struct FileBrowserSystem::Private
    {
        bool native = true;
        bool floating = false;
        std::shared_ptr<FileBrowserModel> model;
        std::shared_ptr<RecentFilesModel> recentFilesModel;
        std::shared_ptr<IFileBrowserThumbnails> thumbnails;

        std::shared_ptr<FileBrowser> fileBrowser;

        std::shared_ptr<Window> window;
        std::shared_ptr<FileBrowserWidget> widget;
        //! The window being let go of, and the timer that lets go. See
        //! the close callback in _openWindow().
        std::shared_ptr<Window> closing;
        std::shared_ptr<Timer> closeTimer;
    };

    FileBrowserSystem::FileBrowserSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::FileBrowserSystem"),
        _p(new Private)
    {
        FTK_P();

        p.model = FileBrowserModel::create(context);
        p.recentFilesModel = RecentFilesModel::create(context);
        p.closeTimer = Timer::create(context);

#if defined(FTK_NFD)
        NFD::Init();
#endif // FTK_NFD
    }

    FileBrowserSystem::~FileBrowserSystem()
    {
        FTK_P();
#if defined(FTK_NFD)
        NFD::Quit();
#endif // FTK_NFD
    }

    std::shared_ptr<FileBrowserSystem> FileBrowserSystem::create(
        const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<FileBrowserSystem>(new FileBrowserSystem(context));
    }

    void FileBrowserSystem::open(
        const std::shared_ptr<IWindow>& window,
        const std::function<void(const Path&)>& callback,
        const FileBrowserOpenOptions& options)
    {
        // The first of them: a caller that takes one file gets one whatever
        // the options say, rather than silently losing the rest.
        open(
            window,
            [callback](const std::vector<Path>& value)
            {
                if (!value.empty())
                {
                    callback(value.front());
                }
            },
            options);
    }

    void FileBrowserSystem::open(
        const std::shared_ptr<IWindow>& window,
        const std::function<void(const std::vector<Path>&)>& callback,
        const FileBrowserOpenOptions& options)
    {
        FTK_P();
        bool native = p.native;
#if defined(FTK_NFD)
        if (native)
        {
            // Build a single native filter group from the extensions. NFD wants
            // a comma-separated spec without leading dots, e.g. "djvr" or
            // "png,jpg".
            std::string spec;
            for (const auto& ext : options.extensions)
            {
                std::string e = ext;
                if (!e.empty() && '.' == e.front())
                {
                    e.erase(0, 1);
                }
                if (e.empty())
                {
                    continue;
                }
                if (!spec.empty())
                {
                    spec += ",";
                }
                spec += e;
            }
            std::vector<nfdu8filteritem_t> filterItems;
            if (!spec.empty())
            {
                filterItems.push_back({ options.extensionsLabel.c_str(), spec.c_str() });
            }
            const nfdu8filteritem_t* filterList = filterItems.empty() ? nullptr : filterItems.data();
            const nfdfiltersize_t filterCount = static_cast<nfdfiltersize_t>(filterItems.size());
            const std::string defaultPathStr = options.path.u8string();
            const nfdu8char_t* defaultPath = defaultPathStr.empty() ? nullptr : defaultPathStr.c_str();

            // Opening more than one is its own call and returns a path set
            // rather than a path, so it is handled apart from the rest.
            if (FileBrowserMode::Open == options.mode && options.multiple)
            {
                const nfdpathset_t* outPaths = nullptr;
                if (NFD_OKAY == NFD::OpenDialogMultiple(
                    outPaths, filterList, filterCount, defaultPath))
                {
                    std::vector<Path> paths;
                    nfdpathsetsize_t count = 0;
                    NFD::PathSet::Count(outPaths, count);
                    for (nfdpathsetsize_t i = 0; i < count; ++i)
                    {
                        nfdu8char_t* outPath = nullptr;
                        if (NFD_OKAY == NFD::PathSet::GetPath(outPaths, i, outPath))
                        {
                            paths.push_back(Path(std::string(outPath)));
                            NFD::PathSet::FreePath(outPath);
                        }
                    }
                    NFD::PathSet::Free(outPaths);
                    if (!paths.empty())
                    {
                        callback(paths);
                    }
                }
            }
            else
            {
                nfdu8char_t* outPath = nullptr;
                switch (options.mode)
                {
                case FileBrowserMode::Open:
                    NFD::OpenDialog(outPath, filterList, filterCount, defaultPath);
                    break;
                case FileBrowserMode::Save:
                    NFD::SaveDialog(outPath, filterList, filterCount, defaultPath);
                    break;
                case FileBrowserMode::Dir:
                    NFD::PickFolder(outPath, defaultPath);
                    break;
                default: break;
                }
                if (outPath)
                {
                    callback({ Path(std::string(outPath)) });
                    NFD::FreePath(outPath);
                }
            }
        }
#else  // FTK_NFD
        native = false;
#endif  // FTK_NFD
        if (!native)
        {
            if (auto context = _context.lock())
            {
                // When a filter is given, use a dedicated model so the shared
                // model (configured elsewhere, e.g. for media) is left untouched.
                std::shared_ptr<FileBrowserModel> model = p.model;
                if (!options.extensions.empty())
                {
                    model = FileBrowserModel::create(context);
                    model->setOptions(p.model->getOptions());
                    // Carry the shared model's location across: a fresh one
                    // starts at the working directory, which is not where the
                    // user was.
                    model->setPath(p.model->getPath());
                    model->setExtsFilter(options.extensions, options.extensionsLabel);
                }

                // A window of its own needs the application to make one. A
                // window that has outlived its application has none, and the
                // dialog is the answer that always works.
                std::shared_ptr<App> app;
                if (p.floating && window)
                {
                    app = window->getApp();
                }
                if (app)
                {
                    _openWindow(context, app, callback, options, model);
                }
                else
                {
                    _openDialog(context, window, callback, options, model);
                }
            }
        }
    }

    void FileBrowserSystem::_openDialog(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWindow>& window,
        const std::function<void(const std::vector<Path>&)>& callback,
        const FileBrowserOpenOptions& options,
        const std::shared_ptr<FileBrowserModel>& model)
    {
        FTK_P();
        p.fileBrowser = FileBrowser::create(
            context,
            options.title,
            options.path,
            options.mode,
            model);
        p.fileBrowser->setTitle(options.title);
        p.fileBrowser->setMultiple(
            FileBrowserMode::Open == options.mode && options.multiple);
        p.fileBrowser->setRecentFilesModel(p.recentFilesModel);
        p.fileBrowser->open(window);
        p.fileBrowser->setCallback(
            [this, callback](const std::vector<Path>& value)
            {
                callback(value);
                _p->fileBrowser->close();
            });
        p.fileBrowser->setCloseCallback(
            [this]
            {
                _p->fileBrowser.reset();
            });
    }

    void FileBrowserSystem::_openWindow(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::function<void(const std::vector<Path>&)>& callback,
        const FileBrowserOpenOptions& options,
        const std::shared_ptr<FileBrowserModel>& model)
    {
        FTK_P();

        // One at a time, the way the dialog is one at a time. Opening the
        // browser again while it is up replaces it rather than leaving two
        // windows answering different callbacks.
        close();

        p.window = Window::create(context, app, options.title, Size2I(1024, 720));
        p.widget = FileBrowserWidget::create(
            context,
            options.path,
            options.mode,
            model,
            p.window);
        p.widget->setMultiple(
            FileBrowserMode::Open == options.mode && options.multiple);
        p.widget->setRecentFilesModel(p.recentFilesModel);
        p.widget->setCallback(
            [this, callback](const std::vector<Path>& value)
            {
                callback(value);
                close();
            });
        p.widget->setCancelCallback(
            [this]
            {
                close();
            });
        p.window->setCloseCallback(
            [this]
            {
                FTK_P();
                // Freeing the window here would take the widget whose
                // callback is running down with it, and the OpenGL context
                // with that, from inside that window's own event handling.
                // So it is held until the next tick and let go of there.
                p.closing = p.window;
                p.window.reset();
                p.widget.reset();
                p.closeTimer->start(
                    std::chrono::milliseconds(0),
                    [this] { _p->closing.reset(); });
            });
        p.window->show();

        // The dialog opens with the file list focused; a window of its own
        // has to be told.
        p.widget->getView()->takeKeyFocus();
    }

    void FileBrowserSystem::close()
    {
        FTK_P();
        if (auto window = p.window)
        {
            // The close callback does the letting go.
            window->close();
        }
    }

    bool FileBrowserSystem::isFloating() const
    {
        return _p->floating;
    }

    void FileBrowserSystem::setFloating(bool value)
    {
        _p->floating = value;
    }

    bool FileBrowserSystem::isNativeFileDialog() const
    {
        return _p->native;
    }

    void FileBrowserSystem::setNativeFileDialog(bool value)
    {
        _p->native = value;
    }

    const std::shared_ptr<FileBrowserModel>& FileBrowserSystem::getModel() const
    {
        return _p->model;
    }

    const std::shared_ptr<RecentFilesModel>& FileBrowserSystem::getRecentFilesModel() const
    {
        return _p->recentFilesModel;
    }

    void FileBrowserSystem::setRecentFilesModel(const std::shared_ptr<RecentFilesModel>& value)
    {
        _p->recentFilesModel = value;
    }

    const std::shared_ptr<IFileBrowserThumbnails>& FileBrowserSystem::getThumbnails() const
    {
        return _p->thumbnails;
    }

    void FileBrowserSystem::setThumbnails(const std::shared_ptr<IFileBrowserThumbnails>& value)
    {
        _p->thumbnails = value;
    }
}
