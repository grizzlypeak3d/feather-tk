// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Splitters.h"

#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/Splitter.h>

using namespace ftk;

namespace widgets
{
    void Splitters::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Splitters", parent);

        // Create splitters. The splitter gives each pane's geometry to its
        // child directly, so each label is centered by a layout in between:
        // the layout centers it across, and the spacers center it along.
        _splitter = Splitter::create(context, Orientation::Vertical, shared_from_this());
        auto splitter2 = Splitter::create(context, Orientation::Horizontal, _splitter);
        for (const auto& text : { "One", "Two", "Three" })
        {
            auto layout = VerticalLayout::create(
                context,
                "Three" == text ?
                    std::static_pointer_cast<IWidget>(_splitter) :
                    std::static_pointer_cast<IWidget>(splitter2));
            auto spacer = Spacer::create(context, Orientation::Vertical, layout);
            spacer->setStretch(Stretch::Expanding);
            auto label = Label::create(context, text, layout);
            label->setFontSize(32);
            label->setHAlign(HAlign::Center);
            spacer = Spacer::create(context, Orientation::Vertical, layout);
            spacer->setStretch(Stretch::Expanding);
        }
    }

    Splitters::~Splitters()
    {}

    std::shared_ptr<IWidget> Splitters::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Splitters>(new Splitters);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Splitters::getSizeHint() const
    {
        return _splitter->getSizeHint();
    }

    void Splitters::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _splitter->setGeometry(value);
    }
}
