// SPDX-License-Identifier: BSD-3-Clause
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowserPrivate.h>

#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/StackLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Path.h>

namespace ftk
{
    struct FileBrowserPath::Private
    {
        std::filesystem::path path;
        std::vector<std::string> pieces;
        bool edit = false;
        std::shared_ptr<HorizontalLayout> layout;
        std::shared_ptr<ToolButton> editButton;
        std::shared_ptr<StackLayout> stackLayout;
        std::shared_ptr<HorizontalLayout> buttonsLayout;
        std::shared_ptr<ScrollWidget> buttonsScrollWidget;
        std::vector<std::shared_ptr<IButton> > buttons;
        std::shared_ptr<ButtonGroup> buttonGroup;
        std::shared_ptr<LineEdit> lineEdit;
        std::function<void(const std::filesystem::path&)> callback;
        std::function<void(bool)> editCallback;
    };

    void FileBrowserPath::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FileBrowserPath", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.layout->setVAlign(VAlign::Center);

        p.editButton = ToolButton::create(context, p.layout);
        p.editButton->setCheckable(true);
        p.editButton->setIcon("Edit");
        p.editButton->setTooltip("Edit the path");

        p.stackLayout = StackLayout::create(context, p.layout);
        p.stackLayout->setHStretch(Stretch::Expanding);

        p.buttonsLayout = HorizontalLayout::create(context);
        p.buttonsLayout->setSpacingRole(SizeRole::None);

        p.buttonsScrollWidget = ScrollWidget::create(context, ScrollType::Horizontal, p.stackLayout);
        p.buttonsScrollWidget->setWidget(p.buttonsLayout);

        p.buttonGroup = ButtonGroup::create(context, ButtonGroupType::Click);

        p.lineEdit = LineEdit::create(context, p.stackLayout);
        p.lineEdit->setHStretch(Stretch::Expanding);

        p.editButton->setCheckedCallback(
            [this](bool value)
            {
                setEdit(value);
                if (_p->editCallback)
                {
                    _p->editCallback(value);
                }
            });

        p.buttonGroup->setClickedCallback(
            [this](int index)
            {
                if (index >= 0 && index < _p->pieces.size())
                {
                    std::filesystem::path path;
                    for (int i = 0; i <= index; ++i)
                    {
                        path = path / _p->pieces[i];
                    }
                    _p->path = path;
                    _widgetUpdate();
                    if (_p->callback)
                    {
                        _p->callback(_p->path);
                    }
                }
            });

        p.lineEdit->setTextCallback(
            [this](const std::string& value)
            {
                _p->path = value;
                _widgetUpdate();
                if (_p->callback)
                {
                    _p->callback(_p->path);
                }
            });
    }

    FileBrowserPath::FileBrowserPath() :
        _p(new Private)
    {}

    FileBrowserPath::~FileBrowserPath()
    {}

    std::shared_ptr<FileBrowserPath> FileBrowserPath::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserPath>(new FileBrowserPath);
        out->_init(context, parent);
        return out;
    }

    void FileBrowserPath::setPath(const std::filesystem::path& value)
    {
        FTK_P();
        if (value == p.path)
            return;
        p.path = value;
        _widgetUpdate();
    }

    void FileBrowserPath::setCallback(const std::function<void(const std::filesystem::path&)>& value)
    {
        _p->callback = value;
    }

    void FileBrowserPath::setEdit(bool value)
    {
        FTK_P();
        if (value == p.edit)
            return;
        p.edit = value;
        _widgetUpdate();
    }

    void FileBrowserPath::setEditCallback(const std::function<void(bool)>& value)
    {
        _p->editCallback = value;
    }

    void FileBrowserPath::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void FileBrowserPath::sizeHintEvent(const SizeHintEvent& value)
    {
        _setSizeHint(_p->layout->getSizeHint());
    }

    void FileBrowserPath::_widgetUpdate()
    {
        FTK_P();
        p.editButton->setChecked(p.edit);
        p.stackLayout->setCurrentWidget(
            p.edit ?
            std::static_pointer_cast<IWidget>(p.lineEdit) :
            std::static_pointer_cast<IWidget>(p.buttonsScrollWidget));

        p.buttons.clear();
        p.buttonGroup->clearButtons();
        p.buttonsLayout->clear();
        p.pieces = split(p.path);
        if (auto context = getContext())
        {
            for (size_t i = 0; i < p.pieces.size(); ++i)
            {
                auto button = ToolButton::create(context, p.pieces[i], p.buttonsLayout);
                p.buttons.push_back(button);
                p.buttonGroup->addButton(button);
                Divider::create(context, Orientation::Horizontal, p.buttonsLayout);
            }
        }

        p.lineEdit->setText(p.path.u8string());
    }
}
