// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Document.h"

#include "ObjIO.h"

using namespace ftk;

namespace objview
{
    void Document::_init(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path)
    {
        _path = path;
        _mesh = read(path);
        _rotation = Observable<ftk::V3F>::create();
    }

    Document::~Document()
    {}

    std::shared_ptr<Document> Document::create(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path)
    {
        auto out = std::shared_ptr<Document>(new Document);
        out->_init(context, path);
        return out;
    }

    const std::filesystem::path& Document::getPath() const
    {
        return _path;
    }

    std::string Document::getName() const
    {
        return _path.filename().u8string();
    }

    std::string Document::getTooltip() const
    {
        return _path.u8string();
    }

    const std::shared_ptr<ftk::TriMesh3F>& Document::getMesh() const
    {
        return _mesh;
    }

    const ftk::V3F& Document::getRotation() const
    {
        return _rotation->get();
    }

    std::shared_ptr<ftk::IObservable<V3F> > Document::observeRotation() const
    {
        return _rotation;
    }

    void Document::setRotation(const V3F& value)
    {
        _rotation->setIfChanged(value);
    }
}
