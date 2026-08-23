// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/ImageIO.h>

namespace ftk
{
    namespace png
    {
        //! \name PNG
        ///@{

        //! PNG image reader.
        class FTK_CORE_API_TYPE ImageReader : public IImageReader
        {
        public:
            FTK_CORE_API ImageReader(
                const std::filesystem::path&,
                const MemFile*,
                const ImageIOOptions&);
                
            FTK_CORE_API virtual ~ImageReader();

            FTK_CORE_API const ImageInfo& getInfo() const override;
            FTK_CORE_API std::shared_ptr<Image> read() override;

        private:
            FTK_PRIVATE();
        };

        //! PNG image writer.
        class FTK_CORE_API_TYPE ImageWriter : public IImageWriter
        {
        public:
            FTK_CORE_API ImageWriter(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions&);

            FTK_CORE_API virtual ~ImageWriter();

            FTK_CORE_API void write(const std::shared_ptr<Image>&) override;

        private:
            FTK_PRIVATE();
        };
            
        //! PNG image I/O plugin.
        class FTK_CORE_API_TYPE ImagePlugin : public IImagePlugin
        {
        public:
            FTK_CORE_API ImagePlugin();

            FTK_CORE_API virtual ~ImagePlugin();

            FTK_CORE_API std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const ImageIOOptions& = ImageIOOptions()) override;
            FTK_CORE_API std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const MemFile&,
                const ImageIOOptions& = ImageIOOptions()) override;
            FTK_CORE_API bool canWrite(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions()) override;
            FTK_CORE_API std::shared_ptr<IImageWriter> write(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions()) override;
        };
            
        ///@}
    }
}
