// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/OffscreenBuffer.h>

#include <ftk/GL/Util.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Texture.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <array>
#include <atomic>
#include <sstream>

namespace ftk
{
    namespace gl
    {
        FTK_ENUM_IMPL(
            OffscreenDepth,
            "None",
            "16",
            "24",
            "32");

        FTK_ENUM_IMPL(
            OffscreenStencil,
            "None",
            "8");

        FTK_ENUM_IMPL(
            OffscreenSampling,
            "None",
            "2",
            "4",
            "8",
            "16");

        namespace
        {
            enum class Error
            {
                ColorTexture,
                RenderBuffer,
                Invalid,
                Create,
                Init
            };

            std::string getErrorLabel(Error error)
            {
                std::string out;
                switch (error)
                {
                case Error::ColorTexture:
                    out = "Cannot create color texture";
                    break;
                case Error::RenderBuffer:
                    out = "Cannot create render buffer";
                    break;
                case Error::Invalid:
                    out = "Invalid buffer configuration";
                    break;
                case Error::Create:
                    out = "Cannot create frame buffer";
                    break;
                case Error::Init:
                    out = "Cannot initialize frame buffer";
                    break;
                }
                return out;
            }

            GLenum getBufferInternalFormat(OffscreenDepth depth, OffscreenStencil stencil)
            {
                GLenum out = GL_NONE;
                switch (depth)
                {
                case OffscreenDepth::None:
                    switch (stencil)
                    {
                    case OffscreenStencil::_8:
                        out = GL_STENCIL_INDEX8;
                        break;
                    default: break;
                    }
                    break;
                case OffscreenDepth::_16:
                    out = GL_DEPTH_COMPONENT16;
                    break;
                case OffscreenDepth::_24:
                    switch (stencil)
                    {
                    case OffscreenStencil::None:
                        out = GL_DEPTH_COMPONENT24;
                        break;
                    case OffscreenStencil::_8:
                        out = GL_DEPTH24_STENCIL8;
                        break;
                    default: break;
                    }
                    break;
                case OffscreenDepth::_32:
                    switch (stencil)
                    {
                    case OffscreenStencil::None:
                        out = GL_DEPTH_COMPONENT32F;
                        break;
                    case OffscreenStencil::_8:
                        out = GL_DEPTH32F_STENCIL8;
                        break;
                    default: break;
                    }
                    break;
                default: break;
                }
                return out;
            }
        }

        TextureType getRenderableType(TextureType type)
        {
#if defined(FTK_API_GLES_3)
            const bool f32 = hasExtension("GL_EXT_color_buffer_float");
            const bool f16 = f32 || hasExtension("GL_EXT_color_buffer_half_float");
            const auto renderable = [f16, f32](TextureType value)
            {
                bool out = false;
                switch (value)
                {
                case TextureType::L_U8:
                case TextureType::LA_U8:
                case TextureType::RGB_U8:
                case TextureType::RGBA_U8:
                    out = true;
                    break;
                case TextureType::L_F16:
                case TextureType::LA_F16:
                case TextureType::RGBA_F16:
                    out = f16;
                    break;
                case TextureType::RGB_F16:
                    out = hasExtension("GL_EXT_color_buffer_half_float");
                    break;
                case TextureType::L_F32:
                case TextureType::LA_F32:
                case TextureType::RGBA_F32:
                    out = f32;
                    break;
                default: break;
                }
                return out;
            };
            TextureType out = type;
            if (!renderable(out))
            {
                // As much of the precision as there is to keep.
                const bool f32Type =
                    TextureType::L_F32 == type ||
                    TextureType::LA_F32 == type ||
                    TextureType::RGB_F32 == type ||
                    TextureType::RGBA_F32 == type;
                if (f32Type && renderable(TextureType::RGBA_F32))
                {
                    out = TextureType::RGBA_F32;
                }
                else if (renderable(TextureType::RGBA_F16))
                {
                    out = TextureType::RGBA_F16;
                }
                else
                {
                    out = TextureType::RGBA_U8;
                }
            }
            return out;
#else // FTK_API_GLES_3
            return type;
#endif // FTK_API_GLES_3
        }

        struct OffscreenBuffer::Private
        {
            TextureInfo info;
            OffscreenBufferOptions options;
            GLuint id = 0;
            GLuint colorID = 0;
            GLuint depthStencilID = 0;
        };

