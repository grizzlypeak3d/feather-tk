// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TabWidget.h>
#include <ftk/UI/TextEdit.h>

namespace textedit
{
    class App;

    //! Document tabs widget.
    class DocumentTabs : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        DocumentTabs() = default;

    public:
        virtual ~DocumentTabs();

        //! Create a new widget.
        static std::shared_ptr<DocumentTabs> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        void setGeometry(const ftk::Box2I&) override;
        void sizeHintEvent(const ftk::SizeHintEvent&) override;

    private:
        std::shared_ptr<ftk::TabWidget> _tabWidget;
        std::map<std::shared_ptr<ftk::IDocument>, std::shared_ptr<ftk::TextEdit> > _textEdits;

        std::shared_ptr<ftk::Observer<std::weak_ptr<ftk::IDocument> > > _addObserver;
        std::shared_ptr<ftk::Observer<std::weak_ptr<ftk::IDocument> > > _closeObserver;
        std::shared_ptr<ftk::Observer<bool> > _clearObserver;
        std::shared_ptr<ftk::Observer<int> > _currentObserver;
        std::map<std::shared_ptr<ftk::IDocument>, std::shared_ptr<ftk::Observer<std::string> > > _nameObservers;
        std::map<std::shared_ptr<ftk::IDocument>, std::shared_ptr<ftk::Observer<std::string> > > _tooltipObservers;
        std::shared_ptr<ftk::Observer<ftk::TextEditOptions> > _textEditOptionsObserver;
        std::shared_ptr<ftk::Observer<ftk::TextEditModelOptions> > _textEditModelOptionsObserver;
    };
}
