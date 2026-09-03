// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ItemButtonList.h>

#include <ftk/UI/DrawUtil.h>

#include <ftk/Core/Math.h>

#include <algorithm>

namespace ftk
{
    struct ItemButtonList::Private
    {
        int current = -1;
        std::function<void(int)> currentCallback;
        std::function<void(int)> activateCallback;
        std::function<void(int)> deleteCallback;

        struct SizeData
        {
            bool init = true;
            int keyFocus = 0;
        };
        SizeData size;
    };

    void ItemButtonList::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        VerticalLayout::_init(context, parent);
        setObjectName("ftk::ItemButtonList");
        setAcceptsKeyFocus(true);
    }

    ItemButtonList::ItemButtonList() :
        _p(new Private)
    {}

    ItemButtonList::~ItemButtonList()
    {}

    std::shared_ptr<ItemButtonList> ItemButtonList::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ItemButtonList>(new ItemButtonList);
        out->_init(context, parent);
        return out;
    }

    int ItemButtonList::getCurrent() const
    {
        return _p->current;
    }

    void ItemButtonList::setCurrent(int value)
    {
        _setCurrent(value, false);
    }

    void ItemButtonList::setCurrentCallback(const std::function<void(int)>& value)
    {
        _p->currentCallback = value;
    }

    void ItemButtonList::setActivateCallback(const std::function<void(int)>& value)
    {
        _p->activateCallback = value;
    }

    void ItemButtonList::setDeleteCallback(const std::function<void(int)>& value)
    {
        _p->deleteCallback = value;
    }

    std::vector<std::shared_ptr<ItemButton> > ItemButtonList::getItems() const
    {
        std::vector<std::shared_ptr<ItemButton> > out;
        for (const auto& child : getChildren())
        {
            if (auto item = std::dynamic_pointer_cast<ItemButton>(child))
            {
                out.push_back(item);
            }
        }
        return out;
    }

    void ItemButtonList::_rowPress(const std::shared_ptr<ItemButton>& row)
    {
        takeKeyFocus();
        const auto items = getItems();
        const auto i = std::find(items.begin(), items.end(), row);
        if (i != items.end())
        {
            _setCurrent(static_cast<int>(i - items.begin()), true);
        }
    }

    void ItemButtonList::_setCurrent(int value, bool callback)
    {
        FTK_P();
        const int size = static_cast<int>(getItems().size());
        value = size > 0 ? clamp(value, 0, size - 1) : -1;
        if (value == p.current)
            return;
        p.current = value;
        setDrawUpdate();
        if (callback && p.currentCallback && p.current >= 0)
        {
            p.currentCallback(p.current);
        }
    }

    void ItemButtonList::drawOverlayEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        VerticalLayout::drawOverlayEvent(drawRect, event);
        FTK_P();
        // The current item wears the focus ring while the list holds the
        // keyboard, the way a menu marks its current item: the list only
        // gains the focus from being used -- a row clicked, the arrows
        // entering it, Tab -- so this is never a ring nobody asked for.
        if (hasKeyFocus() && p.current >= 0)
        {
            const auto items = getItems();
            if (p.current < static_cast<int>(items.size()))
            {
                event.render->drawMesh(
                    border(
                        items[p.current]->getGeometry(),
                        p.size.keyFocus),
                    event.style->getColorRole(ColorRole::KeyFocus));
            }
        }
    }

    void ItemButtonList::sizeHintEvent(const SizeHintEvent& event)
    {
        VerticalLayout::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.keyFocus = event.style->getSizeRole(
                SizeRole::KeyFocus, event.displayScale);
        }
    }

    void ItemButtonList::keyPressEvent(KeyEvent& event)
    {
        VerticalLayout::keyPressEvent(event);
        FTK_P();
        if (event.accept || 0 != event.modifiers)
        {
            return;
        }
        const int size = static_cast<int>(getItems().size());
        switch (event.key)
        {
        case Key::Up:
        case Key::Down:
            if (size > 0)
            {
                event.accept = true;
                // Without the focus the arrows enter the list at its
                // current item; the keys reach here through the hover
                // chain, so the cursor is over the list.
                takeKeyFocus();
                const int step = Key::Up == event.key ? -1 : 1;
                _setCurrent(
                    p.current >= 0 ? p.current + step :
                        (step > 0 ? 0 : size - 1),
                    true);
            }
            break;
        case Key::Home:
            if (size > 0)
            {
                event.accept = true;
                takeKeyFocus();
                _setCurrent(0, true);
            }
            break;
        case Key::End:
            if (size > 0)
            {
                event.accept = true;
                takeKeyFocus();
                _setCurrent(size - 1, true);
            }
            break;
        case Key::Return:
            if (hasKeyFocus() && p.current >= 0)
            {
                event.accept = true;
                if (p.activateCallback)
                {
                    p.activateCallback(p.current);
                }
            }
            break;
        case Key::Delete:
        case Key::Backspace:
            if (hasKeyFocus() && p.current >= 0)
            {
                event.accept = true;
                if (p.deleteCallback)
                {
                    p.deleteCallback(p.current);
                }
            }
            break;
        case Key::Escape:
            if (hasKeyFocus() && showKeyFocus())
            {
                event.accept = true;
                releaseKeyFocus();
            }
            break;
        default: break;
        }
    }

    void ItemButtonList::keyReleaseEvent(KeyEvent& event)
    {
        VerticalLayout::keyReleaseEvent(event);
        event.accept = true;
    }
}
