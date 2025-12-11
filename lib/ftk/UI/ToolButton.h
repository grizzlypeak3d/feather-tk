// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IButton.h>

namespace ftk
{
    class Action;

    //! \name Buttons
    ///@{

    //! Tool button.
    class FTK_API_TYPE ToolButton : public IButton
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>&,
            const std::shared_ptr<IWidget>& parent);

        ToolButton();

    public:
        virtual ~ToolButton();

        //! Create a new widget.
        static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setText(const std::string&) override;
        void setFontRole(FontRole) override;

        void setGeometry(const Box2I&) override;
        void setAcceptsKeyFocus(bool) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
