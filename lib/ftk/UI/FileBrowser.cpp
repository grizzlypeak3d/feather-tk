// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/FileBrowserPrivate.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        FileBrowserMode,
        "File",
        "Dir");

    bool FileBrowserOptions::operator == (const FileBrowserOptions& other) const
    {
        return
            dirList == other.dirList &&
            panel == other.panel &&
            pathEdit == other.pathEdit &&
            bellows == other.bellows;
    }

    bool FileBrowserOptions::operator != (const FileBrowserOptions& other) const
    {
        return !(*this == other);
    }

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

        p.widget = FileBrowserWidget::create(
            context,
            title,
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

    void FileBrowser::setTitle(const std::string& value)
    {
        _p->widget->setTitle(value);
    }

    void FileBrowser::setCallback(const std::function<void(const Path&)>& value)
    {
        _p->widget->setCallback(value);
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

    void to_json(nlohmann::json& json, const FileBrowserOptions& value)
    {
        json["DirList"] = value.dirList;
        json["Panel"] = value.panel;
        json["PathEdit"] = value.pathEdit;
        for (const auto& i : value.bellows)
        {
            json["Bellows"][i.first] = i.second;
        }
    }

    void from_json(const nlohmann::json& json, FileBrowserOptions& value)
    {
        json.at("DirList").get_to(value.dirList);
        json.at("Panel").get_to(value.panel);
        json.at("PathEdit").get_to(value.pathEdit);
        for (auto i = json.at("Bellows").begin(); i != json.at("Bellows").end(); ++i)
        {
            if (i->is_boolean())
            {
                i->get_to(value.bellows[i.key()]);
            }
        }
    }
}