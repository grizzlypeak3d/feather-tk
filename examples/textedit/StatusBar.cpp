// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "StatusBar.h"

#include "App.h"
#include "Document.h"

#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

using namespace ftk;

namespace textedit
{
    void StatusBar::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::textedit::StatusBar", parent);

        // Create the labels.
        _linesLabel = Label::create(context);

        // Layout the widgets.
        _layout = HorizontalLayout::create(context, shared_from_this());
        _layout->setMarginRole(SizeRole::MarginInside);
        _layout->setSpacingRole(SizeRole::SpacingSmall);
        _layout->addSpacer(Stretch::Expanding);
        _linesLabel->setParent(_layout);

        // Observe the current document and update the widgets.
        std::weak_ptr<App> appWeak(app);
        _currentObserver = ftk::Observer<std::shared_ptr<IDocument> >::create(
            app->getDocumentModel()->observeCurrent(),
            [this, appWeak](const std::shared_ptr<IDocument>& idoc)
            {
                if (auto doc = std::dynamic_pointer_cast<Document>(idoc))
                {
                    _textObserver = ListObserver<std::string>::create(
                        doc->getModel()->observeText(),
                        [this](const std::vector<std::string>& lines)
                        {
                            _lineCount = lines.size();
                            _textUpdate();
                        });
                    _cursorObserver = Observer<TextEditPos>::create(
                        doc->getModel()->observeCursor(),
                        [this](const TextEditPos& value)
                        {
                            _currentLine = value.line + 1;
                            _currentColumn = value.chr + 1;
                            _textUpdate();
                        });
                }
                else
                {
                    _lineCount = 0;
                    _currentLine = 0;
                    _currentColumn = 0;
                    _textObserver.reset();
                    _textUpdate();
                }
            });
    }

    StatusBar::~StatusBar()
    {}

    std::shared_ptr<StatusBar> StatusBar::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<StatusBar>(new StatusBar);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I StatusBar::getSizeHint() const
    {
        return _layout->getSizeHint();
    }

    void StatusBar::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void StatusBar::_textUpdate()
    {
        _linesLabel->setText(Format("Line: {0} Column: {1} Line count: {2}").
            arg(_currentLine).
            arg(_currentColumn).
            arg(_lineCount));
    }
}
