// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/PNGPrivate.h>

#include <ftk/Core/Memory.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

#include <cstring>

namespace ftk
{
    namespace png
    {
        namespace
        {
            void memoryRead(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
            {
                png_voidp io_ptr = png_get_io_ptr(png_ptr);
                MemFile* memFile = static_cast<MemFile*>(io_ptr);
                if (byteCountToRead > memFile->size)
                {
                    png_error(png_ptr, "Cannot read");
                }
                memcpy(outBytes, memFile->p, byteCountToRead);
                memFile->p += byteCountToRead;
                memFile->size -= byteCountToRead;
            }

            bool open(
                FILE* f,
                MemFile* memFile,
                png_structp png,
                png_infop* pngInfo,
                png_infop* pngInfoEnd,
                uint16_t& width,
                uint16_t& height,
                uint8_t& channels,
                uint8_t& bitDepth)
            {
                if (setjmp(png_jmpbuf(png)))
                {
                    return false;
                }

                *pngInfo = png_create_info_struct(png);
                if (!*pngInfo)
                {
                    return false;
                }

                *pngInfoEnd = png_create_info_struct(png);
                if (!*pngInfoEnd)
                {
                    return false;
                }

                uint8_t tmp[8];
                memset(tmp, 0, 8);
                if (memFile->p)
                {
                    if (memFile->size < 8)
                    {
                        return false;
                    }
                    memcpy(tmp, memFile->p, 8);
                    memFile->p += 8;
                    memFile->size -= 8;
                }
                else
                {
                    size_t r = fread(tmp, 8, 1, f);
                    if (r != 1)
                    {
                        return false;
                    }
                }
                if (png_sig_cmp(tmp, 0, 8))
                {
                    return false;
                }

                if (memFile->p)
                {
                    png_set_read_fn(png, memFile, memoryRead);
                }
                else
                {
                    png_init_io(png, f);
                }
                png_set_sig_bytes(png, 8);
                png_read_info(png, *pngInfo);

                if (png_get_interlace_type(png, *pngInfo) != PNG_INTERLACE_NONE)
                {
                    return false;
                }

                png_set_expand(png);
                //png_set_gray_1_2_4_to_8(png);
                png_set_palette_to_rgb(png);
                png_set_tRNS_to_alpha(png);

                width = png_get_image_width(png, *pngInfo);
                height = png_get_image_height(png, *pngInfo);

                channels = png_get_channels(png, *pngInfo);
                if (png_get_color_type(png, *pngInfo) == PNG_COLOR_TYPE_PALETTE)
                {
                    channels = 3;
                }
                if (png_get_valid(png, *pngInfo, PNG_INFO_tRNS))
                {
                    ++channels;
                }
                bitDepth = png_get_bit_depth(png, *pngInfo);
                if (bitDepth < 8)
                {
                    bitDepth = 8;
                }

                if (bitDepth >= 16 && Endian::LSB == getEndian())
                {
                    png_set_swap(png);
                }

                return true;
            }

            bool scanline(png_structp png, uint8_t* out)
            {
                if (setjmp(png_jmpbuf(png)))
                {
                    return false;
                }
                png_read_row(png, out, 0);
                return true;
            }

            bool end(png_structp png, png_infop pngInfo)
            {
                if (setjmp(png_jmpbuf(png)))
                {
                    return false;
                }
                png_read_end(png, pngInfo);
                return true;
            }
        }

        struct ImageReader::Private
        {
            png_structp png = nullptr;
            png_infop   pngInfo = nullptr;
            png_infop   pngInfoEnd = nullptr;
            FILE*       f = nullptr;
            MemFile     memFile;
            ErrorStruct error;
            size_t      scanlineSize = 0;
            ImageInfo   info;
        };

        ImageReader::ImageReader(
            const std::filesystem::path& path,
            const MemFile* memFile,
            const ImageIOOptions& options) :
            IImageReader(path, memFile, options),
            _p(new Private)
        {
            FTK_P();
            p.png = png_create_read_struct(
                PNG_LIBPNG_VER_STRING,
                &p.error,
                pngErrorFunc,
                pngWarningFunc);

            if (memFile)
            {
                p.memFile.p = memFile->p;
                p.memFile.size = memFile->size;
            }
            else
            {
#if defined(_WINDOWS)
                if (_wfopen_s(&p.f, path.wstring().c_str(), L"rb") != 0)
                {
                    p.f = nullptr;
                }
#else // _WINDOWS
                p.f = fopen(path.u8string().c_str(), "rb");
#endif // _WINDOWS
                if (!p.f)
                {
                    throw std::runtime_error(Format("Cannot open: \"{0}\"").arg(path.u8string()));
                }
            }

            uint16_t width = 0;
            uint16_t height = 0;
            uint8_t channels = 0;
            uint8_t bitDepth = 0;
            if (!open(
                p.f,
                &p.memFile,
                p.png,
                &p.pngInfo,
                &p.pngInfoEnd,
                width,
                height,
                channels,
                bitDepth))
            {
                throw std::runtime_error(Format("Cannot open: \"{0}\"").arg(path.u8string()));
            }
            p.scanlineSize = width * channels * bitDepth / 8;

            ImageType type = ImageType::None;
            switch (channels)
            {
            case 1:
                switch (bitDepth)
                {
                case 8: type = ImageType::L_U8; break;
                case 16: type = ImageType::L_U16; break;
                default: break;
                }
                break;
            case 2:
                switch (bitDepth)
                {
                case 8: type = ImageType::LA_U8; break;
                case 16: type = ImageType::LA_U16; break;
                default: break;
                }
                break;
            case 3:
                switch (bitDepth)
                {
                case 8: type = ImageType::RGB_U8; break;
                case 16: type = ImageType::RGB_U16; break;
                default: break;
                }
                break;
            case 4:
                switch (bitDepth)
                {
                case 8: type = ImageType::RGBA_U8; break;
                case 16: type = ImageType::RGBA_U16; break;
                default: break;
                }
                break;
            default: break;
            }
            if (ImageType::None == type)
            {
                throw std::runtime_error(Format("Cannot open: \"{0}\"").arg(path.u8string()));
            }

            p.info = ImageInfo(width, height, type);
            p.info.layout.flip.y = true;
        }

        ImageReader::~ImageReader()
        {
            FTK_P();
            if (p.f)
            {
                fclose(p.f);
            }
            if (p.png || p.pngInfo || p.pngInfoEnd)
            {
                png_destroy_read_struct(
                    p.png ? &p.png : nullptr,
                    p.pngInfo ? &p.pngInfo : nullptr,
                    p.pngInfoEnd ? &p.pngInfoEnd : nullptr);
            }
        }

        const ImageInfo& ImageReader::getInfo() const
        {
            return _p->info;
        }

        std::shared_ptr<Image> ImageReader::read()
        {
            FTK_P();
            auto out = Image::create(p.info);
            uint8_t* data = out->getData();
            for (uint16_t y = 0; y < p.info.size.h; ++y, data += p.scanlineSize)
            {
                if (!scanline(p.png, data))
                {
                    break;
                }
            }
            end(p.png, p.pngInfoEnd);
            return out;
        }
    }
}