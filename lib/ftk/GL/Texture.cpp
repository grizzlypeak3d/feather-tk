// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Texture.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Util.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

#include <array>
#include <atomic>
#include <cstring>
#include <stdexcept>

namespace ftk
{
    namespace gl
    {
        FTK_ENUM_IMPL(
            TextureType,
            "None",

            "L U8",
            "L U16",
            "L U32",
            "L F16",
            "L F32",

            "LA U8",
            "LA U16",
            "LA U32",
            "LA F16",
            "LA F32",

            "RGB U8",
            "RGB U10",
            "RGB U16",
            "RGB U32",
            "RGB F16",
            "RGB F32",

            "RGBA U8",
            "RGBA U16",
            "RGBA U32",
            "RGBA F16",
            "RGBA F32");

        TextureType getTextureType(ImageType value)
        {
            TextureType out = TextureType::None;
            switch (value)
            {
            case ImageType::L_U8:     out = TextureType::L_U8;     break;
            case ImageType::L_U16:    out = TextureType::L_U16;    break;
            case ImageType::L_U32:    out = TextureType::L_U32;    break;
            case ImageType::L_F16:    out = TextureType::L_F16;    break;
            case ImageType::L_F32:    out = TextureType::L_F32;    break;

            case ImageType::LA_U8:    out = TextureType::LA_U8;    break;
            case ImageType::LA_U16:   out = TextureType::LA_U16;   break;
            case ImageType::LA_U32:   out = TextureType::LA_U32;   break;
            case ImageType::LA_F16:   out = TextureType::LA_F16;   break;
            case ImageType::LA_F32:   out = TextureType::LA_F32;   break;

            case ImageType::RGB_U8:   out = TextureType::RGB_U8;   break;
            case ImageType::RGB_U10:  out = TextureType::RGB_U10;  break;
            case ImageType::RGB_U16:  out = TextureType::RGB_U16;  break;
            case ImageType::RGB_U32:  out = TextureType::RGB_U32;  break;
            case ImageType::RGB_F16:  out = TextureType::RGB_F16;  break;
            case ImageType::RGB_F32:  out = TextureType::RGB_F32;  break;

            case ImageType::RGBA_U8:  out = TextureType::RGBA_U8;  break;
            case ImageType::RGBA_U16: out = TextureType::RGBA_U16; break;
            case ImageType::RGBA_U32: out = TextureType::RGBA_U32; break;
            case ImageType::RGBA_F16: out = TextureType::RGBA_F16; break;
            case ImageType::RGBA_F32: out = TextureType::RGBA_F32; break;

            default: break;
            }
            return out;
        }

        ImageType getImageType(TextureType value)
        {
            ImageType out = ImageType::None;
            switch (value)
            {
            case TextureType::L_U8:     out = ImageType::L_U8;     break;
            case TextureType::L_U16:    out = ImageType::L_U16;    break;
            case TextureType::L_U32:    out = ImageType::L_U32;    break;
            case TextureType::L_F16:    out = ImageType::L_F16;    break;
            case TextureType::L_F32:    out = ImageType::L_F32;    break;

            case TextureType::LA_U8:    out = ImageType::LA_U8;    break;
            case TextureType::LA_U16:   out = ImageType::LA_U16;   break;
            case TextureType::LA_U32:   out = ImageType::LA_U32;   break;
            case TextureType::LA_F16:   out = ImageType::LA_F16;   break;
            case TextureType::LA_F32:   out = ImageType::LA_F32;   break;

            case TextureType::RGB_U8:   out = ImageType::RGB_U8;   break;
            case TextureType::RGB_U10:  out = ImageType::RGB_U10;  break;
            case TextureType::RGB_U16:  out = ImageType::RGB_U16;  break;
            case TextureType::RGB_U32:  out = ImageType::RGB_U32;  break;
            case TextureType::RGB_F16:  out = ImageType::RGB_F16;  break;
            case TextureType::RGB_F32:  out = ImageType::RGB_F32;  break;

            case TextureType::RGBA_U8:  out = ImageType::RGBA_U8;  break;
            case TextureType::RGBA_U16: out = ImageType::RGBA_U16; break;
            case TextureType::RGBA_U32: out = ImageType::RGBA_U32; break;
            case TextureType::RGBA_F16: out = ImageType::RGBA_F16; break;
            case TextureType::RGBA_F32: out = ImageType::RGBA_F32; break;

            default: break;
            }
            return out;
        }

