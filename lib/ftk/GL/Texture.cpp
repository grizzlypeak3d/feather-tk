// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Texture.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Util.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <array>
#include <cstring>
#include <stdexcept>

namespace ftk
{
    namespace gl
    {
        unsigned int getTextureFormat(ImageType value)
        {
            const std::array<GLenum, static_cast<size_t>(ImageType::Count)> data =
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
                GL_RGBA,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_BGRA
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
                GL_NONE,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_NONE
#endif // FTK_API_GL_4_1
            };
            return data[static_cast<size_t>(value)];
        }

        unsigned int getTextureInternalFormat(ImageType type)
        {
            const std::array<GLenum, static_cast<size_t>(ImageType::Count)> data =
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
                GL_RGBA32F,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

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
                GL_NONE,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_NONE
#endif // FTK_API_GL_4_1
            };
            return data[static_cast<size_t>(type)];
        }

        unsigned int getTextureType(ImageType value)
        {
            const std::array<GLenum, static_cast<size_t>(ImageType::Count)> data =
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
                GL_FLOAT,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_UNSIGNED_SHORT_4_4_4_4_REV
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
                GL_NONE,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_NONE
#endif // FTK_API_GL_4_1
            };
            return data[static_cast<size_t>(value)];
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

        struct Texture::Private
        {
            ImageInfo info;
            GLuint pbo = 0;
            GLuint id = 0;
        };

        Texture::Texture(const ImageInfo& info, const TextureOptions& options) :
            _p(new Private)
        {
            FTK_P();

            p.info = info;
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
        }

        std::shared_ptr<Texture> Texture::create(const ImageInfo& info, const TextureOptions& options)
        {
            return std::shared_ptr<Texture>(new Texture(info, options));
        }

        const ImageInfo& Texture::getInfo() const
        {
            return _p->info;
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

        ImageType Texture::getType() const
        {
            return _p->info.type;
        }

        unsigned int Texture::getID() const
        {
            return _p->id;
        }

        void Texture::copy(const std::shared_ptr<Image>& data)
        {
            FTK_P();
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
                    const auto& info = data->getInfo();
                    glBindTexture(GL_TEXTURE_2D, p.id);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, info.layout.alignment);
                    glPixelStorei(GL_UNPACK_SWAP_BYTES, info.layout.endian != getEndian());
                    glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        info.size.w,
                        info.size.h,
                        getTextureFormat(info.type),
                        getTextureType(info.type),
                        NULL);
                }
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
            else
#endif // FTK_API_GL_4_1
            {
                const auto& info = data->getInfo();
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
                    getTextureFormat(info.type),
                    getTextureType(info.type),
                    data->getData());
            }
        }

        void Texture::copy(const std::shared_ptr<Image>& data, int x, int y)
        {
            FTK_P();
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
                    const auto& info = data->getInfo();
                    glBindTexture(GL_TEXTURE_2D, p.id);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, info.layout.alignment);
                    glPixelStorei(GL_UNPACK_SWAP_BYTES, info.layout.endian != getEndian());
                    glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        x,
                        y,
                        info.size.w,
                        info.size.h,
                        getTextureFormat(info.type),
                        getTextureType(info.type),
                        NULL);
                }
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            }
            else
#endif // FTK_API_GL_4_1
            {
                const auto& info = data->getInfo();
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
                    getTextureFormat(info.type),
                    getTextureType(info.type),
                    data->getData());
            }
        }

        void Texture::copy(const uint8_t* data, const ImageInfo& info)
        {
            FTK_P();
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
                    glPixelStorei(GL_UNPACK_ALIGNMENT, info.layout.alignment);
                    glPixelStorei(GL_UNPACK_SWAP_BYTES, info.layout.endian != getEndian());
                    glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        info.size.w,
                        info.size.h,
                        getTextureFormat(info.type),
                        getTextureType(info.type),
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
                    getTextureFormat(info.type),
                    getTextureType(info.type),
                    data);
            }
        }

        void Texture::bind()
        {
            glBindTexture(GL_TEXTURE_2D, _p->id);
        }
    }
}
