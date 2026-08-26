// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/FileBrowserPrivate.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <array>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        FileBrowserMode,
        "File",
        "Dir");

    FTK_ENUM_IMPL(
        FileBrowserThumbnails,
        "Off",
        "Small",
        "Medium",
        "Large");

    int getThumbnailHeight(FileBrowserThumbnails value, int sizeRole)
    {
        // The style says how large a thumbnail is, and this says how much of
        // one is wanted, so a theme that wants them bigger moves all three.
        int out = 0;
        switch (value)
        {
        case FileBrowserThumbnails::Small:  out = sizeRole / 2; break;
        case FileBrowserThumbnails::Medium: out = sizeRole;     break;
        case FileBrowserThumbnails::Large:  out = sizeRole * 2; break;
        default: break;
        }
        return out;
    }

    bool FileBrowserOptions::operator == (const FileBrowserOptions& other) const
    {
        return
            dirList == other.dirList &&
            thumbnails == other.thumbnails &&
            panel == other.panel &&
            pathEditable == other.pathEditable &&
            bellows == other.bellows;
    }

    bool FileBrowserOptions::operator != (const FileBrowserOptions& other) const
    {
        return !(*this == other);
    }

    IFileBrowserThumbnails::~IFileBrowserThumbnails()
    {}

    struct FileBrowser::Private
    {
        std::shared_ptr<FileBrowserWidget> widget;
    };

    void FileBrowser::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::filesystem::path& path,
        FileBrowserMode mode,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IDialog::_init(context, "ftk::FileBrowser", parent);
        FTK_P();

        setTitle(title);

        p.widget = FileBrowserWidget::create(
            context,
            path,
            mode,
            model,
            shared_from_this());

        p.widget->setCancelCallback(
            [this]
            {
                close();
            });
    }

    FileBrowser::FileBrowser() :
        _p(new Private)
    {}

    FileBrowser::~FileBrowser()
    {}

    std::shared_ptr<FileBrowser> FileBrowser::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::filesystem::path& path,
        FileBrowserMode mode,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowser>(new FileBrowser);
        out->_init(context, title, path, mode, model, parent);
        return out;
    }

    void FileBrowser::setCallback(
        const std::function<void(const std::vector<Path>&)>& value)
    {
        _p->widget->setCallback(value);
    }

    std::string FileBrowser::getFileName() const
    {
        return _p->widget->getFileName();
    }

    void FileBrowser::setFileName(const std::string& value)
    {
        _p->widget->setFileName(value);
    }

    bool FileBrowser::isMultiple() const
    {
        return _p->widget->isMultiple();
    }

    void FileBrowser::setMultiple(bool value)
    {
        _p->widget->setMultiple(value);
    }

    const std::shared_ptr<FileBrowserModel>& FileBrowser::getModel() const
    {
        return _p->widget->getModel();
    }

    const std::shared_ptr<RecentFilesModel>& FileBrowser::getRecentFilesModel() const
    {
        return _p->widget->getRecentFilesModel();
    }

    void FileBrowser::setRecentFilesModel(const std::shared_ptr<RecentFilesModel>& value)
    {
        _p->widget->setRecentFilesModel(value);
    }

    std::shared_ptr<IWidget> FileBrowser::getKeyFocus() const
    {
        return _p->widget->getView();
    }

    void to_json(nlohmann::json& json, const FileBrowserOptions& value)
    {
        json["DirList"] = value.dirList;
        json["Thumbnails"] = to_string(value.thumbnails);
        json["Panel"] = value.panel;
        json["PathEditable"] = value.pathEditable;
        for (const auto& i : value.bellows)
        {
            json["Bellows"][i.first] = i.second;
        }
    }

    void from_json(const nlohmann::json& json, FileBrowserOptions& value)
    {
        json.at("DirList").get_to(value.dirList);
        from_string(json.at("Thumbnails").get<std::string>(), value.thumbnails);
        json.at("Panel").get_to(value.panel);
        json.at("PathEditable").get_to(value.pathEditable);
        for (auto i = json.at("Bellows").begin(); i != json.at("Bellows").end(); ++i)
        {
            if (i->is_boolean())
            {
                i->get_to(value.bellows[i.key()]);
            }
        }
    }
}