        unsigned int getTextureFormat(TextureType value)
        {
            const std::array<GLenum, static_cast<size_t>(TextureType::Count)> data =
            {
                GL_NONE,

#if defined(FTK_API_GL_4_1)
                GL_RED,
                GL_RED,
                GL_RED,
                GL_RED,
                GL_RED,

                GL_RG,
                GL_RG,
                GL_RG,
                GL_RG,
                GL_RG,

                GL_RGB,
                GL_RGBA,
                GL_RGB,
                GL_RGB,
                GL_RGB,
                GL_RGB,

                GL_RGBA,
                GL_RGBA,
                GL_RGBA,
                GL_RGBA,
                GL_RGBA
#elif defined(FTK_API_GLES_2)
                GL_LUMINANCE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_LUMINANCE_ALPHA,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGB,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGBA,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE
#endif // FTK_API_GL_4_1
            };
            return data[static_cast<size_t>(value)];
        }

        unsigned int getTextureInternalFormat(TextureType type)
        {
            const std::array<GLenum, static_cast<size_t>(TextureType::Count)> data =
            {
                GL_NONE,

#if defined(FTK_API_GL_4_1)
                GL_R8,
                GL_R16,
                GL_R32I,
                GL_R16F,
                GL_R32F,

                GL_RG8,
                GL_RG16,
                GL_RG32I,
                GL_RG16F,
                GL_RG32F,

                GL_RGB8,
                GL_RGB10,
                GL_RGB16,
                GL_RGB32I,
                GL_RGB16F,
                GL_RGB32F,

                GL_RGBA8,
                GL_RGBA16,
                GL_RGBA32I,
                GL_RGBA16F,
                GL_RGBA32F
#elif defined(FTK_API_GLES_2)
                GL_LUMINANCE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_LUMINANCE_ALPHA,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGB,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGBA,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE
#endif // FTK_API_GL_4_1
            };
            return data[static_cast<size_t>(type)];
        }

        unsigned int getTextureType(TextureType value)
        {
            const std::array<GLenum, static_cast<size_t>(TextureType::Count)> data =
            {
                GL_NONE,

#if defined(FTK_API_GL_4_1)
                GL_UNSIGNED_BYTE,
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT,

                GL_UNSIGNED_BYTE,
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT,

                GL_UNSIGNED_BYTE,
                GL_UNSIGNED_INT_10_10_10_2,
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT,

                GL_UNSIGNED_BYTE,
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT
#elif defined(FTK_API_GLES_2)
                GL_UNSIGNED_BYTE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_UNSIGNED_BYTE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_UNSIGNED_BYTE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_UNSIGNED_BYTE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE
#endif // FTK_API_GL_4_1
            };
            return data[static_cast<size_t>(value)];
        }

        TextureInfo::TextureInfo(const Size2I& size, TextureType type) :
            size(size),
            type(type)
        {}

        bool TextureInfo::isValid() const
        {
            return size.isValid() && type != TextureType::None;
        }

        float TextureInfo::getAspect() const
        {
            return size.h > 0 ? (size.w / static_cast<float>(size.h)) : 0;
        }

        size_t TextureInfo::getByteCount() const
        {
            size_t out = 0;
            switch (type)
            {
            case TextureType::L_U8:     out = size.w * size.h;         break;
            case TextureType::L_U16:    out = size.w * size.h * 2;     break;
            case TextureType::L_U32:    out = size.w * size.h * 4;     break;
            case TextureType::L_F16:    out = size.w * size.h * 2;     break;
            case TextureType::L_F32:    out = size.w * size.h * 4;     break;

            case TextureType::LA_U8:    out = size.w * size.h * 2;     break;
            case TextureType::LA_U16:   out = size.w * size.h * 2 * 2; break;
            case TextureType::LA_U32:   out = size.w * size.h * 2 * 4; break;
            case TextureType::LA_F16:   out = size.w * size.h * 2 * 2; break;
            case TextureType::LA_F32:   out = size.w * size.h * 2 * 4; break;

            case TextureType::RGB_U8:   out = size.w * size.h * 3;     break;
            case TextureType::RGB_U10:  out = size.w * size.h * 4;     break;
            case TextureType::RGB_U16:  out = size.w * size.h * 3 * 2; break;
            case TextureType::RGB_U32:  out = size.w * size.h * 3 * 4; break;
            case TextureType::RGB_F16:  out = size.w * size.h * 3 * 2; break;
            case TextureType::RGB_F32:  out = size.w * size.h * 3 * 4; break;

            case TextureType::RGBA_U8:  out = size.w * size.h * 4;     break;
            case TextureType::RGBA_U16: out = size.w * size.h * 4 * 2; break;
            case TextureType::RGBA_U32: out = size.w * size.h * 4 * 4; break;
            case TextureType::RGBA_F16: out = size.w * size.h * 4 * 2; break;
            case TextureType::RGBA_F32: out = size.w * size.h * 4 * 4; break;

            default: break;
            }
            return out;
        }
        
