// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/FileBrowser.h>
#include <ftk/UI/LineEdit.h>

#include <ftk/Core/Path.h>

namespace ftk
{
    //! \name File Widgets
    ///@{

    //! File edit widget.
    class FTK_UI_API_TYPE FileEdit : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<IWidget>& parent);

        FileEdit();

    public:
        FTK_UI_API virtual ~FileEdit();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileEdit> create(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the path.
        FTK_UI_API const Path& getPath() const;

        //! Set the path.
        FTK_UI_API void setPath(const Path&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Path&)>&);

        //! Get the text entry, for a dialog that wants the key focus to
        //! start in the path rather than on one of its buttons.
        FTK_UI_API const std::shared_ptr<LineEdit>& getLineEdit() const;

        FTK_UI_API void dropEvent(DragDropEvent&) override;

    private:
        void _openDialog();
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
