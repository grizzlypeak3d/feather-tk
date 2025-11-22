// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/PNGPrivate.h>

#include <filesystem>

namespace ftk
{
    namespace png
    {
        ImagePlugin::ImagePlugin() :
            IImagePlugin("PNG", { ".png" })
        {}

        ImagePlugin::~ImagePlugin()
        {}

        std::shared_ptr<IImageReader> ImagePlugin::read(
            const std::filesystem::path& path,
            const ImageIOOptions& options)
        {
            return std::shared_ptr<ImageReader>(
                new ImageReader(path, nullptr, options));
        }

        std::shared_ptr<IImageReader> ImagePlugin::read(
            const std::filesystem::path& path,
            const MemFile& memFile,
            const ImageIOOptions& options)
        {
            return std::shared_ptr<ImageReader>(
                new ImageReader(path, &memFile, options));
        }

        bool ImagePlugin::canWrite(
            const std::filesystem::path& path,
            const ImageInfo& info,
            const ImageIOOptions& options)
        {
            return IImagePlugin::canWrite(path, info, options) &&
                (
                    info.type == ImageType::L_U8 ||
                    info.type == ImageType::L_U16 ||
                    info.type == ImageType::LA_U8 ||
                    info.type == ImageType::LA_U16 ||
                    info.type == ImageType::RGB_U8 ||
                    info.type == ImageType::RGB_U16 ||
                    info.type == ImageType::RGBA_U8 ||
                    info.type == ImageType::RGBA_U16
                );
        }

        std::shared_ptr<IImageWriter> ImagePlugin::write(
            const std::filesystem::path& path,
            const ImageInfo& info,
            const ImageIOOptions& options)
        {
            return std::shared_ptr<ImageWriter>(
                new ImageWriter(path, info, options));
        }
    }
}

extern "C"
{
    void pngErrorFunc(png_structp in, png_const_charp msg)
    {
        auto error = reinterpret_cast<ftk::png::ErrorStruct*>(png_get_error_ptr(in));
        error->message = msg;
        longjmp(png_jmpbuf(in), 1);
    }

    void pngWarningFunc(png_structp in, png_const_charp msg)
    {
        auto error = reinterpret_cast<ftk::png::ErrorStruct*>(png_get_error_ptr(in));
        error->message = msg;
    }
}

