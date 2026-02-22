// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "SysLog.h"

#include "App.h"

#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Splitter.h>

using namespace ftk;

namespace widgets
{
    void SysLog::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "SysLog", parent);

        _textEdit = TextEdit::create(context, shared_from_this());
        _textEdit->setMarginRole(SizeRole::Margin);

        _logObserver = ListObserver<std::string>::create(
            app->getSysLogModel()->observeLog(),
            [this](const std::vector<std::string>& value)
            {
                _textEdit->setText(value);
                _textEdit->getModel()->setCursor(TextEditPos(value.size(), 0));
            });
    }

    SysLog::~SysLog()
    {}

    std::shared_ptr<IWidget> SysLog::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<SysLog>(new SysLog);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I SysLog::getSizeHint() const
    {
        return _textEdit->getSizeHint();
    }

    void SysLog::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _textEdit->setGeometry(value);
    }
}
