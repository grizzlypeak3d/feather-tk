// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Splitters.h"

#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Splitter.h>

using namespace ftk;

namespace widgets
{
    void Splitters::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Splitters", nullptr);

        // Create splitters.
        _splitter = Splitter::create(context, Orientation::Vertical, shared_from_this());
        auto splitter2 = Splitter::create(context, Orientation::Horizontal, _splitter);
        auto label = Label::create(context, "Label 1", splitter2);
        label->setAlign(HAlign::Center, VAlign::Center);
        label = Label::create(context, "Label 2", splitter2);
        label->setAlign(HAlign::Center, VAlign::Center);
        label = Label::create(context, "Label 3", _splitter);
        label->setAlign(HAlign::Center, VAlign::Center);
    }

    Splitters::~Splitters()
    {}

    std::shared_ptr<Splitters> Splitters::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Splitters>(new Splitters);
        out->_init(context);
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
