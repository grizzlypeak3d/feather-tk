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
        FTK_API virtual ~ToolButton();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get whether the button has a popup icon.
        bool hasPopupIcon() const;

        //! Set whether the button has a popup icon.
        void setPopupIcon(bool);

        FTK_API void setText(const std::string&) override;
        FTK_API void setFontRole(FontRole) override;

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void setAcceptsKeyFocus(bool) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
