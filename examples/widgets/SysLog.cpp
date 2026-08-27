// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "SysLog.h"

#include "App.h"

#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>
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
        _textEdit->setReadOnly(true);
        _textEdit->setMarginRole(SizeRole::Margin);

        _logObserver = ListObserver<std::string>::create(
            app->getSysLogModel()->observeLog(),
            [this](const std::vector<std::string>& value)
            {
                // Keep the reading position: setting the text resets the
                // cursor, and the view follows the cursor, so the cursor,
                // the selection, and the scroll are all put back.
                auto model = _textEdit->getModel();
                const TextEditPos cursor = model->getCursor();
                const TextEditSelection selection = model->getSelection();
                const V2I scrollPos = _textEdit->getScrollWidget()->getScrollPos();
                _textEdit->setText(value);
                model->setCursor(cursor);
                model->setSelection(selection);
                _textEdit->getScrollWidget()->setScrollPos(scrollPos);
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
