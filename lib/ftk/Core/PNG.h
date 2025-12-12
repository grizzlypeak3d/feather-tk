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
        class FTK_API_TYPE ImageReader : public IImageReader
        {
        public:
            FTK_API ImageReader(
                const std::filesystem::path&,
                const MemFile*,
                const ImageIOOptions&);
                
            FTK_API virtual ~ImageReader();

            FTK_API const ImageInfo& getInfo() const override;
            FTK_API std::shared_ptr<Image> read() override;

        private:
            FTK_PRIVATE();
        };

        //! PNG image writer.
        class FTK_API_TYPE ImageWriter : public IImageWriter
        {
        public:
            FTK_API ImageWriter(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions&);

            FTK_API virtual ~ImageWriter();

            FTK_API void write(const std::shared_ptr<Image>&) override;

        private:
            FTK_PRIVATE();
        };
            
        //! PNG image I/O plugin.
        class FTK_API_TYPE ImagePlugin : public IImagePlugin
        {
        public:
            FTK_API ImagePlugin();

            FTK_API virtual ~ImagePlugin();

            FTK_API std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const ImageIOOptions& = ImageIOOptions()) override;
            FTK_API std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const MemFile&,
                const ImageIOOptions& = ImageIOOptions()) override;
            FTK_API bool canWrite(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions()) override;
            FTK_API std::shared_ptr<IImageWriter> write(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions()) override;
        };
            
        ///@}
    }
}
