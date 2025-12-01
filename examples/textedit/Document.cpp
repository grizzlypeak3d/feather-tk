// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Document.h"

#include <ftk/Core/FileIO.h>

using namespace ftk;

namespace textedit
{
    void Document::_init(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path)
    {
        std::vector<std::string> lines;
        if (!path.empty())
        {
            // Read the file.
            lines = readLines(path);
        }
        _model = TextEditModel::create(context, lines);

        _path = Observable<std::filesystem::path>::create(path);
        _name = Observable<std::string>::create();
        _tooltip = Observable<std::string>::create();
        _changed = Observable<bool>::create(false);

        _nameUpdate();

        // Observe changes to the text and update the name.
        _textObserver = ListObserver<std::string>::create(
            _model->observeText(),
            [this](const std::vector<std::string>&)
            {
                if (_changed->setIfChanged(true))
                {
                    _nameUpdate();
                }
            },
            ObserverAction::Suppress);
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
        return _path->get();
    }

    std::shared_ptr<ftk::IObservable<std::filesystem::path> > Document::observePath() const
    {
        return _path;
    }

    std::shared_ptr<ftk::IObservable<std::string> > Document::observeName() const
    {
        return _name;
    }

    std::shared_ptr<ftk::IObservable<std::string> > Document::observeTooltip() const
    {
        return _tooltip;
    }

    const std::shared_ptr<ftk::TextEditModel>& Document::getModel() const
    {
        return _model;
    }

    bool Document::isChanged() const
    {
        return _changed->get();
    }

    std::shared_ptr<ftk::IObservable<bool> > Document::observeChanged() const
    {
        return _changed;
    }

    void Document::resetChanged()
    {
        _changed->setIfChanged(false);
    }

    void Document::save()
    {
        writeLines(_path->get(), _model->getText());
        _changed->setIfChanged(false);
        _nameUpdate();
    }

    void Document::saveAs(const std::filesystem::path& path)
    {
        writeLines(path, _model->getText());
        _path->setIfChanged(path);
        _changed->setIfChanged(false);
        _nameUpdate();
    }

    void Document::_nameUpdate()
    {
        const std::filesystem::path& path = _path->get();
        std::string name = !path.empty() ? path.filename().u8string() : "Untitled";
        if (_changed->get())
        {
            name += "*";
        }
        _name->setIfChanged(name);
        _tooltip->setIfChanged(!path.empty() ? path.u8string() : "Untitled");
    }
}
