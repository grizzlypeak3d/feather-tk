// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IButton.h>

#include <ftk/UI/DrawUtil.h>

namespace ftk
{
    struct IButton::Private
    {
        bool checkable = false;
        float iconScale = 1.F;
        bool repeatClick = false;
        bool repeatClickInit = false;
        std::chrono::steady_clock::time_point repeatClickTimer;
    };

    void IButton::_init(
        const std::shared_ptr<Context>& context,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, objectName, parent);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);
    }

    IButton::IButton() :
        _p(new Private)
    {}

    IButton::~IButton()
    {}

    bool IButton::isCheckable() const
    {
        return _p->checkable;
    }

    void IButton::setCheckable(bool value)
    {
        FTK_P();
        if (value == p.checkable)
            return;
        p.checkable = value;
        if (!p.checkable && _checked)
        {
            _checked = false;
            setDrawUpdate();
        }
    }

    bool IButton::isChecked() const
    {
        return _checked;
    }

    void IButton::setChecked(bool value)
    {
        FTK_P();
        if (value == _checked)
            return;
        _checked = value;
        setDrawUpdate();
    }

    const std::string& IButton::getText() const
    {
        return _text;
    }

    void IButton::setText(const std::string& value)
    {
        if (value == _text)
            return;
        _text = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    FontRole IButton::getFontRole() const
    {
        return _fontRole;
    }

    void IButton::setFontRole(FontRole value)
    {
        if (value == _fontRole)
            return;
        _fontRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    const std::string& IButton::getIcon() const
    {
        return _icon;
    }

    void IButton::setIcon(const std::string& icon)
    {
        FTK_P();
        if (icon == _icon)
            return;
        _icon = icon;
        _iconImage.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    const std::string& IButton::getCheckedIcon() const
    {
        return _checkedIcon;
    }

    void IButton::setCheckedIcon(const std::string& icon)
    {
        FTK_P();
        if (icon == _checkedIcon)
            return;
        _checkedIcon = icon;
        _checkedIconImage.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    ColorRole IButton::getButtonRole() const
    {
        return _buttonRole;
    }

    void IButton::setButtonRole(ColorRole value)
    {
        if (value == _buttonRole)
            return;
        _buttonRole = value;
        setDrawUpdate();
    }

    ColorRole IButton::getCheckedRole() const
    {
        return _checkedRole;
    }

    void IButton::setCheckedRole(ColorRole value)
    {
        if (value == _checkedRole)
            return;
        _checkedRole = value;
        setDrawUpdate();
    }

    ColorRole IButton::getTextRole() const
    {
        return _textRole;
    }

    void IButton::setTextRole(ColorRole value)
    {
        if (value == _textRole)
            return;
        _textRole = value;
        setDrawUpdate();
    }

    bool IButton::hasRepeatClick() const
    {
        return _p->repeatClick;
    }

    void IButton::setRepeatClick(bool value)
    {
        _p->repeatClick = value;
    }

    void IButton::setHoveredCallback(const std::function<void(bool)>& value)
    {
        _hoveredCallback = value;
    }

    void IButton::setPressedCallback(const std::function<void(void)>& value)
    {
        _pressedCallback = value;
    }

    void IButton::setClickedCallback(const std::function<void(void)>& value)
    {
        _clickedCallback = value;
    }

    void IButton::setCheckedCallback(const std::function<void(bool)>& value)
    {
        _checkedCallback = value;
    }

    void IButton::click()
    {
        FTK_P();
        if (_clickedCallback)
        {
            _clickedCallback();
        }
        if (p.checkable)
        {
            _checked = !_checked;
            setDrawUpdate();
            if (_checkedCallback)
            {
                _checkedCallback(_checked);
            }
        }
    }

    void IButton::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        IMouseWidget::tickEvent(parentsVisible, parentsEnabled, event);
        FTK_P();
        if (_isMousePressed() && p.repeatClick)
        {
            const float duration = p.repeatClickInit ? .4F : .02F;
            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<float> diff = now - p.repeatClickTimer;
            if (diff.count() > duration)
            {
                click();
                p.repeatClickInit = false;
                p.repeatClickTimer = now;
            }
        }
    }

    void IButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (event.displayScale != p.iconScale)
        {
            p.iconScale = event.displayScale;
            _iconImage.reset();
            _checkedIconImage.reset();
        }
        if (!_icon.empty() && !_iconImage)
        {
            _iconImage = event.iconSystem->get(_icon, event.displayScale);
        }
        if (!_checkedIcon.empty() && !_checkedIconImage)
        {
            _checkedIconImage = event.iconSystem->get(_checkedIcon, event.displayScale);
        }
    }

    void IButton::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        event.accept = true;
        setDrawUpdate();
        if (_hoveredCallback)
        {
            _hoveredCallback(_isMouseInside());
        }
    }

    void IButton::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
        if (_hoveredCallback)
        {
            _hoveredCallback(_isMouseInside());
        }
    }

    void IButton::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (acceptsKeyFocus())
        {
            takeKeyFocus();
        }
        setDrawUpdate();
        if (_pressedCallback)
        {
            _pressedCallback();
        }
        if (p.repeatClick)
        {
            p.repeatClickInit = true;
            p.repeatClickTimer = std::chrono::steady_clock::now();
        }
    }

    void IButton::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        setDrawUpdate();
        if (contains(getGeometry(), _getMousePos()))
        {
            click();
        }
    }
}
