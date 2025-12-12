// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableList.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    class Context;

    //! Base class for documents.
    class FTK_API_TYPE IDocument : public std::enable_shared_from_this<IDocument>
    {
    public:
        FTK_API virtual ~IDocument() = 0;
    };

    //! Document model.
    class FTK_API_TYPE DocumentModel : public std::enable_shared_from_this<DocumentModel>
    {
    protected:
        void _init(const std::shared_ptr<ftk::Context>&);

        DocumentModel() = default;

    public:
        FTK_API virtual ~DocumentModel();

        //! Create a new document model.
        FTK_API static std::shared_ptr<DocumentModel> create(
            const std::shared_ptr<ftk::Context>&);

        //! \name Documents
        ///@{

        FTK_API const std::vector<std::shared_ptr<IDocument> >& getList() const;
        FTK_API std::shared_ptr<ftk::IObservableList<std::shared_ptr<IDocument> > > observeList() const;

        ///@}

        //! \name Add Documents
        ///@{

        FTK_API void add(const std::shared_ptr<IDocument>&);
        FTK_API std::shared_ptr<ftk::IObservable<std::weak_ptr<IDocument> > > observeAdd() const;

        ///@}

        //! \name Close Documents
        ///@{

        FTK_API void close(int);
        FTK_API void closeAll();
        FTK_API std::shared_ptr<ftk::IObservable<std::weak_ptr<IDocument> > > observeClose() const;
        FTK_API std::shared_ptr<ftk::IObservable<bool> > observeCloseAll() const;

        ///@}

        //! \name Current Document
        ///@{

        FTK_API std::shared_ptr<IDocument> getCurrent() const;
        FTK_API int getCurrentIndex() const;
        FTK_API std::shared_ptr<ftk::IObservable<std::shared_ptr<IDocument> > > observeCurrent() const;
        FTK_API std::shared_ptr<ftk::IObservable<int> > observeCurrentIndex() const;
        FTK_API void setCurrentIndex(int);

        ///@}

    private:
        std::shared_ptr<ftk::ObservableList<std::shared_ptr<IDocument> > > _documents;
        std::shared_ptr<ftk::Observable<std::weak_ptr<IDocument> > > _add;
        std::shared_ptr<ftk::Observable<std::weak_ptr<IDocument> > > _close;
        std::shared_ptr<ftk::Observable<bool> > _closeAll;
        std::shared_ptr<ftk::Observable<std::shared_ptr<IDocument> > > _current;
        std::shared_ptr<ftk::Observable<int> > _currentIndex;
    };
}
