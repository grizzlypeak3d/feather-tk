// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Buttons
    ///@{

    //! Base class for buttons.
    class FTK_API_TYPE IButton : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        IButton();

    public:
        FTK_API virtual ~IButton();

        //! Get whether the button is checkable.
        FTK_API bool isCheckable() const;

        //! Set whether the button is checkable.
        FTK_API void setCheckable(bool);

        //! Get whether the button is checked.
        FTK_API bool isChecked() const;

        //! Set whether the button is checked.
        FTK_API void setChecked(bool);

        //! Get the text.
        FTK_API const std::string& getText() const;

        //! Set the text.
        FTK_API virtual void setText(const std::string&);

        //! Get the font role.
        FTK_API FontRole getFontRole() const;

        //! Set the font role.
        FTK_API virtual void setFontRole(FontRole);

        //! Get the icon.
        FTK_API const std::string& getIcon() const;

        //! Set the icon.
        FTK_API void setIcon(const std::string&);

        //! Get the checked icon.
        FTK_API const std::string& getCheckedIcon() const;

        //! Set the checked icon.
        FTK_API void setCheckedIcon(const std::string&);

        //! Get the button color role.
        FTK_API ColorRole getButtonRole() const;

        //! Set the button color role.
        FTK_API void setButtonRole(ColorRole);

        //! Get the checked color role.
        FTK_API ColorRole getCheckedRole() const;

        //! Set the checked color role.
        FTK_API void setCheckedRole(ColorRole);

        //! Get the text color role.
        FTK_API ColorRole getTextRole() const;

        //! Set the text color role.
        FTK_API void setTextRole(ColorRole);

        //! Set the hovered callback.
        FTK_API void setHoveredCallback(const std::function<void(bool)>&);

        //! Set the pressed callback.
        FTK_API void setPressedCallback(const std::function<void(void)>&);

        //! Get whether the button repeats clicks when pressed.
        FTK_API bool hasRepeatClick() const;

        //! Set whether the button repeats clicks when pressed.
        FTK_API void setRepeatClick(bool);

        //! Set the clicked callback.
        FTK_API void setClickedCallback(const std::function<void(void)>&);

        //! Set the checked callback.
        FTK_API void setCheckedCallback(const std::function<void(bool)>&);

        //! Click the button.
        FTK_API void click();

        FTK_API void tickEvent(
            bool,
            bool,
            const TickEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    protected:
        std::string _text;
        FontRole _fontRole = FontRole::Label;

        std::string _icon;
        std::shared_ptr<Image> _iconImage;
        std::string _checkedIcon;
        std::shared_ptr<Image> _checkedIconImage;

        ColorRole _buttonRole = ColorRole::Button;
        ColorRole _checkedRole = ColorRole::Checked;
        ColorRole _textRole = ColorRole::Text;
        bool _checked = false;

        std::function<void(bool)> _hoveredCallback;
        std::function<void(void)> _pressedCallback;
        std::function<void(void)> _clickedCallback;
        std::function<void(bool)> _checkedCallback;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
