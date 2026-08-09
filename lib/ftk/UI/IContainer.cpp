// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IContainer.h>

namespace ftk
{
    void IContainer::_init(
        const std::shared_ptr<Context>& context,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, objectName, parent);
    }

    IContainer::IContainer()
    {}

    IContainer::~IContainer()
    {}

    const std::shared_ptr<IWidget>& IContainer::getWidget() const
    {
        return _widget;
    }

    void IContainer::_setWidget(const std::shared_ptr<IWidget>& value)
    {
        if (value == _widget)
            return;
        if (_widget)
        {
            _widget->setParent(nullptr);
        }
        _widget = value;
        if (_widget)
        {
            _widget->setParent(shared_from_this());
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    Size2I IContainer::getSizeHint() const
    {
        return _widget ? _widget->getSizeHint() : Size2I();
    }

    void IContainer::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        if (_widget)
        {
            _widget->setGeometry(value);
        }
    }
}
