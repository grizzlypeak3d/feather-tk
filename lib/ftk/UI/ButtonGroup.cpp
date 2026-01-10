// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ButtonGroup.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

namespace ftk
{
    FTK_ENUM_IMPL(
        ButtonGroupType,
        "Click",
        "Check",
        "Radio",
        "Toggle");

    struct ButtonGroup::Private
    {
        ButtonGroupType type = ButtonGroupType::Click;
        std::vector<std::shared_ptr<IButton> > buttons;
        int radio = -1;
        std::function<void(int)> clickedCallback;
        std::function<void(int, bool)> checkedCallback;
    };

    void ButtonGroup::_init(
        const std::shared_ptr<Context>& context,
        ButtonGroupType type)
    {
        FTK_P();
        p.type = type;
    }

    ButtonGroup::ButtonGroup() :
        _p(new Private)
    {}

    ButtonGroup::~ButtonGroup()
    {}

    std::shared_ptr<ButtonGroup> ButtonGroup::create(
        const std::shared_ptr<Context>& context,
        ButtonGroupType type)
    {
        auto out = std::shared_ptr<ButtonGroup>(new ButtonGroup);
        out->_init(context, type);
        return out;
    }

    const std::vector<std::shared_ptr<IButton> >& ButtonGroup::getButtons() const
    {
        return _p->buttons;
    }

    void ButtonGroup::addButton(const std::shared_ptr<IButton>& button)
    {
        FTK_P();
        switch (p.type)
        {
        case ButtonGroupType::Click:
            button->setCheckable(false);
            break;
        case ButtonGroupType::Check:
            button->setCheckable(true);
            break;
        case ButtonGroupType::Radio:
            button->setCheckable(true);
            break;
        case ButtonGroupType::Toggle:
            button->setCheckable(true);
            break;
        default: break;
        }
        const size_t index = p.buttons.size();
        p.buttons.push_back(button);
        button->setClickedCallback(
            [this, index]
            {
                FTK_P();
                if (p.clickedCallback)
                {
                    p.clickedCallback(index);
                }
            });
        button->setCheckedCallback(
            [this, index](bool value)
            {
                FTK_P();
                switch (p.type)
                {
                case ButtonGroupType::Check:
                    if (p.checkedCallback)
                    {
                        p.checkedCallback(index, value);
                    }
                    break;
                case ButtonGroupType::Radio:
                    p.radio = index;
                    for (size_t i = 0; i < p.buttons.size(); ++i)
                    {
                        p.buttons[i]->setChecked(i == index);
                    }
                    if (p.checkedCallback && index != p.radio)
                    {
                        p.checkedCallback(index, true);
                    }
                    break;
                case ButtonGroupType::Toggle:
                    for (size_t i = 0; i < p.buttons.size(); ++i)
                    {
                        if (i != index)
                        {
                            p.buttons[i]->setChecked(false);
                        }
                    }
                    if (p.checkedCallback)
                    {
                        p.checkedCallback(index, value);
                    }
                    break;
                default: break;
                }
            });
    }

    void ButtonGroup::clearButtons()
    {
        FTK_P();
        p.buttons.clear();
    }

    void ButtonGroup::setChecked(int index, bool value)
    {
        FTK_P();
        switch (p.type)
        {
        case ButtonGroupType::Check:
            if (index >= 0 && index < p.buttons.size())
            {
                p.buttons[index]->setChecked(value);
            }
            break;
        case ButtonGroupType::Radio:
            for (size_t i = 0; i < p.buttons.size(); ++i)
            {
                p.buttons[i]->setChecked(i == index);
            }
            p.radio = index;
            break;
        case ButtonGroupType::Toggle:
            for (size_t i = 0; i < p.buttons.size(); ++i)
            {
                p.buttons[i]->setChecked(i == index ? value : false);
            }
            break;
        default: break;
        }
    }

    void ButtonGroup::setClickedCallback(const std::function<void(int)>& value)
    {
        _p->clickedCallback = value;
    }

    void ButtonGroup::setCheckedCallback(const std::function<void(int, bool)>& value)
    {
        _p->checkedCallback = value;
    }
}
