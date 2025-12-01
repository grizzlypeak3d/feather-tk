// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DocumentModel.h>

#include <ftk/Core/Math.h>

namespace ftk
{
    IDocument::~IDocument()
    {}

    void DocumentModel::_init(const std::shared_ptr<Context>& context)
    {
        _documents = ObservableList<std::shared_ptr<IDocument> >::create();
        _add = Observable<std::weak_ptr<IDocument> >::create();
        _close = Observable<std::weak_ptr<IDocument> >::create();
        _closeAll = Observable<bool>::create(false);
        _current = Observable<std::shared_ptr<IDocument> >::create();
        _currentIndex = Observable<int>::create(-1);
    }

    DocumentModel::~DocumentModel()
    {}

    std::shared_ptr<DocumentModel> DocumentModel::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<DocumentModel>(new DocumentModel);
        out->_init(context);
        return out;
    }

    const std::vector<std::shared_ptr<IDocument> >& DocumentModel::getList() const
    {
        return _documents->get();
    }

    std::shared_ptr<ftk::IObservableList<std::shared_ptr<IDocument> > > DocumentModel::observeList() const
    {
        return _documents;
    }

    void DocumentModel::add(const std::shared_ptr<IDocument>& doc)
    {
        _documents->pushBack(doc);
        _add->setAlways(doc);
        _current->setAlways(doc);
        _currentIndex->setAlways(_documents->getSize() - 1);
    }

    std::shared_ptr<ftk::IObservable<std::weak_ptr<IDocument> > > DocumentModel::observeAdd() const
    {
        return _add;
    }

    void DocumentModel::close(int index)
    {
        if (index >= 0 && index < _documents->getSize())
        {
            _close->setAlways(_documents->getItem(index));
            _documents->removeItem(index);

            // Update the current document if necessary.
            if (index == _currentIndex->get())
            {
                _currentIndex->setIfChanged(std::min(
                    index,
                    static_cast<int>(_documents->getSize()) - 1));
            }
            else if (index < _currentIndex->get())
            {
                _currentIndex->setIfChanged(_currentIndex->get() - 1);
            }
            _current->setIfChanged(
                _currentIndex->get() >= 0 &&
                _currentIndex->get() < _documents->getSize() ?
                _documents->getItem(_currentIndex->get()) :
                nullptr);
        }
    }

    void DocumentModel::closeAll()
    {
        _documents->clear();
        _closeAll->setAlways(true);
        _current->setIfChanged(nullptr);
        _currentIndex->setIfChanged(-1);
    }

    std::shared_ptr<ftk::IObservable<std::weak_ptr<IDocument> > > DocumentModel::observeClose() const
    {
        return _close;
    }

    std::shared_ptr<ftk::IObservable<bool> > DocumentModel::observeCloseAll() const
    {
        return _closeAll;
    }

    std::shared_ptr<IDocument> DocumentModel::getCurrent() const
    {
        return _current->get();
    }

    int DocumentModel::getCurrentIndex() const
    {
        return _currentIndex->get();
    }

    std::shared_ptr<ftk::IObservable<std::shared_ptr<IDocument> > > DocumentModel::observeCurrent() const
    {
        return _current;
    }

    std::shared_ptr<ftk::IObservable<int> > DocumentModel::observeCurrentIndex() const
    {
        return _currentIndex;
    }

    void DocumentModel::setCurrentIndex(int value)
    {
        const auto& docs = _documents->get();
        const int tmp = clamp(value, 0, static_cast<int>(docs.size()) - 1);
        if (_currentIndex->setIfChanged(tmp))
        {
            _current->setIfChanged(
                tmp >= 0 && tmp < docs.size() ?
                docs[tmp] :
                nullptr);
        }
    }
}
