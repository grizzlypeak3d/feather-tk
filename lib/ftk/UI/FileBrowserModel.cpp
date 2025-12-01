// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowser.h>

namespace ftk
{
    struct FileBrowserModel::Private
    {
        std::vector<std::filesystem::path> paths;
        int currentPath = -1;
        std::shared_ptr<Observable<std::filesystem::path> > path;
        std::shared_ptr<Observable<bool> > hasForward;
        std::shared_ptr<Observable<bool> > hasBack;
        std::shared_ptr<Observable<FileBrowserOptions> > options;
        std::shared_ptr<ObservableList<std::string> > exts;
        std::shared_ptr<Observable<std::string> > ext;
    };

    FileBrowserModel::FileBrowserModel(const std::shared_ptr<Context>&) :
        _p(new Private)
    {
        FTK_P();
        const auto path = std::filesystem::current_path().u8string();
        p.paths.push_back(path);
        p.currentPath = 0;
        p.path = Observable<std::filesystem::path>::create(path);
        p.hasForward = Observable<bool>::create(false);
        p.hasBack = Observable<bool>::create(false);
        p.options = Observable<FileBrowserOptions>::create();
        p.exts = ObservableList<std::string>::create();
        p.ext = Observable<std::string>::create();
    }

    FileBrowserModel::~FileBrowserModel()
    {}

    std::shared_ptr<FileBrowserModel> FileBrowserModel::create(const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<FileBrowserModel>(new FileBrowserModel(context));
    }

    const std::filesystem::path& FileBrowserModel::getPath() const
    {
        return _p->path->get();
    }

    std::shared_ptr<IObservable<std::filesystem::path> > FileBrowserModel::observePath() const
    {
        return _p->path;
    }

    void FileBrowserModel::setPath(const std::filesystem::path& value)
    {
        FTK_P();
        while (p.currentPath < static_cast<int>(p.paths.size()) - 1)
        {
            p.paths.pop_back();
        }
        if (p.paths.empty() ||
            (!p.paths.empty() && value != p.paths[p.paths.size() - 1]))
        {
            p.paths.push_back(value);
            p.currentPath++;

            p.path->setIfChanged(value);
            p.hasForward->setIfChanged(p.currentPath < p.paths.size() - 1);
            p.hasBack->setIfChanged(p.currentPath > 0);
        }
    }

    void FileBrowserModel::forward()
    {
        FTK_P();
        if (!p.paths.empty() && p.currentPath < p.paths.size() - 1)
        {
            p.currentPath = p.currentPath + 1;
            p.path->setIfChanged(p.paths[p.currentPath]);
            p.hasForward->setIfChanged(p.currentPath < p.paths.size() - 1);
            p.hasBack->setIfChanged(p.currentPath > 0);
        }
    }

    std::shared_ptr<IObservable<bool> > FileBrowserModel::observeHasForward() const
    {
        return _p->hasForward;
    }

    void FileBrowserModel::back()
    {
        FTK_P();
        if (p.currentPath > 0)
        {
            p.currentPath = p.currentPath - 1;
            p.path->setIfChanged(p.paths[p.currentPath]);
            p.hasForward->setIfChanged(p.currentPath < p.paths.size() - 1);
            p.hasBack->setIfChanged(p.currentPath > 0);
        }
    }

    std::shared_ptr<IObservable<bool> > FileBrowserModel::observeHasBack() const
    {
        return _p->hasBack;
    }

    const FileBrowserOptions& FileBrowserModel::getOptions() const
    {
        return _p->options->get();
    }

    std::shared_ptr<IObservable<FileBrowserOptions> > FileBrowserModel::observeOptions() const
    {
        return _p->options;
    }

    void FileBrowserModel::setOptions(const FileBrowserOptions& value)
    {
        _p->options->setIfChanged(value);
    }

    const std::vector<std::string>& FileBrowserModel::getExts() const
    {
        return _p->exts->get();
    }

    std::shared_ptr<IObservableList<std::string> > FileBrowserModel::observeExts() const
    {
        return _p->exts;
    }

    void FileBrowserModel::setExts(const std::vector<std::string>& value)
    {
        _p->exts->setIfChanged(value);
    }

    const std::string& FileBrowserModel::getExt() const
    {
        return _p->ext->get();
    }

    std::shared_ptr<IObservable<std::string> > FileBrowserModel::observeExt() const
    {
        return _p->ext;
    }

    void FileBrowserModel::setExt(const std::string& value)
    {
        _p->ext->setIfChanged(value);
    }
}