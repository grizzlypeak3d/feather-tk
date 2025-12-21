// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowserPrivate.h>

#include <ftk/UI/ComboBox.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RecentFilesModel.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/SearchBox.h>
#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/Splitter.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

#include <filesystem>
#include <sstream>

namespace ftk
{
    struct FileBrowserWidget::Private
    {
        FileBrowserMode mode = FileBrowserMode::File;
        std::shared_ptr<FileBrowserModel> model;
        std::shared_ptr<RecentFilesModel> recentFilesModel;

        std::shared_ptr<Label> titleLabel;
        std::shared_ptr<ToolButton> panelButton;
        std::shared_ptr<ToolButton> upButton;
        std::shared_ptr<ToolButton> forwardButton;
        std::shared_ptr<ToolButton> backButton;
        std::shared_ptr<ToolButton> reloadButton;
        std::shared_ptr<FileBrowserPath> pathWidget;
        std::shared_ptr<FileBrowserPanel> panelWidget;
        std::shared_ptr<ScrollWidget> panelScrollWidget;
        std::shared_ptr<FileBrowserView> view;
        std::shared_ptr<ScrollWidget> viewScrollWidget;
        std::shared_ptr<LineEdit> fileEdit;
        std::shared_ptr<SearchBox> searchBox;
        std::shared_ptr<ComboBox> extsComboBox;
        std::shared_ptr<ComboBox> sortComboBox;
        std::shared_ptr<ToolButton> reverseSortButton;
        std::shared_ptr<PushButton> okButton;
        std::shared_ptr<PushButton> cancelButton;
        std::shared_ptr<Splitter> splitter;
        std::shared_ptr<VerticalLayout> layout;

        std::function<void(const Path&)> callback;
        std::function<void(void)> cancelCallback;

        std::shared_ptr<Observer<int> > currentObserver;
        std::shared_ptr<Observer<std::filesystem::path> > pathObserver;
        std::shared_ptr<Observer<bool> > forwardObserver;
        std::shared_ptr<Observer<bool> > backObserver;
        std::shared_ptr<Observer<FileBrowserOptions> > optionsObserver;
        std::shared_ptr<ListObserver<std::string> > extsObserver;
    };

    void FileBrowserWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::filesystem::path& path,
        FileBrowserMode mode,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::FileBrowserWidget", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);
        setVStretch(Stretch::Expanding);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.mode = mode;
        p.model = model;

        p.titleLabel = Label::create(context, title);
        p.titleLabel->setMarginRole(SizeRole::MarginSmall);
        p.titleLabel->setBackgroundRole(ColorRole::Button);

        p.panelButton = ToolButton::create(context);
        p.panelButton->setCheckable(true);
        p.panelButton->setIcon("PanelLeft");
        p.panelButton->setTooltip("Toggle the panel");

        p.upButton = ToolButton::create(context);
        p.upButton->setIcon("DirectoryUp");
        p.upButton->setRepeatClick(true);
        p.upButton->setTooltip("Go up a directory");

        p.backButton = ToolButton::create(context);
        p.backButton->setIcon("DirectoryBack");
        p.backButton->setRepeatClick(true);
        p.backButton->setTooltip("Go back a directory");

        p.forwardButton = ToolButton::create(context);
        p.forwardButton->setIcon("DirectoryForward");
        p.forwardButton->setRepeatClick(true);
        p.forwardButton->setTooltip("Go forward a directory");

        p.reloadButton = ToolButton::create(context);
        p.reloadButton->setIcon("Reload");
        p.reloadButton->setTooltip("Reload the current directory");

        p.pathWidget = FileBrowserPath::create(context);
        p.pathWidget->setTooltip("Current directory");

        p.panelWidget = FileBrowserPanel::create(context, model);
        p.panelScrollWidget = ScrollWidget::create(context);
        p.panelScrollWidget->setWidget(p.panelWidget);
        p.panelScrollWidget->setVStretch(Stretch::Expanding);

        p.view = FileBrowserView::create(context, mode, model);
        p.viewScrollWidget = ScrollWidget::create(context);
        p.viewScrollWidget->setWidget(p.view);
        p.viewScrollWidget->setVStretch(Stretch::Expanding);

        p.fileEdit = LineEdit::create(context);
        p.fileEdit->setText(path.u8string());
        p.fileEdit->setTextCallbackOnFocusLost(false);

        p.searchBox = SearchBox::create(context);
        p.searchBox->setTooltip("Filter");

        p.extsComboBox = ComboBox::create(context);
        p.extsComboBox->setTooltip("Filter by extension");
        p.extsComboBox->setVisible(FileBrowserMode::File == mode);

        p.sortComboBox = ComboBox::create(context, getDirListSortLabels());
        p.sortComboBox->setTooltip("Sorting");

