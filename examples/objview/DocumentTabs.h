// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/TabWidget.h>

namespace objview
{
    class App;
    class ObjView;

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

        //! \name Current View
        ///@{

        const std::shared_ptr<ObjView>& getCurrentView() const;
        std::shared_ptr<ftk::IObservable<std::shared_ptr<ObjView> > > observeCurrentView() const;

        ///@}

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::shared_ptr<ftk::TabWidget> _tabWidget;
        std::map<std::shared_ptr<ftk::IDocument>, std::shared_ptr<ObjView> > _views;
        std::shared_ptr<ftk::Observable<std::shared_ptr<ObjView> > > _currentView;

        std::shared_ptr<ftk::Observer<std::weak_ptr<ftk::IDocument> > > _addObserver;
        std::shared_ptr<ftk::Observer<std::weak_ptr<ftk::IDocument> > > _closeObserver;
        std::shared_ptr<ftk::Observer<bool> > _clearObserver;
        std::shared_ptr<ftk::Observer<std::shared_ptr<ftk::IDocument> > > _currentObserver;
        std::shared_ptr<ftk::Observer<int> > _currentIndexObserver;
    };
}
