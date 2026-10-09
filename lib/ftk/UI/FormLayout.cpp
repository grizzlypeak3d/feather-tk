// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FormLayout.h>

#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/Spacer.h>

#include <algorithm>

namespace ftk
{
    struct FormLayout::Private
    {
        std::vector<std::pair<std::shared_ptr<Label>, std::shared_ptr<IWidget> > > widgets;
        std::shared_ptr<GridLayout> layout;
        std::shared_ptr<FormGroup> group;
    };

    FormGroup::FormGroup()
    {}

    FormGroup::~FormGroup()
    {}

    std::shared_ptr<FormGroup> FormGroup::create()
    {
        return std::shared_ptr<FormGroup>(new FormGroup);
    }

    int FormGroup::_getLabelWidth(const SizeHintEvent& event)
    {
        int out = 0;
        auto i = _forms.begin();
        while (i != _forms.end())
        {
            if (auto form = i->lock())
            {
                out = std::max(out, form->_getLabelWidth(event));
                ++i;
            }
            else
            {
                i = _forms.erase(i);
            }
        }
        return out;
    }

    void FormLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FormLayout", parent);
        FTK_P();
        p.layout = GridLayout::create(context);
        _setWidget(p.layout);
    }

    FormLayout::FormLayout() :
        _p(new Private)
    {}

    FormLayout::~FormLayout()
    {}

    std::shared_ptr<FormLayout> FormLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FormLayout>(new FormLayout);
        out->_init(context, parent);
        return out;
    }

    int FormLayout::addRow(const std::string& text, const std::shared_ptr<IWidget>& widget)
    {
        FTK_P();
        const int index = static_cast<int>(p.widgets.size());
        if (auto context = getContext())
        {
            auto label = Label::create(context, text, p.layout);
            p.layout->setGridPos(label, index, 0);
            widget->setParent(p.layout);
            p.layout->setGridPos(widget, index, 1);
            p.widgets.push_back({ label, widget });
        }
        return index;
    }

    void FormLayout::removeRow(int index)
    {
        FTK_P();
        if (index >= 0 && index < static_cast<int>(p.widgets.size()))
        {
            if (p.widgets[index].first)
            {
                p.widgets[index].first->setParent(nullptr);
            }
            if (p.widgets[index].second)
            {
                p.widgets[index].second->setParent(nullptr);
            }
            p.widgets.erase(p.widgets.begin() + index);

            // Renumber the grid positions of the rows after the removed one.
            // Grid rows are assigned from the row count at insertion time, so
            // erasing a row would otherwise leave the following rows at stale
            // grid rows - and the next added row (which uses the new, smaller
            // row count as its grid row) would land on top of one of them.
            // Reassigning keeps the grid rows dense and collision-free.
            for (size_t i = index; i < p.widgets.size(); ++i)
            {
                if (p.widgets[i].first)
                {
                    p.layout->setGridPos(p.widgets[i].first, static_cast<int>(i), 0);
                    p.layout->setGridPos(p.widgets[i].second, static_cast<int>(i), 1);
                }
                else if (p.widgets[i].second)
                {
                    p.layout->setGridPos(p.widgets[i].second, static_cast<int>(i), 0);
                }
            }
        }
    }

    void FormLayout::removeRow(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        for (size_t i = 0; i < p.widgets.size(); ++i)
        {
            if (value == p.widgets[i].second)
            {
                removeRow(static_cast<int>(i));
                break;
            }
        }
    }

    void FormLayout::clear()
    {
        FTK_P();
        p.widgets.clear();
        p.layout->clear();
    }

    void FormLayout::setText(int row, const std::string& text)
    {
        FTK_P();
        if (row >= 0 && row < static_cast<int>(p.widgets.size()) && p.widgets[row].first)
        {
            p.widgets[row].first->setText(text);
        }
    }

    void FormLayout::setText(const std::shared_ptr<IWidget>& widget, const std::string& text)
    {
        FTK_P();
        for (size_t i = 0; i < p.widgets.size(); ++i)
        {
            if (widget == p.widgets[i].second)
            {
                if (p.widgets[i].first)
                {
                    p.widgets[i].first->setText(text);
                }
                break;
            }
        }
    }

    void FormLayout::setRowVisible(int index, bool visible)
    {
        FTK_P();
        if (index >= 0 && index < static_cast<int>(p.widgets.size()))
        {
            if (p.widgets[index].first)
            {
                p.widgets[index].first->setVisible(visible);
            }
            if (p.widgets[index].second)
            {
                p.widgets[index].second->setVisible(visible);
            }
        }
    }

    void FormLayout::setRowVisible(const std::shared_ptr<IWidget>& widget, bool visible)
    {
        FTK_P();
        for (size_t i = 0; i < p.widgets.size(); ++i)
        {
            if (widget == p.widgets[i].second)
            {
                setRowVisible(static_cast<int>(i), visible);
                break;
            }
        }
    }

    SizeRole FormLayout::getMarginRole() const
    {
        return _p->layout->getMarginRole();
    }

    void FormLayout::setMarginRole(SizeRole value)
    {
        _p->layout->setMarginRole(value);
    }

    SizeRole FormLayout::getSpacingRole() const
    {
        return _p->layout->getSpacingRole();
    }

    void FormLayout::setSpacingRole(SizeRole value)
    {
        _p->layout->setSpacingRole(value);
    }

    int FormLayout::addSpacer()
    {
        return addSpacer(_p->layout->getSpacingRole());
    }

    int FormLayout::addSpacer(SizeRole value)
    {
        FTK_P();
        const int index = static_cast<int>(p.widgets.size());
        if (auto context = getContext())
        {
            auto spacer = Spacer::create(context, Orientation::Vertical, p.layout);
            spacer->setSpacingRole(value);
            p.layout->setGridPos(spacer, index, 0);
            p.widgets.push_back({ nullptr, spacer });
        }
        return index;
    }

    const std::shared_ptr<FormGroup>& FormLayout::getGroup() const
    {
        return _p->group;
    }

    void FormLayout::setGroup(const std::shared_ptr<FormGroup>& value)
    {
        FTK_P();
        if (value == p.group)
            return;
        if (p.group)
        {
            auto& forms = p.group->_forms;
            forms.erase(
                std::remove_if(
                    forms.begin(),
                    forms.end(),
                    [this](const std::weak_ptr<FormLayout>& i)
                    {
                        auto form = i.lock();
                        return !form || form.get() == this;
                    }),
                forms.end());
        }
        p.group = value;
        if (p.group)
        {
            p.group->_forms.push_back(
                std::dynamic_pointer_cast<FormLayout>(shared_from_this()));
        }
        else
        {
            p.layout->setColumnMinWidth(0, 0);
        }
        setSizeUpdate();
    }

    void FormLayout::sizeHintEvent(const SizeHintEvent& event)
    {
        IContainer::sizeHintEvent(event);
        FTK_P();
        if (p.group)
        {
            // The whole of the group's answer, at once: it does not wait
            // for the other forms to be sized, so this form is laid out at
            // the shared width the first time, wherever it comes among
            // them.
            p.layout->setColumnMinWidth(0, p.group->_getLabelWidth(event));
        }
    }

    int FormLayout::_getLabelWidth(const SizeHintEvent& event)
    {
        FTK_P();
        int out = 0;
        for (const auto& i : p.widgets)
        {
            // Hidden rows do not count, as in a form of its own. A label is
            // sized here if it has not been yet, by its own measure.
            if (i.first && i.first->isVisible(false))
            {
                i.first->sizeHintEvent(event);
                out = std::max(out, i.first->getSizeHint().w);
            }
        }
        return out;
    }

    void setFormGroup(
        const std::shared_ptr<IWidget>& widget,
        const std::shared_ptr<FormGroup>& group)
    {
        if (!widget)
            return;
        if (auto form = std::dynamic_pointer_cast<FormLayout>(widget))
        {
            form->setGroup(group);
        }
        for (const auto& child : widget->getChildren())
        {
            setFormGroup(child, group);
        }
    }
}
