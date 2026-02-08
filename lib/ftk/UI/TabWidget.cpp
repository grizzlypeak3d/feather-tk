// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/TabWidget.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/StackLayout.h>
#include <ftk/UI/TabBar.h>

namespace ftk
{
    struct TabWidget::Private
    {
        std::shared_ptr<TabBar> tabs;
        std::shared_ptr<StackLayout> stack;
        std::shared_ptr<RowLayout> layout;
        std::function<void(int)> callback;
        std::function<void(const std::shared_ptr<IWidget>&)> widgetCallback;
    };

    void TabWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::TabWidget", parent);
        FTK_P();

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::None);

        p.tabs = TabBar::create(context, p.layout);

        Divider::create(context, Orientation::Vertical, p.layout);

        p.stack = StackLayout::create(context, p.layout);
        p.stack->setVStretch(Stretch::Expanding);

        _widgetUpdate();

        p.tabs->setCallback(
            [this](int value)
            {
                FTK_P();
                _widgetUpdate();
                if (p.callback)
                {
                    p.callback(value);
                }
                if (p.widgetCallback)
                {
                    const auto& children = p.stack->getChildren();
                    auto i = children.begin();
                    for (int j = 0; j < value; ++j, ++i)
                        ;
                    p.widgetCallback(i != children.end() ? *i : nullptr);
                }
            });
    }

    TabWidget::TabWidget() :
        _p(new Private)
    {}

    TabWidget::~TabWidget()
    {}

    std::shared_ptr<TabWidget> TabWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<TabWidget>(new TabWidget);
        out->_init(context, parent);
        return out;
    }

    const std::vector<std::string>& TabWidget::getTabs() const
    {
        return _p->tabs->getTabs();
    }

    void TabWidget::addTab(
        const std::string& value,
        const std::shared_ptr<IWidget>& widget,
        const std::string& tooltip)
    {
        FTK_P();
        p.tabs->addTab(value, tooltip);
        widget->setParent(p.stack);
        _widgetUpdate();
    }

    void TabWidget::removeTab(int index)
    {
        FTK_P();
        p.tabs->removeTab(index);
        auto children = p.stack->getChildren();
        int i = 0;
        for (auto j = children.begin(); j != children.end(); ++j, ++i)
        {
            if (i == index)
            {
                (*j)->setParent(nullptr);
                break;
            }
        }
    }

    void TabWidget::removeTab(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        auto children = p.stack->getChildren();
        int i = 0;
        for (auto j = children.begin(); j != children.end(); ++j, ++i)
        {
            if (*j == value)
            {
                (*j)->setParent(nullptr);
                break;
            }
        }
        p.tabs->removeTab(i);
    }

    void TabWidget::clear()
    {
        FTK_P();
        p.tabs->clear();
        auto children = p.stack->getChildren();
        for (const auto& child : children)
        {
            child->setParent(nullptr);
        }
    }

    int TabWidget::getCurrent() const
    {
        return _p->tabs->getCurrent();
    }

    void TabWidget::setCurrent(int value)
    {
        FTK_P();
        p.tabs->setCurrent(value);
        _widgetUpdate();
    }

    void TabWidget::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    std::shared_ptr<IWidget> TabWidget::getCurrentWidget() const
    {
        FTK_P();
        std::shared_ptr<IWidget> out;
        const auto& children = p.stack->getChildren();
        int i = 0;
        for (auto j = children.begin(); j != children.end(); ++j, ++i)
        {
            if (i == p.tabs->getCurrent())
            {
                out = *j;
                break;
            }
        }
        return out;
    }

    void TabWidget::setCurrentWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        const auto& children = p.stack->getChildren();
        int i = 0;
        for (auto j = children.begin(); j != children.end(); ++j, ++i)
        {
            if (*j == value)
            {
                setCurrent(i);
                break;
            }
        }
    }

    void TabWidget::setWidgetCallback(const std::function<void(const std::shared_ptr<IWidget>&)>& value)
    {
        _p->widgetCallback = value;
    }

    void TabWidget::setText(int index, const std::string& text)
    {
        _p->tabs->setText(index, text);
    }

    void TabWidget::setText(const std::shared_ptr<IWidget>& widget, const std::string& text)
    {
        FTK_P();
        const auto& children = p.stack->getChildren();
        int i = 0;
        for (auto j = children.begin(); j != children.end(); ++j, ++i)
        {
            if (*j == widget)
            {
                break;
            }
        }
        setText(i, text);
    }

    void TabWidget::setTabTooltip(int index, const std::string& tooltip)
    {
        _p->tabs->setTabTooltip(index, tooltip);
    }

    void TabWidget::setTabTooltip(const std::shared_ptr<IWidget>& widget, const std::string& tooltip)
    {
        FTK_P();
        const auto& children = p.stack->getChildren();
        int i = 0;
        for (auto j = children.begin(); j != children.end(); ++j, ++i)
        {
            if (*j == widget)
            {
                break;
            }
        }
        setTabTooltip(i, tooltip);
    }

    bool TabWidget::isClosable() const
    {
        return _p->tabs->isClosable();
    }

    void TabWidget::setClosable(bool value)
    {
        _p->tabs->setClosable(value);
    }

    void TabWidget::setCloseCallback(const std::function<void(int)>& value)
    {
        _p->tabs->setCloseCallback(value);
    }
    
    Size2I TabWidget::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void TabWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void TabWidget::_widgetUpdate()
    {
        FTK_P();
        const int index = p.tabs->getCurrent();
        p.stack->setCurrentIndex(index);
    }
}
