// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/RecentFilesModel.h>

#include <ftk/Core/Path.h>

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
        std::shared_ptr<FileBrowserModel> model;
        std::shared_ptr<RecentFilesModel> recentFilesModel;

        std::shared_ptr<FileBrowser> fileBrowser;
    };

    FileBrowserSystem::FileBrowserSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::FileBrowserSystem"),
        _p(new Private)
    {
        FTK_P();

        p.model = FileBrowserModel::create(context);
        p.recentFilesModel = RecentFilesModel::create(context);

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
        }
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
}
