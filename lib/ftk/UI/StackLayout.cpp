// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/StackLayout.h>

#include <optional>

namespace ftk
{
    struct StackLayout::Private
    {
        int currentIndex = 0;
        SizeRole marginRole = SizeRole::None;
        std::shared_ptr<Observable<bool> > hasNextIndex;
        std::shared_ptr<Observable<bool> > hasPrevIndex;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
        };
        SizeData size;
    };

    void StackLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::StackLayout", parent);
        FTK_P();
        p.hasNextIndex = Observable<bool>::create(false);
        p.hasPrevIndex = Observable<bool>::create(false);
    }

    StackLayout::StackLayout() :
        _p(new Private)
    {}

    StackLayout::~StackLayout()
    {}

    std::shared_ptr<StackLayout> StackLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<StackLayout>(new StackLayout);
        out->_init(context, parent);
        return out;
    }

    int StackLayout::getCurrentIndex() const
    {
        return _p->currentIndex;
    }

    void StackLayout::setCurrentIndex(int value)
    {
        FTK_P();
        const int tmp = clamp(value, 0, static_cast<int>(getChildren().size()) - 1);
        if (tmp == p.currentIndex)
            return;
        p.currentIndex = tmp;
        _widgetUpdate();
    }

    void StackLayout::nextIndex()
    {
        setCurrentIndex(_p->currentIndex + 1);
    }

    std::shared_ptr<IObservable<bool> > StackLayout::observeHasNextIndex() const
    {
        return _p->hasNextIndex;
    }

    void StackLayout::prevIndex()
    {
        setCurrentIndex(_p->currentIndex - 1);
    }

    std::shared_ptr<IObservable<bool> > StackLayout::observeHasPrevIndex() const
    {
        return _p->hasPrevIndex;
    }

    void StackLayout::firstIndex()
    {
        setCurrentIndex(0);
    }

    void StackLayout::lastIndex()
    {
        setCurrentIndex(static_cast<int>(getChildren().size()) - 1);
    }

    void StackLayout::setCurrentWidget(const std::shared_ptr<IWidget>& value)
    {
        int index = -1;
        const auto& children = getChildren();
        int j = 0;
        for (auto i = children.begin(); i != children.end(); ++i, ++j)
        {
            if (value == *i)
            {
                index = j;
                break;
            }
        }
        setCurrentIndex(index);
    }

    void StackLayout::clear()
    {
        auto children = getChildren();
        for (auto child : children)
        {
            child->setParent(nullptr);
        }
    }

    SizeRole StackLayout::getMarginRole() const
    {
        return _p->marginRole;
    }

    void StackLayout::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    Size2I StackLayout::getSizeHint() const
    {
        FTK_P();
        Size2I out;
        for (const auto& child : getChildren())
        {
            const Size2I& childSizeHint = child->getSizeHint();
            out.w = std::max(out.w, childSizeHint.w);
            out.h = std::max(out.h, childSizeHint.h);
        }
        out.w += p.size.margin * 2;
        out.h += p.size.margin * 2;
        return out;
    }

    void StackLayout::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        const Box2I g = margin(getGeometry(), -p.size.margin);
        for (const auto& child : getChildren())
        {
            child->setGeometry(g);
        }
    }

    Box2I StackLayout::getChildrenClipRect() const
    {
        return margin(getGeometry(), -_p->size.margin);
    }

    void StackLayout::childAddEvent(const ChildAddEvent& event)
    {
        IWidget::childAddEvent(event);
        _widgetUpdate();
    }

    void StackLayout::childRemoveEvent(const ChildRemoveEvent& event)
    {
        IWidget::childRemoveEvent(event);
        FTK_P();
        p.currentIndex = clamp(
            p.currentIndex,
            0,
            static_cast<int>(getChildren().size()) - 1);
        _widgetUpdate();
    }

    void StackLayout::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
        }
    }

    void StackLayout::_widgetUpdate()
    {
        FTK_P();
        const auto& children = getChildren();
        int j = 0;
        for (auto i = children.begin(); i != children.end(); ++i, ++j)
        {
            (*i)->setVisible(j == p.currentIndex);
        }
        p.hasNextIndex->setIfChanged(p.currentIndex < (static_cast<int>(children.size()) - 1));
        p.hasPrevIndex->setIfChanged(p.currentIndex > 0);
        setSizeUpdate();
        setDrawUpdate();
    }
}