        namespace
        {
            std::atomic<size_t> objectCount = 0;
            std::atomic<size_t> totalByteCount = 0;
        }

        void OffscreenBuffer::_init(
            const TextureInfo& info,
            const OffscreenBufferOptions& options)
        {
            FTK_P();

            ++objectCount;
            totalByteCount += info.getByteCount();

            p.info = info;
            p.info.type = getRenderableType(info.type);
            p.options = options;

            if (!p.info.isValid())
            {
                throw std::runtime_error("Invalid offscreen buffer");
            }

            GLenum target = GL_TEXTURE_2D;

#if defined(FTK_API_GL_4_1)
            size_t samples = 0;
            switch (p.options.sampling)
            {
            case OffscreenSampling::_2:
                samples = 2;
                target = GL_TEXTURE_2D_MULTISAMPLE;
                break;
            case OffscreenSampling::_4:
                samples = 4;
                target = GL_TEXTURE_2D_MULTISAMPLE;
                break;
            case OffscreenSampling::_8:
                samples = 8;
                target = GL_TEXTURE_2D_MULTISAMPLE;
                break;
            case OffscreenSampling::_16:
                samples = 16;
                target = GL_TEXTURE_2D_MULTISAMPLE;
                break;
            default: break;
            }
#endif // FTK_API_GL_4_1

            // Create the color texture.
            if (p.info.type != TextureType::None)
            {
                glGenTextures(1, &p.colorID);
                if (!p.colorID)
                {
                    throw std::runtime_error(getErrorLabel(Error::ColorTexture));
                }
                glBindTexture(target, p.colorID);
                switch (p.options.sampling)
                {
#if defined(FTK_API_GL_4_1)
                case OffscreenSampling::_2:
                case OffscreenSampling::_4:
                case OffscreenSampling::_8:
                case OffscreenSampling::_16:
                    glTexImage2DMultisample(
                        target,
                        static_cast<GLsizei>(samples),
                        getTextureInternalFormat(p.info.type),
                        p.info.size.w,
                        p.info.size.h,
                        false);
                    break;
#endif // FTK_API_GL_4_1
                default:
                    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, getTextureFilter(p.options.colorFilters.minify));
                    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, getTextureFilter(p.options.colorFilters.magnify));
                    glTexImage2D(
                        target,
                        0,
                        getTextureInternalFormat(p.info.type),
                        p.info.size.w,
                        p.info.size.h,
                        0,
                        getTextureFormat(p.info.type),
                        getTextureType(p.info.type),
                        0);
                    break;
                }
            }

            // Create the depth/stencil buffer.
            if (p.options.depth != OffscreenDepth::None ||
                p.options.stencil != OffscreenStencil::None)
            {
                glGenRenderbuffers(1, &p.depthStencilID);
                if (!p.depthStencilID)
                {
                    throw std::runtime_error(getErrorLabel(Error::RenderBuffer));
                }
                glBindRenderbuffer(GL_RENDERBUFFER, p.depthStencilID);
#if defined(FTK_API_GL_4_1)
                glRenderbufferStorageMultisample(
                    GL_RENDERBUFFER,
                    static_cast<GLsizei>(samples),
                    getBufferInternalFormat(p.options.depth, p.options.stencil),
                    p.info.size.w,
                    p.info.size.h);
#elif defined(FTK_API_GLES_3)
                glRenderbufferStorage(
                    GL_RENDERBUFFER,
                    getBufferInternalFormat(p.options.depth, p.options.stencil),
                    p.info.size.w,
                    p.info.size.h);
#endif // FTK_API_GL_4_1
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
            }

            if (!p.colorID && !p.depthStencilID)
            {
                throw std::runtime_error(getErrorLabel(Error::Invalid));
            }

