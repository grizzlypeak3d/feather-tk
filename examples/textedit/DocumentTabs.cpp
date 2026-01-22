// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "DocumentTabs.h"

#include "App.h"
#include "Document.h"
#include "SettingsModel.h"

using namespace ftk;

namespace textedit
{
    void DocumentTabs::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::textedit::DocumentTabs", parent);

        _tabWidget = TabWidget::create(context, shared_from_this());
        _tabWidget->setTabsClosable(true);

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
            [this, appWeak](const std::weak_ptr<IDocument>& idoc)
            {
                if (auto doc = std::dynamic_pointer_cast<Document>(idoc.lock()))
                {
                    // Create a new text editor.
                    auto context = getContext();
                    auto app = appWeak.lock();
                    auto textEdit = TextEdit::create(context, doc->getModel());
                    textEdit->setOptions(app->getSettingsModel()->getTextEditOptions());
                    textEdit->getModel()->setOptions(app->getSettingsModel()->getTextEditModelOptions());
                    textEdit->setMarginRole(SizeRole::MarginSmall);
                    _textEdits[doc] = textEdit;
                    _tabWidget->addTab(std::string(), textEdit);
                    textEdit->takeKeyFocus();

                    // Observe the document to update the tab text and tooltip.
                    _nameObservers[doc] = Observer<std::string>::create(
                        doc->observeName(),
                        [this, textEdit](const std::string& value)
                        {
                            _tabWidget->setTabText(textEdit, value);
                        });
                    _tooltipObservers[doc] = Observer<std::string>::create(
                        doc->observeTooltip(),
                        [this, textEdit](const std::string& value)
                        {
                            _tabWidget->setTabTooltip(textEdit, value);
                        });
                }
            });

        // Observe when documents are closed.
        _closeObserver = ftk::Observer<std::weak_ptr<IDocument> >::create(
            app->getDocumentModel()->observeClose(),
            [this](const std::weak_ptr<IDocument>& idoc)
            {
                if (auto doc = std::dynamic_pointer_cast<Document>(idoc.lock()))
                {
                    // Remove the text editor.
                    auto i = _textEdits.find(doc);
                    if (i != _textEdits.end())
                    {
                        _tabWidget->removeTab(i->second);
                        _textEdits.erase(i);
                    }

                    // Remove the observers.
                    auto j = _nameObservers.find(doc);
                    if (j != _nameObservers.end())
                    {
                        _nameObservers.erase(j);
                    }
                    auto k = _tooltipObservers.find(doc);
                    if (k != _tooltipObservers.end())
                    {
                        _tooltipObservers.erase(k);
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
                    _textEdits.clear();
                    _tabWidget->clear();
                    _nameObservers.clear();
                    _tooltipObservers.clear();
                }
            });

        // Observe the current document and update the current tab.
        _currentObserver = ftk::Observer<int>::create(
            app->getDocumentModel()->observeCurrentIndex(),
            [this, appWeak](int index)
            {
                _tabWidget->setCurrentTab(index);
            });

        // Observe text editor options.
        _textEditOptionsObserver = Observer<TextEditOptions>::create(
            app->getSettingsModel()->observeTextEditOptions(),
            [this](const TextEditOptions& value)
            {
                for (auto i : _textEdits)
                {
                    i.second->setOptions(value);
                }
            });
        _textEditModelOptionsObserver = Observer<TextEditModelOptions>::create(
            app->getSettingsModel()->observeTextEditModelOptions(),
            [this](const TextEditModelOptions& value)
            {
                for (auto i : _textEdits)
                {
                    i.second->getModel()->setOptions(value);
                }
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
