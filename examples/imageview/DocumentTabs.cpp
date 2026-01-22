// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "DocumentTabs.h"

#include "App.h"
#include "Document.h"
#include "ImageView.h"
#include "SettingsModel.h"

#include <ftk/UI/ScrollWidget.h>

using namespace ftk;

namespace imageview
{
    void DocumentTabs::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::imageview::DocumentTabs", parent);

        _tabWidget = TabWidget::create(context, shared_from_this());
        _tabWidget->setTabsClosable(true);

        _currentView = Observable<std::shared_ptr<ImageView> >::create();

        // Set tab callbacks.
        auto appWeak = std::weak_ptr<App>(app);
        _tabWidget->setCurrentTabCallback(
            [appWeak](int index)
            {
                auto app = appWeak.lock();
                app->getDocumentModel()->setCurrentIndex(index);
            });
        _tabWidget->setTabCloseCallback(
            [appWeak](int index)
            {
                auto app = appWeak.lock();
                app->close(index);
            });

        // Observe when documents are added.
        _addObserver = ftk::Observer<std::weak_ptr<IDocument> >::create(
            app->getDocumentModel()->observeAdd(),
            [this, appWeak](const std::weak_ptr<IDocument>& docWeak)
            {
                if (auto doc = std::dynamic_pointer_cast<Document>(docWeak.lock()))
                {
                    // Create a new view.
                    auto context = getContext();
                    auto app = appWeak.lock();
                    auto view = ImageView::create(context, app, doc);
                    auto scrollWidget = ScrollWidget::create(context);
                    scrollWidget->setBorder(false);
                    scrollWidget->setWidget(view);
                    _tabWidget->addTab(doc->getName(), scrollWidget, doc->getTooltip());
                    view->takeKeyFocus();
                    _views[doc] = scrollWidget;
                }
            });

        // Observe when documents are closed.
        _closeObserver = ftk::Observer<std::weak_ptr<IDocument> >::create(
            app->getDocumentModel()->observeClose(),
            [this](const std::weak_ptr<IDocument>& docWeak)
            {
                if (auto doc = std::dynamic_pointer_cast<Document>(docWeak.lock()))
                {
                    // Remove the view.
                    auto i = _views.find(doc);
                    if (i != _views.end())
                    {
                        _tabWidget->removeTab(i->second);
                        _views.erase(i);
                    }
                }
            });

        // Observe when all the documents are closed.
        _clearObserver = ftk::Observer<bool>::create(
            app->getDocumentModel()->observeCloseAll(),
            [this](bool value)
            {
                if (value)
                {
                    _views.clear();
                    _tabWidget->clear();
                }
            });

        // Observe the current document and update the current view.
        _currentObserver = ftk::Observer<std::shared_ptr<IDocument> >::create(
            app->getDocumentModel()->observeCurrent(),
            [this](const std::shared_ptr<IDocument>& doc)
            {
                auto i = _views.find(doc);
                _currentView->setIfChanged(i != _views.end() ?
                    std::dynamic_pointer_cast<ImageView>(i->second->getWidget()) :
                    nullptr);
            });

        // Observe the current document and update the current tab.
        _currentIndexObserver = ftk::Observer<int>::create(
            app->getDocumentModel()->observeCurrentIndex(),
            [this](int index)
            {
                _tabWidget->setCurrentTab(index);
            });
    }

    DocumentTabs::~DocumentTabs()
    {}

    std::shared_ptr<DocumentTabs> DocumentTabs::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DocumentTabs>(new DocumentTabs);
        out->_init(context, app, parent);
        return out;
    }

    const std::shared_ptr<ImageView>& DocumentTabs::getCurrentView() const
    {
        return _currentView->get();
    }

    std::shared_ptr<ftk::IObservable<std::shared_ptr<ImageView> > > DocumentTabs::observeCurrentView() const
    {
        return _currentView;
    }

    Size2I DocumentTabs::getSizeHint() const
    {
        return _tabWidget->getSizeHint();
    }

    void DocumentTabs::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _tabWidget->setGeometry(value);
    }
}