        p.reverseSortButton = ToolButton::create(context);
        p.reverseSortButton->setCheckable(true);
        p.reverseSortButton->setIcon("ReverseSort");
        p.reverseSortButton->setTooltip("Reverse sorting");

        p.okButton = PushButton::create(context);
        p.okButton->setText("Ok");

        p.cancelButton = PushButton::create(context);
        p.cancelButton->setText("Cancel");

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::None);
        p.titleLabel->setParent(p.layout);
        Divider::create(context, Orientation::Vertical, p.layout);
        auto vLayout = VerticalLayout::create(context, p.layout);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        vLayout->setMarginRole(SizeRole::MarginSmall);
        vLayout->setVStretch(Stretch::Expanding);
        auto hLayout = HorizontalLayout::create(context, vLayout);
        hLayout->setSpacingRole(SizeRole::SpacingTool);
        hLayout->setVAlign(VAlign::Center);
        p.panelButton->setParent(hLayout);
        p.upButton->setParent(hLayout);
        p.backButton->setParent(hLayout);
        p.forwardButton->setParent(hLayout);
        p.reloadButton->setParent(hLayout);
        p.pathWidget->setParent(hLayout);
        p.splitter = Splitter::create(context, Orientation::Horizontal, vLayout);
        p.splitter->setSplit(.2F);
        p.splitter->setBorder(false);
        p.panelScrollWidget->setParent(p.splitter);
        p.viewScrollWidget->setParent(p.splitter);
        p.fileEdit->setParent(vLayout);
        hLayout = HorizontalLayout::create(context, vLayout);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        p.searchBox->setParent(hLayout);
        p.extsComboBox->setParent(hLayout);
        p.sortComboBox->setParent(hLayout);
        p.reverseSortButton->setParent(hLayout);
        hLayout->addSpacer(SizeRole::None, Stretch::Expanding);
        p.okButton->setParent(hLayout);
        p.cancelButton->setParent(hLayout);

        _optionsUpdate();
        _extsUpdate();

        p.panelButton->setCheckedCallback(
            [this](bool value)
            {
                FTK_P();
                FileBrowserOptions options = p.model->getOptions();
                options.panel = value;
                p.model->setOptions(options);
            });

        p.upButton->setClickedCallback(
            [this]
            {
                FTK_P();
                const std::filesystem::path path = p.model->getPath();
                const std::filesystem::path parentPath(path.parent_path());
                if (parentPath != path)
                {
                    p.model->setPath(parentPath);
                }
            });

        p.backButton->setClickedCallback(
            [this]
            {
                _p->model->back();
            });

        p.forwardButton->setClickedCallback(
            [this]
            {
                _p->model->forward();
            });

        p.reloadButton->setClickedCallback(
            [this]
            {
                _p->view->reload();
            });

        p.pathWidget->setCallback(
            [this](const std::filesystem::path& value)
            {
                _p->model->setPath(value);
            });
        p.pathWidget->setEditableCallback(
            [this](bool value)
            {
                FTK_P();
                FileBrowserOptions options = p.model->getOptions();
                options.pathEditable = value;
                p.model->setOptions(options);
            });

        p.view->setCallback(
            [this](const Path& value)
            {
                FTK_P();
                if (p.recentFilesModel)
                {
                    p.recentFilesModel->addRecent(value.get());
                }
                if (p.callback)
                {
                    p.callback(value);
                }
            });
        p.view->setSelectCallback(
            [this](const Path& value)
            {
                FTK_P();
                if (!value.isEmpty())
                {
                    p.fileEdit->setText(value.getFileName());
                }
            });

        p.fileEdit->setTextCallback(
            [this](const std::string& text)
            {
                _accept(text);
            });

        p.searchBox->setCallback(
            [this](const std::string& value)
            {
                _p->view->setSearch(value);
            });

        p.extsComboBox->setIndexCallback(
            [this](int value)
            {
                FTK_P();
                const std::vector<std::string>& exts = p.model->getExts();
                if (value >= 0 && value <= exts.size())
                {
                    const std::string ext = value > 0 ? exts[value - 1] : "";
                    p.model->setExt(ext);
                }
            });

        p.sortComboBox->setIndexCallback(
            [this](int value)
            {
                FTK_P();
                FileBrowserOptions options = p.model->getOptions();
                options.dirList.sort = static_cast<DirListSort>(value);
                p.model->setOptions(options);
            });

        p.reverseSortButton->setCheckedCallback(
            [this](bool value)
            {
                FTK_P();
                FileBrowserOptions options = p.model->getOptions();
                options.dirList.sortReverse = value;
                p.model->setOptions(options);
            });

        p.okButton->setClickedCallback(
            [this]
            {
                _accept(_p->fileEdit->getText());
            });

        p.cancelButton->setClickedCallback(
            [this]
            {
                FTK_P();
                if (p.cancelCallback)
                {
                    p.cancelCallback();
                }
            });

        p.currentObserver = Observer<int>::create(
            p.view->observeCurrent(),
            [this](int value)
            {
                if (value >= 0)
                {
                    const Box2I r = _p->view->getRect(value);
                    _p->viewScrollWidget->scrollTo(r);
                }
            });

        p.pathObserver = Observer<std::filesystem::path>::create(
            model->observePath(),
            [this](const std::filesystem::path& value)
            {
                FTK_P();
                p.pathWidget->setPath(value);
                p.viewScrollWidget->setScrollPos(V2I());
            });

        p.forwardObserver = Observer<bool>::create(
            model->observeHasForward(),
            [this](bool value)
            {
                _p->forwardButton->setEnabled(value);
            });

        p.backObserver = Observer<bool>::create(
            model->observeHasBack(),
            [this](bool value)
            {
                _p->backButton->setEnabled(value);
            });

        p.optionsObserver = Observer<FileBrowserOptions>::create(
            model->observeOptions(),
            [this](const FileBrowserOptions&)
            {
                _optionsUpdate();
            });

        p.extsObserver = ListObserver<std::string>::create(
            model->observeExts(),
            [this](const std::vector<std::string>&)
            {
                _extsUpdate();
            });
    }

    FileBrowserWidget::FileBrowserWidget() :
        _p(new Private)
    {}

    FileBrowserWidget::~FileBrowserWidget()
    {}

    std::shared_ptr<FileBrowserWidget> FileBrowserWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::filesystem::path& path,
        FileBrowserMode mode,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserWidget>(new FileBrowserWidget);
        out->_init(context, title, path, mode, model, parent);
        return out;
    }

    void FileBrowserWidget::setTitle(const std::string& value)
    {
        _p->titleLabel->setText(value);
    }

    void FileBrowserWidget::setCallback(const std::function<void(const Path&)>& value)
    {
        _p->callback = value;
    }

    void FileBrowserWidget::setCancelCallback(const std::function<void(void)>& value)
    {
        _p->cancelCallback = value;
    }

    const std::shared_ptr<FileBrowserModel>& FileBrowserWidget::getModel() const
    {
        return _p->model;
    }

    const std::shared_ptr<RecentFilesModel>& FileBrowserWidget::getRecentFilesModel() const
    {
        return _p->recentFilesModel;
    }

    void FileBrowserWidget::setRecentFilesModel(const std::shared_ptr<RecentFilesModel>& value)
    {
        FTK_P();
        p.recentFilesModel = value;
        p.panelWidget->setRecentFilesModel(value);
    }

    void FileBrowserWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void FileBrowserWidget::sizeHintEvent(const SizeHintEvent& value)
    {
        IMouseWidget::sizeHintEvent(value);
        setSizeHint(_p->layout->getSizeHint());
    }

    void FileBrowserWidget::_accept(const std::string& text)
    {
        FTK_P();
        Path path;
        switch (p.mode)
        {
        case FileBrowserMode::File:
            if (!text.empty())
            {
                path = Path((p.model->getPath() /
                    std::filesystem::u8path(text)).u8string());
            }
            break;
        case FileBrowserMode::Dir:
            path = Path(p.model->getPath().u8string());
            if (!text.empty())
            {
                path = Path((std::filesystem::u8path(path.get()) /
                    std::filesystem::u8path(text)).u8string());
            }
            break;
        default: break;
        }
        if (!path.isEmpty() && p.recentFilesModel)
        {
            p.recentFilesModel->addRecent(path.get());
        }
        if (p.callback)
        {
            p.callback(path);
        }
    }

    void FileBrowserWidget::_optionsUpdate()
    {
        FTK_P();
        const FileBrowserOptions& options = p.model->getOptions();

        p.panelButton->setChecked(options.panel);

        p.pathWidget->setEditable(options.pathEditable);

        p.panelScrollWidget->setVisible(options.panel);

        p.sortComboBox->setCurrentIndex(static_cast<int>(options.dirList.sort));
        p.reverseSortButton->setChecked(options.dirList.sortReverse);
    }

    void FileBrowserWidget::_extsUpdate()
    {
        FTK_P();
        const std::vector<std::string>& exts = p.model->getExts();
        const std::string& ext = p.model->getExt();

        std::vector<std::string> extsLabels;
        extsLabels.push_back("*.*");
        for (const auto& ext : exts)
        {
            extsLabels.push_back("*" + ext);
        }
        p.extsComboBox->setItems(extsLabels);
        const auto i = std::find(
            exts.begin(),
            exts.end(),
            ext);
        if (i != exts.end())
        {
            p.extsComboBox->setCurrentIndex(i - exts.begin() + 1);
        }
    }
}
