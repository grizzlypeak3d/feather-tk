// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileEdit.h>

#include <ftk/UI/LineEdit.h>
#include <ftk/UI/LineEditModel.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>
#include <ftk/Core/Path.h>

namespace ftk
{
    namespace
    {
        //! The last component of a path: a directory's name whether or not
        //! it ends in a separator, and the whole path when there is no name
        //! to show, as for the root.
        std::string getName(const Path& path)
        {
            std::string s = path.get();
            while (s.size() > 1 && ('/' == s.back() || '\\' == s.back()))
            {
                s.pop_back();
            }
            const std::string out = Path(s).getFileName();
            return out.empty() ? path.get() : out;
        }
    }

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
        IContainer::_init(context, "ftk::FileEdit", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.mode = mode;

        p.lineEdit = LineEdit::create(context);
        // Room for a name, in characters of an average width: the field
        // shows a name until it is being edited, and thirty of the widest
        // character made every form holding one wider than its other
        // fields asked for.
        p.lineEdit->setFormat(std::string(20, '0'));
        p.lineEdit->setHStretch(Stretch::Expanding);

        p.browseButton = ToolButton::create(context);
        p.browseButton->setIcon("FileBrowser");
        p.browseButton->setTooltip("Open the file browser");

        p.layout = HorizontalLayout::create(context);
        _setWidget(p.layout);
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.lineEdit->setParent(p.layout);
        p.browseButton->setParent(p.layout);

        _widgetUpdate();

        p.lineEdit->setCallback(
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
    
    const std::shared_ptr<LineEdit>& FileEdit::getLineEdit() const
    {
        return _p->lineEdit;
    }


    

    void FileEdit::dropEvent(DragDropEvent& event)
    {
        FTK_P();
        if (auto data = std::dynamic_pointer_cast<DragDropTextData>(event.data))
        {
            event.accept = true;
            const auto& text = data->getText();
            if (!text.empty())
            {
                p.path = Path(text.front());
                _widgetUpdate();
                if (p.callback)
                {
                    p.callback(p.path);
                }
            }
        }
    }

    void FileEdit::_openDialog()
    {
        FTK_P();
        if (auto context = getContext())
        {
            if (auto fileBrowserSystem = context->getSystem<FileBrowserSystem>())
            {
                FileBrowserOpenOptions options;
                options.title = "File Browser";
                // The browser opens at a directory while the edit holds a
                // file, so what is passed is the directory that file is in:
                // handing over the file itself asked for a listing of it and
                // got nothing, which is no help to someone changing it. A
                // directory is passed as it stands, whether it is what is
                // being picked or what the field was left holding.
                std::filesystem::path path =
                    toFileSystem(p.path.get());
                std::error_code ec;
                if (!path.empty() && !std::filesystem::is_directory(path, ec))
                {
                    path = path.parent_path();
                }
                options.path = path;
                options.mode = p.mode;
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
                    options);
            }
        }
    }
    void FileEdit::_widgetUpdate()
    {
        FTK_P();
        // The name until the field is being edited, then the whole path to
        // edit; the tooltip has the whole path throughout. A directory too:
        // its last component does not say where it is, but neither does any
        // part of a path short of the whole of it, which is a hover or a
        // click away. (Directories showed the whole path from 2026-08-07,
        // which made the field as wide as the longest path it might hold.)
        p.lineEdit->setText(p.lineEdit->hasKeyFocus() ?
            p.path.get() :
            getName(p.path));
        p.lineEdit->setTooltip(p.path.get());
    }
}