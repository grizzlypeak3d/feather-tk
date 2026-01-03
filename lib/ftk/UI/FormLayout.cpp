// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FormLayout.h>

#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/Spacer.h>

namespace ftk
{
    struct FormLayout::Private
    {
        std::vector<std::pair<std::shared_ptr<Label>, std::shared_ptr<IWidget> > > widgets;
        std::shared_ptr<GridLayout> layout;
    };

    void FormLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FormLayout", parent);
        FTK_P();
        p.layout = GridLayout::create(context, shared_from_this());
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
        auto label = Label::create(getContext(), text, p.layout);
        const size_t index = p.widgets.size();
        p.layout->setGridPos(label, index, 0);
        widget->setParent(p.layout);
        p.layout->setGridPos(widget, index, 1);
        p.widgets.push_back({ label, widget });
        return index;
    }

    void FormLayout::removeRow(int index)
    {
        FTK_P();
        if (index >= 0 && index < p.widgets.size())
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
        }
    }

    void FormLayout::removeRow(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        for (size_t i = 0; i < p.widgets.size(); ++i)
        {
            if (value == p.widgets[i].second)
            {
                removeRow(i);
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

    void FormLayout::setRowVisible(int index, bool visible)
    {
        FTK_P();
        if (index >= 0 && index < p.widgets.size())
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
                setRowVisible(i, visible);
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
        const size_t index = p.widgets.size();
        if (auto context = getContext())
        {
            auto spacer = Spacer::create(context, Orientation::Vertical, p.layout);
            spacer->setSpacingRole(value);
            p.layout->setGridPos(spacer, index, 0);
            p.widgets.push_back({ nullptr, spacer });
        }
        return index;
    }
    
    Size2I FormLayout::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void FormLayout::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}