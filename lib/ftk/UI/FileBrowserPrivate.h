// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IContainer.h>
#include <ftk/UI/FileBrowserWidgets.h>

#include <ftk/UI/IButton.h>

namespace ftk
{
    class FileBrowserPanel : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserPanel();

    public:
        virtual ~FileBrowserPanel();

        static std::shared_ptr<FileBrowserPanel> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);


    private:
        FTK_PRIVATE();
    };
}
