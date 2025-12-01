// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>

#include <ftk/Core/Mesh.h>

#include <filesystem>

namespace objview
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

        //! \name Information
        ///@{

        const std::filesystem::path& getPath() const;
        std::string getName() const;
        std::string getTooltip() const;

        ///@}

        //! \name Mesh
        ///@{

        const std::shared_ptr<ftk::TriMesh3F>& getMesh() const;

        ///@}

        //! \name Rotation
        ///@{

        const ftk::V3F& getRotation() const;
        std::shared_ptr<ftk::IObservable<ftk::V3F> > observeRotation() const;
        void setRotation(const ftk::V3F&);

        ///@}

    private:
        std::filesystem::path _path;
        std::shared_ptr<ftk::TriMesh3F> _mesh;
        std::shared_ptr<ftk::Observable<ftk::V3F> > _rotation;
    };
}