        bool TextureInfo::operator == (const TextureInfo& other) const
        {
            return
                size == other.size &&
                type == other.type;
        }

        bool TextureInfo::operator != (const TextureInfo& other) const
        {
            return !(*this == other);
        }

        std::string getLabel(const TextureInfo& info)
        {
            return ftk::Format("{0}x{1}:{2} {3}").
                arg(info.size.w).
                arg(info.size.h).
                arg(info.getAspect(), 2).
                arg(info.type);
        }

        bool TextureOptions::operator == (const TextureOptions& other) const
        {
            return
                filters == other.filters &&
                pbo == other.pbo;
        }

        bool TextureOptions::operator != (const TextureOptions& other) const
        {
            return !(*this == other);
        }

        unsigned int getTextureFilter(ImageFilter value)
        {
            const std::array<GLenum, static_cast<size_t>(ImageFilter::Count)> data =
            {
                GL_NEAREST,
                GL_LINEAR
            };
            return data[static_cast<size_t>(value)];
        }

        namespace
        {
            std::atomic<size_t> objectCount = 0;
            std::atomic<size_t> totalByteCount = 0;
        }

        struct Texture::Private
        {
            TextureInfo info;
            ImageInfo imageInfo;
            GLuint pbo = 0;
            GLuint id = 0;
        };

        Texture::Texture(
            const ImageInfo& imageInfo,
            const TextureOptions& options) :
            _p(new Private)
        {
            FTK_P();

            p.info.size = imageInfo.size;
            p.info.type = getTextureType(imageInfo.type);
            p.imageInfo = imageInfo;

            ++objectCount;
            totalByteCount += p.info.getByteCount();

            if (!p.info.isValid())
            {
                throw std::runtime_error("Invalid texture");
            }

#if defined(FTK_API_GL_4_1)
            if (options.pbo)
            {
                glGenBuffers(1, &p.pbo);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, p.pbo);
                glBufferData(
                    GL_PIXEL_UNPACK_BUFFER,
                    p.info.getByteCount(),
                    NULL,
                    GL_STREAM_DRAW);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
#endif // FTK_API_GL_4_1

            glGenTextures(1, &p.id);
            glBindTexture(GL_TEXTURE_2D, p.id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getTextureFilter(options.filters.minify));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getTextureFilter(options.filters.magnify));
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                getTextureInternalFormat(p.info.type),
                p.info.size.w,
                p.info.size.h,
                0,
                getTextureFormat(p.info.type),
                getTextureType(p.info.type),
                NULL);
        }

        Texture::~Texture()
        {
            FTK_P();
            if (p.pbo)
            {
                glDeleteBuffers(1, &p.pbo);
                p.pbo = 0;
            }
            if (p.id)
            {
                glDeleteTextures(1, &p.id);
                p.id = 0;
            }

            --objectCount;
            totalByteCount -= p.info.getByteCount();
        }

        std::shared_ptr<Texture> Texture::create(
            const ImageInfo& info,
            const TextureOptions& options)
        {
            return std::shared_ptr<Texture>(new Texture(info, options));
        }

        const TextureInfo& Texture::getInfo() const
        {
            return _p->info;
        }

        const ImageInfo& Texture::getImageInfo() const
        {
            return _p->imageInfo;
        }

        const Size2I& Texture::getSize() const
        {
            return _p->info.size;
        }

        int Texture::getWidth() const
        {
            return _p->info.size.w;
        }

        int Texture::getHeight() const
        {
            return _p->info.size.h;
        }

        TextureType Texture::getType() const
        {
            return _p->info.type;
        }

        unsigned int Texture::getID() const
        {
            return _p->id;
        }

        bool Texture::copy(const std::shared_ptr<Image>& data)
        {
            FTK_P();
            const auto& info = data->getInfo();
            if (!_isCompatible(info))
                return false;
#if defined(FTK_API_GL_4_1)
            if (p.pbo)
            {
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, p.pbo);
                if (void* buffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY))
                {
                    memcpy(
                        buffer,
                        data->getData(),
                        data->getByteCount());
                    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
                    glBindTexture(GL_TEXTURE_2D, p.id);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, p.imageInfo.layout.alignment);
                    glPixelStorei(GL_UNPACK_SWAP_BYTES, p.imageInfo.layout.endian != getEndian());
                    glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        info.size.w,
                        info.size.h,
                        getTextureFormat(p.info.type),
                        getTextureType(p.info.type),
                        NULL);
                }
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
            else
