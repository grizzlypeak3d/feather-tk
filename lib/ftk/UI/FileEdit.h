// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FileBrowser.h>

#include <ftk/Core/Path.h>

namespace ftk
{
    class RecentFilesModel;

    //! \name File Widgets
    ///@{

    //! File edit widget.
    class FileEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<IWidget>& parent);

        FileEdit();

    public:
        virtual ~FileEdit();

        //! Create a new widget.
        static std::shared_ptr<FileEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<FileEdit> create(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the path.
        const Path& getPath() const;

        //! Set the path.
        void setPath(const Path&);

        //! Set the callback.
        void setCallback(const std::function<void(const Path&)>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _openDialog();
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
