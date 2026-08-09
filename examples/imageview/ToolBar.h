// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IContainer.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

namespace imageview
{
    class Actions;

    //! Tool bar widget.
    class ToolBar : public ftk::IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        ToolBar() = default;

    public:
        virtual ~ToolBar();

        //! Create a new tool bar.
        static std::shared_ptr<ToolBar> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);


    private:
        void _createFileToolBar(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);
        void _createWindowToolBar(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);
        void _createViewToolBar(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);

        std::map<std::string, std::shared_ptr<ftk::ToolButton> > _buttons;
        std::shared_ptr<ftk::HorizontalLayout> _layout;
    };
}
