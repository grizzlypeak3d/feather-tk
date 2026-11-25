// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/Core/ObservableList.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    class Context;

    //! Base class for documents.
    class FTK_UI_API_TYPE IDocument : public std::enable_shared_from_this<IDocument>
    {
    public:
        FTK_UI_API virtual ~IDocument() = 0;
    };

    //! Document model.
    class FTK_UI_API_TYPE DocumentModel : public std::enable_shared_from_this<DocumentModel>
    {
    protected:
        void _init(const std::shared_ptr<ftk::Context>&);

        DocumentModel() = default;

    public:
        FTK_UI_API virtual ~DocumentModel();

        //! Create a new document model.
        FTK_UI_API static std::shared_ptr<DocumentModel> create(
            const std::shared_ptr<ftk::Context>&);

        //! \name Documents
        ///@{

        //! Get the documents.
        FTK_UI_API const std::vector<std::shared_ptr<IDocument> >& getList() const;
        //! Observe the documents.
        FTK_UI_API std::shared_ptr<ftk::IObservableList<std::shared_ptr<IDocument> > > observeList() const;

        ///@}

        //! \name Add Documents
        ///@{

        //! Add a document. It becomes the current document.
        FTK_UI_API void add(const std::shared_ptr<IDocument>&);
        //! Observe the documents as they are added.
        FTK_UI_API std::shared_ptr<ftk::IObservable<std::weak_ptr<IDocument> > > observeAdd() const;

        ///@}

        //! \name Close Documents
        ///@{

        //! Close the document at the given index; an index out of range is
        //! ignored. Closing the current document makes the next one current, or
        //! the last one when it was the last.
        FTK_UI_API void close(int);
        //! Close all of the documents. This sends observeCloseAll(), not
        //! observeClose() for each.
        FTK_UI_API void closeAll();
        //! Observe the documents as close() closes them: each one is sent
        //! before it is removed.
        FTK_UI_API std::shared_ptr<ftk::IObservable<std::weak_ptr<IDocument> > > observeClose() const;
        //! Observe closeAll(): true is sent each time it is called.
        FTK_UI_API std::shared_ptr<ftk::IObservable<bool> > observeCloseAll() const;

        ///@}

        //! \name Current Document
        ///@{

        //! Get the current document, or null for none.
        FTK_UI_API std::shared_ptr<IDocument> getCurrent() const;
        //! Get the index of the current document, or -1 for none. An index set
        //! is clamped to the documents.
        FTK_UI_API int getCurrentIndex() const;
        //! Observe the current document.
        FTK_UI_API std::shared_ptr<ftk::IObservable<std::shared_ptr<IDocument> > > observeCurrent() const;
        //! Observe the index of the current document.
        FTK_UI_API std::shared_ptr<ftk::IObservable<int> > observeCurrentIndex() const;
        FTK_UI_API void setCurrentIndex(int);

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
