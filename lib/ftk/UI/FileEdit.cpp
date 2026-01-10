// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileEdit.h>

#include <ftk/UI/LineEdit.h>
#include <ftk/UI/LineEditModel.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

namespace ftk
{
    struct FileEdit::Private
    {
        FileBrowserMode mode = FileBrowserMode::Open;
        Path path;

        std::shared_ptr<LineEdit> lineEdit;
        std::shared_ptr<ToolButton> browseButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(const Path&)> callback;
    };

    void FileEdit::_init(
        const std::shared_ptr<Context>& context,
        FileBrowserMode mode,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FileEdit", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.mode = mode;

        p.lineEdit = LineEdit::create(context);
        p.lineEdit->setHStretch(Stretch::Expanding);

        p.browseButton = ToolButton::create(context);
        p.browseButton->setIcon("FileBrowser");
        p.browseButton->setTooltip("Open the file browser");

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.lineEdit->setParent(p.layout);
        p.browseButton->setParent(p.layout);

        _widgetUpdate();

        p.lineEdit->setTextCallback(
            [this](const std::string& value)
            {
                FTK_P();
                p.path = Path(value);
                _widgetUpdate();
                if (p.callback)
                {
                    p.callback(p.path);
                }
            });

        p.lineEdit->setFocusCallback(
            [this](bool value)
            {
                _widgetUpdate();
            });

        p.browseButton->setClickedCallback(
            [this]
            {
                _openDialog();
            });
    }

    FileEdit::FileEdit() :
        _p(new Private)
    {}

    FileEdit::~FileEdit()
    {}

    std::shared_ptr<FileEdit> FileEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileEdit>(new FileEdit);
        out->_init(context, FileBrowserMode::Open, parent);
        return out;
    }

    std::shared_ptr<FileEdit> FileEdit::create(
        const std::shared_ptr<Context>& context,
        FileBrowserMode mode,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileEdit>(new FileEdit);
        out->_init(context, mode, parent);
        return out;
    }

    const Path& FileEdit::getPath() const
    {
        return _p->path;
    }

    void FileEdit::setPath(const Path& value)
    {
        FTK_P();
        if (value == p.path)
            return;
        p.path = value;
        _widgetUpdate();
    }

    void FileEdit::setCallback(const std::function<void(const Path&)>& value)
    {
        _p->callback = value;
    }
    
    Size2I FileEdit::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void FileEdit::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void FileEdit::_openDialog()
    {
        FTK_P();
        if (auto context = getContext())
        {
            if (auto fileBrowserSystem = context->getSystem<FileBrowserSystem>())
            {
                fileBrowserSystem->open(
                    getWindow(),
                    [this](const Path& value)
                    {
                        FTK_P();
                        p.path = value;
                        _widgetUpdate();
                        if (p.callback)
                        {
                            p.callback(p.path);
                        }
                    },
                    "File Browser",
                    std::filesystem::u8path(p.path.get()),
                    p.mode);
            }
        }
    }
    void FileEdit::_widgetUpdate()
    {
        FTK_P();
        p.lineEdit->setText(
            p.lineEdit->hasKeyFocus() ?
            p.path.get() :
            p.path.getFileName());
        p.lineEdit->setTooltip(p.path.get());
    }
}