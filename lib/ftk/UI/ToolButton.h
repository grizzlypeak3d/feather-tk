// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IButton.h>

namespace ftk
{
    class Action;

    //! \name Buttons
    ///@{

    //! Tool button.
    class FTK_UI_API_TYPE ToolButton : public IButton
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>&,
            const std::shared_ptr<IWidget>& parent);

        ToolButton();

    public:
        FTK_UI_API virtual ~ToolButton();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ToolButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<Action>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        //! Get the corner radius role.
        SizeRole getCornerRadiusRole() const;

        //! Set the corner radius role.
        void setCornerRadiusRole(SizeRole);

        //! Get whether an active accent underline is drawn.
        ColorRole hasAccentUnderline() const;

        //! Set whether an active accent underline is drawn along the bottom edge.
        void setAccentUnderline(ColorRole);

        //! Get whether the button has a popup icon.
        FTK_UI_API bool hasPopupIcon() const;

        //! Set whether the button has a popup icon.
        FTK_UI_API void setPopupIcon(bool);

        //! A string literal converts to bool, so a caller asking for an icon
        //! by name would silently ask for true instead.
        void setPopupIcon(const char*) = delete;


        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void setAcceptsKeyFocus(bool) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    protected:
        FTK_UI_API void _sizeDirty() override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
