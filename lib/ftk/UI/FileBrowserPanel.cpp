// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowserPrivate.h>

#include <ftk/UI/Bellows.h>
#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/CheckBox.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/DrivesModel.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/ListItemsWidget.h>
#include <ftk/UI/RecentFilesModel.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Path.h>

#include <filesystem>

namespace ftk
{
    struct FileBrowserDrives::Private
    {
        std::shared_ptr<DrivesModel> drivesModel;
        std::vector<std::filesystem::path> drives;
        std::shared_ptr<ListItemsWidget> listWidget;
        std::shared_ptr<ListObserver<std::filesystem::path> > drivesObserver;
    };

    void FileBrowserDrives::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FileBrowserDrives", parent);
        FTK_P();

        setBackgroundRole(ColorRole::Base);

        p.drivesModel = DrivesModel::create(context);

        p.listWidget = ListItemsWidget::create(
            context,
            ButtonGroupType::Click);
        _setWidget(p.listWidget);

        p.listWidget->setCallback(
            [this, model](int index, bool)
            {
                FTK_P();
                if (index >= 0 && index < static_cast<int>(p.drives.size()))
                {
                    model->setPath(p.drives[index]);
                }
            });

        p.drivesObserver = ListObserver<std::filesystem::path>::create(
            p.drivesModel->observeDrives(),
            [this](const std::vector<std::filesystem::path>& value)
            {
                FTK_P();
                p.drives = value;
                std::vector<ListItem> items;
                for (const auto& drive : p.drives)
                {
                    items.push_back(ListItem(drive.u8string(), drive.u8string()));
                }
                p.listWidget->setItems(items);
            });
    }

    FileBrowserDrives::FileBrowserDrives() :
        _p(new Private)
    {}

    FileBrowserDrives::~FileBrowserDrives()
    {}

    std::shared_ptr<FileBrowserDrives> FileBrowserDrives::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserDrives>(new FileBrowserDrives);
        out->_init(context, model, parent);
        return out;
    }

    

    

    struct FileBrowserShortcuts::Private
    {
        std::vector<std::filesystem::path> shortcuts;
        std::shared_ptr<ListItemsWidget> listWidget;
    };

    void FileBrowserShortcuts::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FileBrowserShortcuts", parent);
        FTK_P();

        setBackgroundRole(ColorRole::Base);

        p.listWidget = ListItemsWidget::create(
            context,
            ButtonGroupType::Click);
        _setWidget(p.listWidget);

        _widgetUpdate();

        p.listWidget->setCallback(
            [this, model](int index, bool)
            {
                FTK_P();
                if (index >= 0 && index < static_cast<int>(p.shortcuts.size()))
                {
                    model->setPath(p.shortcuts[index]);
                }
            });
    }

    FileBrowserShortcuts::FileBrowserShortcuts() :
        _p(new Private)
    {}

    FileBrowserShortcuts::~FileBrowserShortcuts()
    {}

    std::shared_ptr<FileBrowserShortcuts> FileBrowserShortcuts::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserShortcuts>(new FileBrowserShortcuts);
        out->_init(context, model, parent);
        return out;
    }

    

    

    void FileBrowserShortcuts::_widgetUpdate()
    {
        FTK_P();
        p.shortcuts.clear();
        std::filesystem::path path = std::filesystem::current_path();
        p.shortcuts.push_back(path);
        std::vector<ListItem> items;
        items.push_back(ListItem("Current", path.u8string()));
        for (auto userPath : getUserPathEnums())
        {
            path = getUserPath(userPath);
            p.shortcuts.push_back(path);
            items.push_back(ListItem(path.filename().u8string(), path.u8string()));
        }
        p.listWidget->setItems(items);
    }

    struct FileBrowserRecent::Private
    {
        std::shared_ptr<RecentFilesModel> recentFilesModel;
        // The directories the recent files are in, which is what this lists:
        // a browser goes to a place rather than to a file.
        std::vector<std::filesystem::path> recent;
        std::shared_ptr<ListItemsWidget> listWidget;
        std::shared_ptr<ListObserver<Path> > recentObserver;
    };

    void FileBrowserRecent::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FileBrowserRecent", parent);
        FTK_P();

        setBackgroundRole(ColorRole::Base);

        p.listWidget = ListItemsWidget::create(
            context,
            ButtonGroupType::Click);
        _setWidget(p.listWidget);

        _widgetUpdate();

        p.listWidget->setCallback(
            [this, model](int index, bool)
            {
                FTK_P();
                if (index >= 0 && index < static_cast<int>(p.recent.size()))
                {
                    model->setPath(p.recent[index]);
                }
            });
    }

    FileBrowserRecent::FileBrowserRecent() :
        _p(new Private)
    {}

    FileBrowserRecent::~FileBrowserRecent()
    {}

    std::shared_ptr<FileBrowserRecent> FileBrowserRecent::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserRecent>(new FileBrowserRecent);
        out->_init(context, model, parent);
        return out;
    }

    const std::shared_ptr<RecentFilesModel>& FileBrowserRecent::getRecentFilesModel() const
    {
        return _p->recentFilesModel;
    }

    void FileBrowserRecent::setRecentFilesModel(const std::shared_ptr<RecentFilesModel>& value)
    {
        FTK_P();
        p.recentFilesModel = value;
        if (p.recentFilesModel)
        {
            p.recentObserver = ListObserver<Path>::create(
                p.recentFilesModel->observeRecent(),
                [this](const std::vector<Path>& paths)
                {
                    _p->recent.clear();
                    for (auto i = paths.rbegin(); i != paths.rend(); ++i)
                    {
                        std::filesystem::path tmp =
                            std::filesystem::u8path(i->get());
                        if (!std::filesystem::is_directory(tmp))
                        {
                            tmp = tmp.parent_path();
                        }
                        const auto j = std::find(_p->recent.begin(), _p->recent.end(), tmp);
                        if (j == _p->recent.end())
                        {
                            _p->recent.push_back(tmp);
                        }
                    }
                    _widgetUpdate();
                });
        }
        else
        {
            p.recentObserver.reset();
        }
    }
    
    

    

    void FileBrowserRecent::_widgetUpdate()
    {
        FTK_P();
        std::vector<ListItem> items;
        for (const auto& recent : p.recent)
        {
            std::filesystem::path tmp = recent.filename();
            if (tmp.empty())
            {
                tmp = recent.parent_path().filename();
            }
            if (tmp.empty())
            {
                tmp = recent;
            }
            items.push_back(ListItem(tmp.u8string(), recent.u8string()));
        }
        p.listWidget->setItems(items);
    }

    struct FileBrowserSettings::Private
    {
        FileBrowserOptions options;
        std::shared_ptr<ftk::ComboBox> thumbnailsComboBox;
        std::shared_ptr<ftk::CheckBox> seqCheckBox;
        std::shared_ptr<ftk::CheckBox> hiddenCheckBox;
        std::shared_ptr<FormLayout> layout;
        std::shared_ptr<Observer<FileBrowserOptions> > optionsObserver;
    };

    void FileBrowserSettings::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FileBrowserSettings", parent);
        FTK_P();

        p.thumbnailsComboBox = ftk::ComboBox::create(
            context, getFileBrowserThumbnailsLabels());
        p.thumbnailsComboBox->setHStretch(ftk::Stretch::Expanding);
        p.thumbnailsComboBox->setTooltip(
            "How large the thumbnails are.\n"
            "\n"
            "Reading them is work, and a directory at the\n"
            "other end of a network is where that is felt.");

        p.seqCheckBox = ftk::CheckBox::create(context);
        p.seqCheckBox->setHStretch(ftk::Stretch::Expanding);

        p.hiddenCheckBox = ftk::CheckBox::create(context);
        p.hiddenCheckBox->setHStretch(ftk::Stretch::Expanding);

        p.layout = FormLayout::create(context);
        _setWidget(p.layout);
        p.layout->setMarginRole(ftk::SizeRole::MarginSmall);
        p.layout->addRow("Thumbnails:", p.thumbnailsComboBox);
        p.layout->addRow("File sequences:", p.seqCheckBox);
        p.layout->addRow("Show hidden:", p.hiddenCheckBox);

        p.thumbnailsComboBox->setIndexCallback(
            [model](int value)
            {
                FileBrowserOptions options = model->getOptions();
                options.thumbnails = static_cast<FileBrowserThumbnails>(value);
                model->setOptions(options);
            });

        p.seqCheckBox->setCheckedCallback(
            [model](bool value)
            {
                FileBrowserOptions options = model->getOptions();
                options.dirList.seq = value;
                model->setOptions(options);
            });

        p.hiddenCheckBox->setCheckedCallback(
            [model](bool value)
            {
                FileBrowserOptions options = model->getOptions();
                options.dirList.hidden = value;
                model->setOptions(options);
            });

        p.optionsObserver = Observer<FileBrowserOptions>::create(
            model->observeOptions(),
            [this](const FileBrowserOptions& value)
            {
                _p->thumbnailsComboBox->setCurrentIndex(
                    static_cast<int>(value.thumbnails));
                _p->seqCheckBox->setChecked(value.dirList.seq);
                _p->hiddenCheckBox->setChecked(value.dirList.hidden);
            });
    }

    FileBrowserSettings::FileBrowserSettings() :
        _p(new Private)
    {}

    FileBrowserSettings::~FileBrowserSettings()
    {}

    std::shared_ptr<FileBrowserSettings> FileBrowserSettings::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserSettings>(new FileBrowserSettings);
        out->_init(context, model, parent);
        return out;
    }

    

    

    void FileBrowserSettings::_widgetUpdate()
    {
        FTK_P();
        p.hiddenCheckBox->setChecked(p.options.dirList.hidden);
    }

    struct FileBrowserPanel::Private
    {
        std::shared_ptr<FileBrowserDrives> drivesWidget;
        std::shared_ptr<FileBrowserShortcuts> shortcutsWidget;
        std::shared_ptr<FileBrowserRecent> recentWidget;
        std::shared_ptr<FileBrowserSettings> settingsWidget;
        std::map<std::string, std::shared_ptr<Bellows> > bellows;
        std::shared_ptr<VerticalLayout> layout;
        std::shared_ptr<Observer<FileBrowserOptions> > optionsObserver;
    };

    void FileBrowserPanel::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FileBrowserPanel", parent);
        FTK_P();

        p.drivesWidget = FileBrowserDrives::create(context, model);
        p.shortcutsWidget = FileBrowserShortcuts::create(context, model);
        p.recentWidget = FileBrowserRecent::create(context, model);
        p.settingsWidget = FileBrowserSettings::create(context, model);

        p.layout = VerticalLayout::create(context);
        _setWidget(p.layout);
        p.layout->setSpacingRole(SizeRole::Border);
        p.bellows["Drives"] = Bellows::create(context, "Drives", p.layout);
        p.bellows["Drives"]->setWidget(p.drivesWidget);
        p.bellows["Shortcuts"] = Bellows::create(context, "Shortcuts", p.layout);
        p.bellows["Shortcuts"]->setWidget(p.shortcutsWidget);
        p.bellows["Recent"] = Bellows::create(context, "Recent", p.layout);
        p.bellows["Recent"]->setWidget(p.recentWidget);
        p.bellows["Settings"] = Bellows::create(context, "Settings", p.layout);
        p.bellows["Settings"]->setWidget(p.settingsWidget);

        for (const auto& bellows : p.bellows)
        {
            const std::string name = bellows.first;
            bellows.second->setOpenCallback(
                [model, name](bool value)
                {
                    auto options = model->getOptions();
                    options.bellows[name] = value;
                    model->setOptions(options);
                });
        }

        p.optionsObserver = Observer<FileBrowserOptions>::create(
            model->observeOptions(),
            [this](const FileBrowserOptions& value)
            {
                FTK_P();
                for (const auto& i : value.bellows)
                {
                    if (auto j = p.bellows.find(i.first); j != p.bellows.end())
                    {
                        j->second->setOpen(i.second);
                    }
                }
            });
    }

    FileBrowserPanel::FileBrowserPanel() :
        _p(new Private)
    {}

    FileBrowserPanel::~FileBrowserPanel()
    {}

    std::shared_ptr<FileBrowserPanel> FileBrowserPanel::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserPanel>(new FileBrowserPanel);
        out->_init(context, model, parent);
        return out;
    }

    void FileBrowserPanel::setRecentFilesModel(const std::shared_ptr<RecentFilesModel>& value)
    {
        _p->recentWidget->setRecentFilesModel(value);
    }

    

    
}
