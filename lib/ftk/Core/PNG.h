// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ImageIO.h>

namespace ftk
{
    namespace png
    {
        //! \name PNG
        ///@{

        //! PNG image reader.
        class ImageReader : public IImageReader
        {
        public:
            ImageReader(
                const std::filesystem::path&,
                const MemFile*,
                const ImageIOOptions&);
                
            virtual ~ImageReader();

            const ImageInfo& getInfo() const override;
            std::shared_ptr<Image> read() override;

        private:
            FTK_PRIVATE();
        };

        //! PNG image writer.
        class ImageWriter : public IImageWriter
        {
        public:
            ImageWriter(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions&);

            virtual ~ImageWriter();

            void write(const std::shared_ptr<Image>&) override;

        private:
            FTK_PRIVATE();
        };
            
        //! PNG image I/O plugin.
        class ImagePlugin : public IImagePlugin
        {
        public:
            ImagePlugin();

            virtual ~ImagePlugin();

            std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const ImageIOOptions& = ImageIOOptions()) override;
            std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const MemFile&,
                const ImageIOOptions& = ImageIOOptions()) override;
            bool canWrite(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions()) override;
            std::shared_ptr<IImageWriter> write(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions()) override;
        };
            
        ///@}
    }
}
