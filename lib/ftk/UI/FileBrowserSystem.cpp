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
        const std::string& title,
        const std::filesystem::path& path,
        FileBrowserMode mode)
    {
        FTK_P();
        bool native = p.native;
#if defined(FTK_NFD)
        if (native)
        {
            nfdu8char_t* outPath = nullptr;
            switch (mode)
            {
            case FileBrowserMode::File:
                NFD::OpenDialog(outPath);
                break;
            case FileBrowserMode::Dir:
                NFD::PickFolder(outPath);
                break;
            default: break;
            }
            if (outPath)
            {
                callback(Path(std::string(outPath)));
                NFD::FreePath(outPath);
            }
        }
#else  // FTK_NFD
        native = false;
#endif  // FTK_NFD
        if (!native)
        {
            if (auto context = _context.lock())
            {
                if (!p.fileBrowser)
                {
                    p.fileBrowser = FileBrowser::create(
                        context,
                        title,
                        path,
                        mode,
                        p.model);
                }
                p.fileBrowser->setTitle(title);
                p.fileBrowser->setRecentFilesModel(p.recentFilesModel);
                p.fileBrowser->open(window);
                p.fileBrowser->setCallback(
                    [this, callback](const Path& value)
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
