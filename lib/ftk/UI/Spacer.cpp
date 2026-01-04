// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Spacer.h>

#include <optional>

namespace ftk
{
    struct Spacer::Private
    {
        Orientation orientation = Orientation::Horizontal;
        SizeRole spacingRole = SizeRole::Spacing;

        struct SizeData
        {
            std::optional<float> displayScale;
            Size2I sizeHint;
        };
        SizeData size;
    };

    void Spacer::_init(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::Spacer", parent);
        FTK_P();
        p.orientation = orientation;
    }

    Spacer::Spacer() :
        _p(new Private)
    {}

    Spacer::~Spacer()
    {}

    std::shared_ptr<Spacer> Spacer::create(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Spacer>(new Spacer);
        out->_init(context, orientation, parent);
        return out;
    }

    SizeRole Spacer::getSpacingRole() const
    {
        return _p->spacingRole;
    }

    void Spacer::setSpacingRole(SizeRole value)
    {
        FTK_P();
        if (value == p.spacingRole)
            return;
        p.spacingRole = value;
        p.size.displayScale.reset();
        setSizeUpdate();
    }
    
    Size2I Spacer::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void Spacer::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            const int size = event.style->getSizeRole(p.spacingRole, event.displayScale);

            p.size.sizeHint = Size2I();
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                p.size.sizeHint.w = size;
                break;
            case Orientation::Vertical:
                p.size.sizeHint.h = size;
                break;
            default: break;
            }
        }
    }
}