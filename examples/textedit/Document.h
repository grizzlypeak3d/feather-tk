// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/TextEditModel.h>

#include <filesystem>

namespace textedit
{
    //! Document.
    class Document : public ftk::IDocument
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::filesystem::path& = std::filesystem::path());

        Document() = default;

    public:
        virtual ~Document();

        //! Create a new document.
        static std::shared_ptr<Document> create(
            const std::shared_ptr<ftk::Context>&,
            const std::filesystem::path& = std::filesystem::path());

        //! Get the text model.
        const std::shared_ptr<ftk::TextEditModel>& getModel() const;

        //! \name Information
        ///@{

        const std::filesystem::path& getPath() const;
        std::shared_ptr<ftk::IObservable<std::filesystem::path> > observePath() const;
        std::shared_ptr<ftk::IObservable<std::string> > observeName() const;
        std::shared_ptr<ftk::IObservable<std::string> > observeTooltip() const;

        ///@}

        //! \name Document Changes
        ///@{

        bool isChanged() const;
        std::shared_ptr<ftk::IObservable<bool> > observeChanged() const;
        void resetChanged();

        ///@}

        //! \name Save
        ///@{

        void save();
        void saveAs(const std::filesystem::path&);

        ///@}

    private:
        void _nameUpdate();

        std::shared_ptr<ftk::TextEditModel> _model;
        std::shared_ptr<ftk::Observable<std::filesystem::path> > _path;
        std::shared_ptr<ftk::Observable<std::string> > _name;
        std::shared_ptr<ftk::Observable<std::string> > _tooltip;
        std::shared_ptr<ftk::Observable<bool> > _changed;
        std::shared_ptr<ftk::ListObserver<std::string> > _textObserver;
    };
}
