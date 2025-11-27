// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "StatusBar.h"

#include "App.h"
#include "Document.h"

#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

using namespace ftk;

namespace imageview
{
    void StatusBar::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::imageview::StatusBar", parent);

        // Create the labels.
        _labels["Info"] = Label::create(context);

        // Layout the widgets.
        _layout = HorizontalLayout::create(context, shared_from_this());
        _layout->setMarginRole(SizeRole::MarginInside);
        _layout->setSpacingRole(SizeRole::SpacingSmall);
        _layout->addSpacer(Stretch::Expanding);
        _labels["Info"]->setParent(_layout);

        // Observe the current document and update the widgets.
        std::weak_ptr<App> appWeak(app);
        _currentObserver = ftk::ValueObserver<std::shared_ptr<IDocument> >::create(
            app->getDocumentModel()->observeCurrent(),
            [this, appWeak](const std::shared_ptr<IDocument>& idoc)
            {
                std::string text;
                if (auto doc = std::dynamic_pointer_cast<Document>(idoc))
                {
                    if (auto image = doc->getImage())
                    {
                        const Size2I& size = image->getSize();
                        text = Format("Size: {0}x{1}, Aspect ratio: {2}, Type: {3}").
                            arg(size.w).
                            arg(size.h).
                            arg(image->getAspect(), 2).
                            arg(image->getType());
                    }
                }
                _labels["Info"]->setText(text);
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

    void StatusBar::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void StatusBar::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_layout->getSizeHint());
    }
}