            // Create the FBO.
            glGenFramebuffers(1, &p.id);
            if (!p.id)
            {
                throw std::runtime_error(getErrorLabel(Error::Create));
            }
            const OffscreenBufferBinding binding(shared_from_this());
            if (p.colorID)
            {
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    target,
                    p.colorID,
                    0);
            }
            if (p.depthStencilID)
            {
                // Stencil alone is a stencil attachment: attached as depth
                // and stencil, the depth half has nothing to attach.
                const bool depth = p.options.depth != OffscreenDepth::None;
                const bool stencil = p.options.stencil != OffscreenStencil::None;
                const GLenum attachment =
                    depth && stencil ? GL_DEPTH_STENCIL_ATTACHMENT :
                    (stencil ? GL_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT);
                glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    attachment,
                    GL_RENDERBUFFER,
                    p.depthStencilID);
            }
            GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (error != GL_FRAMEBUFFER_COMPLETE)
            {
                throw std::runtime_error(getErrorLabel(Error::Init));
            }
        }

        OffscreenBuffer::OffscreenBuffer() :
            _p(new Private)
        {}

        OffscreenBuffer::~OffscreenBuffer()
        {
            FTK_P();
            if (p.id)
            {
                glDeleteFramebuffers(1, &p.id);
                p.id = 0;
            }
            if (p.colorID)
            {
                glDeleteTextures(1, &p.colorID);
                p.colorID = 0;
            }
            if (p.depthStencilID)
            {
                glDeleteRenderbuffers(1, &p.depthStencilID);
                p.depthStencilID = 0;
            }

            --objectCount;
            totalByteCount -= p.info.getByteCount();
        }

        std::shared_ptr<OffscreenBuffer> OffscreenBuffer::create(
            const TextureInfo& info,
            const OffscreenBufferOptions& options)
        {
            auto out = std::shared_ptr<OffscreenBuffer>(new OffscreenBuffer);
            out->_init(info, options);
            return out;
        }

        std::shared_ptr<OffscreenBuffer> OffscreenBuffer::create(
            const Size2I& size,
            TextureType type,
            const OffscreenBufferOptions& options)
        {
            auto out = std::shared_ptr<OffscreenBuffer>(new OffscreenBuffer);
            out->_init(TextureInfo(size, type), options);
            return out;
        }

        const TextureInfo& OffscreenBuffer::getInfo() const
        {
            return _p->info;
        }

        const Size2I& OffscreenBuffer::getSize() const
        {
            return _p->info.size;
        }

        int OffscreenBuffer::getWidth() const
        {
            return _p->info.size.w;
        }

        int OffscreenBuffer::getHeight() const
        {
            return _p->info.size.h;
        }

        TextureType OffscreenBuffer::getType() const
        {
            return _p->info.type;
        }

        const OffscreenBufferOptions& OffscreenBuffer::getOptions() const
        {
            return _p->options;
        }

        GLuint OffscreenBuffer::getID() const
        {
            return _p->id;
        }

        GLuint OffscreenBuffer::getColorID() const
        {
            return _p->colorID;
        }

        void OffscreenBuffer::bind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, _p->id);
        }

        size_t OffscreenBuffer::getObjectCount()
        {
            return objectCount;
        }

        size_t OffscreenBuffer::getTotalByteCount()
        {
            return totalByteCount;
        }

        bool doCreate(
            const std::shared_ptr<OffscreenBuffer>& offscreenBuffer,
            const TextureInfo& info,
            const OffscreenBufferOptions& options)
        {
            // Against what the buffer would be made with, which is not always
            // what was asked for; see getRenderableType().
            TextureInfo renderable = info;
            renderable.type = getRenderableType(info.type);
            bool out = false;
            out |= info.size.isValid() && !offscreenBuffer;
            out |= info.size.isValid() && offscreenBuffer && offscreenBuffer->getInfo() != renderable;
            out |= info.size.isValid() && offscreenBuffer && offscreenBuffer->getOptions() != options;
            return out;
        }

        bool doCreate(
            const std::shared_ptr<OffscreenBuffer>& offscreenBuffer,
            const Size2I& size,
            TextureType type,
            const OffscreenBufferOptions& options)
        {
            return doCreate(offscreenBuffer, TextureInfo(size, type), options);
        }

        struct OffscreenBufferBinding::Private
        {
            std::shared_ptr<OffscreenBuffer> buffer;
            GLint previous = 0;
        };

        OffscreenBufferBinding::OffscreenBufferBinding(const std::shared_ptr<OffscreenBuffer>& buffer) :
            _p(new Private)
        {
            FTK_P();
            p.buffer = buffer;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &p.previous);
            p.buffer->bind();
        }

        OffscreenBufferBinding::~OffscreenBufferBinding()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, _p->previous);
        }
    }
}
