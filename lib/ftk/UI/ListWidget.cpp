// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ListWidget.h>

#include <ftk/UI/ListItemsWidget.h>

#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    struct ListWidget::Private
    {
        std::shared_ptr<ListItemsWidget> widget;
        std::shared_ptr<ScrollWidget> scrollWidget;
        std::shared_ptr<Observer<int> > scrollToObserver;
    };

    void ListWidget::_init(
        const std::shared_ptr<Context>& context,
        ButtonGroupType type,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ListWidget", parent);
        FTK_P();

        p.widget = ListItemsWidget::create(context, type);

        p.scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        p.scrollWidget->setWidget(p.widget);

        p.scrollToObserver = Observer<int>::create(
            p.widget->observeScrollTo(),
            [this](int value)
            {
                if (value >= 0)
                {
                    _scrollUpdate(value);
                }
            });
    }

    ListWidget::ListWidget() :
        _p(new Private)
    {}

    ListWidget::~ListWidget()
    {}

    std::shared_ptr<ListWidget> ListWidget::create(
        const std::shared_ptr<Context>& context,
        ButtonGroupType type,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ListWidget>(new ListWidget);
        out->_init(context, type, parent);
        return out;
    }

    const std::vector<ListItem>& ListWidget::getItems() const
    {
        return _p->widget->getItems();
    }

    void ListWidget::setItems(const std::vector<ListItem>& value)
    {
        _p->widget->setItems(value);
    }

    void ListWidget::setItems(const std::vector<std::string>& value)
    {
        _p->widget->setItems(value);
    }

    bool ListWidget::getChecked(int index) const
    {
        return _p->widget->getChecked(index);
    }

    void ListWidget::setChecked(int index, bool value)
    {
        _p->widget->setChecked(index, value);
    }

    void ListWidget::setCallback(const std::function<void(int, bool)>& value)
    {
        _p->widget->setCallback(value);
    }

    int ListWidget::getCurrent() const
    {
        return _p->widget->getCurrent();
    }

    void ListWidget::setCurrent(int value)
    {
        _p->widget->setCurrent(value);
    }

    const std::string& ListWidget::getSearch() const
    {
        return _p->widget->getSearch();
    }

    void ListWidget::setSearch(const std::string& value)
    {
        _p->widget->setSearch(value);
    }

    void ListWidget::clearSearch()
    {
        _p->widget->clearSearch();
    }

    void ListWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->scrollWidget->setGeometry(value);
    }

    void ListWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_p->scrollWidget->getSizeHint());
    }

    void ListWidget::_scrollUpdate(int value)
    {
        FTK_P();
        const V2I& pos = _p->scrollWidget->getScrollPos();
        const Size2I vp = _p->scrollWidget->getScrollInfo().viewport.size();
        const Box2I r = _p->widget->getRect(value);
        if (r.min.y < pos.y || r.max.y > pos.y + vp.h)
        {
            _p->scrollWidget->scrollTo(r);
        }
    }
}
