// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Divider.h>

#include <optional>

namespace ftk
{
    struct Divider::Private
    {
        struct SizeData
        {
            bool init = true;
            Size2I sizeHint;
        };
        SizeData size;
    };

    void Divider::_init(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::Divider", parent);
        setBackgroundRole(ColorRole::Border);
        switch (orientation)
        {
        case Orientation::Horizontal:
            setVStretch(Stretch::Expanding);
            break;
        case Orientation::Vertical:
            setHStretch(Stretch::Expanding);
            break;
        default: break;
        }
    }

    Divider::Divider() :
        _p(new Private)
    {}

    Divider::~Divider()
    {}

    std::shared_ptr<Divider> Divider::create(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Divider>(new Divider);
        out->_init(context, orientation, parent);
        return out;
    }

    Size2I Divider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void Divider::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void Divider::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.sizeHint.w = p.size.sizeHint.h =
                event.style->getSizeRole(SizeRole::Border, event.displayScale);
        }
    }

    void updateDividers(const std::shared_ptr<IWidget>& parent)
    {
        std::shared_ptr<IWidget> pending;
        bool before = false;
        for (const auto& child : parent->getChildren())
        {
            if (std::dynamic_pointer_cast<Divider>(child))
            {
                child->setVisible(false);
                if (before && !pending)
                {
                    pending = child;
                }
            }
            else if (child->isVisible(false))
            {
                if (pending)
                {
                    pending->setVisible(true);
                    pending.reset();
                }
                before = true;
            }
        }
    }
}