#endif // FTK_API_GL_4_1
            {
                glBindTexture(GL_TEXTURE_2D, p.id);
                glPixelStorei(GL_UNPACK_ALIGNMENT, info.layout.alignment);
#if defined(FTK_API_GL_4_1)
                glPixelStorei(GL_UNPACK_SWAP_BYTES, info.layout.endian != getEndian());
#endif // FTK_API_GL_4_1
                glTexSubImage2D(
                    GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    info.size.w,
                    info.size.h,
                    getTextureFormat(getTextureType(info.type)),
                    getTextureType(getTextureType(info.type)),
                    data->getData());
            }
            return true;
        }

        bool Texture::copy(const std::shared_ptr<Image>& data, int x, int y)
        {
            FTK_P();
            const auto& info = data->getInfo();
            if (!_isCompatible(info))
                return false;
#if defined(FTK_API_GL_4_1)
            if (p.pbo)
            {
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, p.pbo);
                if (void* buffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY))
                {
                    memcpy(
                        buffer,
                        data->getData(),
                        data->getByteCount());
                    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
                    glBindTexture(GL_TEXTURE_2D, p.id);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, p.imageInfo.layout.alignment);
                    glPixelStorei(GL_UNPACK_SWAP_BYTES, p.imageInfo.layout.endian != getEndian());
                    glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        x,
                        y,
                        info.size.w,
                        info.size.h,
                        getTextureFormat(p.info.type),
                        getTextureType(p.info.type),
                        NULL);
                }
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
            else
#endif // FTK_API_GL_4_1
            {
                glBindTexture(GL_TEXTURE_2D, p.id);
                glPixelStorei(GL_UNPACK_ALIGNMENT, info.layout.alignment);
#if defined(FTK_API_GL_4_1)
                glPixelStorei(GL_UNPACK_SWAP_BYTES, info.layout.endian != getEndian());
#endif // FTK_API_GL_4_1
                glTexSubImage2D(
                    GL_TEXTURE_2D,
                    0,
                    x,
                    y,
                    info.size.w,
                    info.size.h,
                    getTextureFormat(getTextureType(info.type)),
                    getTextureType(getTextureType(info.type)),
                    data->getData());
            }
            return false;
        }

        bool Texture::copy(const uint8_t* data, const ImageInfo& info)
        {
            FTK_P();
            if (!_isCompatible(info))
                return false;
#if defined(FTK_API_GL_4_1)
            if (p.pbo)
            {
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, p.pbo);
                if (void* buffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY))
                {
                    memcpy(
                        buffer,
                        data,
                        info.getByteCount());
                    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
                    glBindTexture(GL_TEXTURE_2D, p.id);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, p.imageInfo.layout.alignment);
                    glPixelStorei(GL_UNPACK_SWAP_BYTES, p.imageInfo.layout.endian != getEndian());
                    glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        info.size.w,
                        info.size.h,
                        getTextureFormat(p.info.type),
                        getTextureType(p.info.type),
                        NULL);
                }
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
            else
#endif // FTK_API_GL_4_1
            {
                glBindTexture(GL_TEXTURE_2D, p.id);
                glPixelStorei(GL_UNPACK_ALIGNMENT, info.layout.alignment);
#if defined(FTK_API_GL_4_1)
                glPixelStorei(GL_UNPACK_SWAP_BYTES, info.layout.endian != getEndian());
#endif // FTK_API_GL_4_1
                glTexSubImage2D(
                    GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    info.size.w,
                    info.size.h,
                    getTextureFormat(getTextureType(info.type)),
                    getTextureType(getTextureType(info.type)),
                    data);
            }
            return true;
        }

        void Texture::bind()
        {
            glBindTexture(GL_TEXTURE_2D, _p->id);
        }

        size_t Texture::getObjectCount()
        {
            return objectCount;
        }

        size_t Texture::getTotalByteCount()
        {
            return totalByteCount;
        }

        bool Texture::_isCompatible(const ImageInfo& other) const
        {
            FTK_P();
            return
                other.size.w <= p.info.size.w &&
                other.size.h <= p.info.size.h &&
                (p.pbo ? other.type == p.imageInfo.type : true) &&
                other.layout == p.imageInfo.layout;
        }
    }
}
