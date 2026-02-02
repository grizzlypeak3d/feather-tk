// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/RenderPrivate.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>

namespace ftk
{
    namespace gl
    {
        namespace
        {
            const int pboSizeMin = 1024;
            const size_t statsAverageCount = 10;
            const size_t statsTimer = 600; // 60Hz * 10 seconds
        }

        void Render::_init(
            const std::shared_ptr<LogSystem>& logSystem,
            const std::shared_ptr<FontSystem>& fontSystem,
            const std::shared_ptr<TextureCache>& textureCache)
        {
            IRender::_init(logSystem, fontSystem);
            FTK_P();
            p.textureCache = textureCache;
            if (!p.textureCache)
            {
                p.textureCache = std::make_shared<TextureCache>();
            }
        }

        Render::Render() :
            _p(new Private)
        {}

        Render::~Render()
        {}

        std::shared_ptr<Render> Render::create(
            const std::shared_ptr<LogSystem>& logSystem,
            const std::shared_ptr<FontSystem>& fontSystem,
            const std::shared_ptr<TextureCache>& textureCache)
        {
            auto out = std::shared_ptr<Render>(new Render);
            out->_init(logSystem, fontSystem, textureCache);
            return out;
        }

        std::shared_ptr<Shader> Render::getShader(const std::string& value)
        {
            return _p->shaders[value];
        }

        const std::shared_ptr<TextureCache>& Render::getTextureCache() const
        {
            return _p->textureCache;
        }

        void Render::begin(
            const Size2I& size,
            const RenderOptions& options)
        {
            FTK_P();

            p.startTime = std::chrono::steady_clock::now();
            p.stats = Private::Stats();
            
            p.size = size;
            p.options = options;
            p.textureCache->setMax(options.textureCacheByteCount);

            if (!p.glyphAtlas ||
                (p.glyphAtlas && options.glyphAtlasSize != p.glyphAtlas->getSize()))
            {
                ImageType imageType = ImageType::L_U8;
#if defined(FTK_API_GLES_2)
                //! \bug Some GLES 2 implementations (Pi Zero W) only support RGBA?
                imageType = ImageType::RGBA_U8;
#endif // FTK_API_GLES_2
                p.glyphAtlas = TextureAtlas::create(
                    options.glyphAtlasSize,
                    imageType,
                    ImageFilter::Linear);
            }

            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);

            if (!p.shaders["rect"])
            {
                p.shaders["rect"] = Shader::create(
                    vertexSource(),
                    meshFragmentSource());
            }
            if (!p.shaders["line"])
            {
                p.shaders["line"] = Shader::create(
                    vertexSource(),
                    meshFragmentSource());
            }
            if (!p.shaders["mesh"])
            {
                p.shaders["mesh"] = Shader::create(
                    vertexSource(),
                    meshFragmentSource());
            }
            if (!p.shaders["colorMesh"])
            {
                p.shaders["colorMesh"] = Shader::create(
                    colorMeshVertexSource(),
                    colorMeshFragmentSource());
            }
            if (!p.shaders["texture"])
            {
                p.shaders["texture"] = gl::Shader::create(
                    vertexSource(),
                    textureFragmentSource());
            }
            if (!p.shaders["text"])
            {
                p.shaders["text"] = Shader::create(
                    vertexSource(),
                    textFragmentSource());
            }
            if (!p.shaders["image"])
            {
                p.shaders["image"] = Shader::create(
                    vertexSource(),
                    imageFragmentSource());
            }

            p.vbos["rect"] = VBO::create(2 * 3, VBOType::Pos2_F32);
            p.vaos["rect"] = VAO::create(p.vbos["rect"]->getType(), p.vbos["rect"]->getID());
            p.vbos["line"] = VBO::create(2 * 3, VBOType::Pos2_F32);
            p.vaos["line"] = VAO::create(p.vbos["line"]->getType(), p.vbos["line"]->getID());
            p.vbos["texture"] = gl::VBO::create(2 * 3, gl::VBOType::Pos2_F32_UV_U16);
            p.vaos["texture"] = gl::VAO::create(p.vbos["texture"]->getType(), p.vbos["texture"]->getID());

            setViewport(Box2I(0, 0, size.w, size.h));
            if (options.clear)
            {
                clearViewport(options.clearColor);
            }
            setTransform(ortho(
                0.F,
                static_cast<float>(size.w),
                static_cast<float>(size.h),
                0.F,
                -1.F,
                1.F));
        }
        
        void Render::end()
        {
            FTK_P();
            const auto now = std::chrono::steady_clock::now();
            const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - p.startTime);
            p.stats.renderTime = diff.count();
            p.statsList.push_back(p.stats);
            while (p.statsList.size() > statsAverageCount)
            {
                p.statsList.pop_front();
            }
            p.statsCounter = p.statsCounter + 1;
            if (p.statsCounter > statsTimer)
            {
                p.statsCounter = 0;
                if (p.options.log)
                {
                    _log();
                }
            }
        }

        Size2I Render::getRenderSize() const
        {
            return _p->size;
        }

        void Render::setRenderSize(const Size2I& value)
        {
            _p->size = value;
        }

        RenderOptions Render::getRenderOptions() const
        {
            return _p->options;
        }

        Box2I Render::getViewport() const
        {
            return _p->viewport;
        }

        void Render::setViewport(const Box2I& value)
        {
            FTK_P();
            p.viewport = value;
            glViewport(
                value.x(),
                p.size.h - value.h() - value.y(),
                value.w(),
                value.h());
        }

        void Render::clearViewport(const Color4F& value)
        {
            glClearColor(value.r, value.g, value.b, value.a);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        bool Render::getClipRectEnabled() const
        {
            return _p->clipRectEnabled;
        }

        void Render::setClipRectEnabled(bool value)
        {
            FTK_P();
            p.clipRectEnabled = value;
            if (p.clipRectEnabled)
            {
                glEnable(GL_SCISSOR_TEST);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }
        }

        Box2I Render::getClipRect() const
        {
            return _p->clipRect;
        }

        void Render::setClipRect(const Box2I& value)
        {
            FTK_P();
            p.clipRect = value;
            const Size2I size = value.size();
            if (size.isValid())
            {
                glScissor(
                    value.x(),
                    p.size.h - size.h - value.y(),
                    size.w,
                    size.h);
            }
        }

        M44F Render::getTransform() const
        {
            return _p->transform;
        }

        void Render::setTransform(const M44F& value)
        {
            FTK_P();
            p.transform = value;
            for (auto i : p.shaders)
            {
                i.second->bind();
                i.second->setUniform("transform.mvp", value);
            }
        }

        std::vector<std::shared_ptr<Texture> > Render::_getTextures(
            const ImageInfo& info,
            const ImageFilters& imageFilters,
            size_t offset)
        {
            std::vector<std::shared_ptr<Texture> > out;
            TextureOptions options;
            options.filters = imageFilters;
            options.pbo = info.size.w >= pboSizeMin || info.size.h >= pboSizeMin;
            switch (info.type)
            {
            case ImageType::YUV_420P_U8:
            {
                auto infoTmp = ImageInfo(info.size, ImageType::L_U8);
                out.push_back(Texture::create(infoTmp, options));
                infoTmp = ImageInfo(info.size.w / 2, info.size.h / 2, ImageType::L_U8);
                out.push_back(Texture::create(infoTmp, options));
                out.push_back(Texture::create(infoTmp, options));
                break;
            }
            case ImageType::YUV_422P_U8:
            {
                auto infoTmp = ImageInfo(info.size, ImageType::L_U8);
                out.push_back(Texture::create(infoTmp, options));
                infoTmp = ImageInfo(info.size.w / 2, info.size.h, ImageType::L_U8);
                out.push_back(Texture::create(infoTmp, options));
                out.push_back(Texture::create(infoTmp, options));
                break;
            }
            case ImageType::YUV_444P_U8:
            {
                auto infoTmp = ImageInfo(info.size, ImageType::L_U8);
                out.push_back(Texture::create(infoTmp, options));
                infoTmp = ImageInfo(info.size, ImageType::L_U8);
                out.push_back(Texture::create(infoTmp, options));
                out.push_back(Texture::create(infoTmp, options));
                break;
            }
            case ImageType::YUV_420P_U16:
            {
                auto infoTmp = ImageInfo(info.size, ImageType::L_U16);
                out.push_back(Texture::create(infoTmp, options));
                infoTmp = ImageInfo(info.size.w / 2, info.size.h / 2, ImageType::L_U16);
                out.push_back(Texture::create(infoTmp, options));
                out.push_back(Texture::create(infoTmp, options));
                break;
            }
            case ImageType::YUV_422P_U16:
            {
                auto infoTmp = ImageInfo(info.size, ImageType::L_U16);
                out.push_back(Texture::create(infoTmp, options));
                infoTmp = ImageInfo(info.size.w / 2, info.size.h, ImageType::L_U16);
                out.push_back(Texture::create(infoTmp, options));
                out.push_back(Texture::create(infoTmp, options));
                break;
            }
            case ImageType::YUV_444P_U16:
            {
                auto infoTmp = ImageInfo(info.size, ImageType::L_U16);
                out.push_back(Texture::create(infoTmp, options));
                infoTmp = ImageInfo(info.size, ImageType::L_U16);
                out.push_back(Texture::create(infoTmp, options));
                out.push_back(Texture::create(infoTmp, options));
                break;
            }
            default:
            {
                auto texture = Texture::create(info, options);
                out.push_back(texture);
                break;
            }
            }
            return out;
        }

        void Render::_copyTextures(
            const std::shared_ptr<Image>& image,
            const std::vector<std::shared_ptr<Texture> >& textures,
            size_t offset)
        {
            const auto& info = image->getInfo();
            switch (info.type)
            {
            case ImageType::YUV_420P_U8:
            {
                if (3 == textures.size())
                {
                    textures[0]->copy(image->getData(), textures[0]->getImageInfo());
                    const std::size_t w = info.size.w;
                    const std::size_t h = info.size.h;
                    const std::size_t w2 = w / 2;
                    const std::size_t h2 = h / 2;
                    textures[1]->copy(image->getData() + (w * h), textures[1]->getImageInfo());
                    textures[2]->copy(image->getData() + (w * h) + (w2 * h2), textures[2]->getImageInfo());
                }
                break;
            }
            case ImageType::YUV_422P_U8:
            {
                if (3 == textures.size())
                {
                    textures[0]->copy(image->getData(), textures[0]->getImageInfo());
                    const std::size_t w = info.size.w;
                    const std::size_t h = info.size.h;
                    const std::size_t w2 = w / 2;
                    textures[1]->copy(image->getData() + (w * h), textures[1]->getImageInfo());
                    textures[2]->copy(image->getData() + (w * h) + (w2 * h), textures[2]->getImageInfo());
                }
                break;
            }
            case ImageType::YUV_444P_U8:
            {
                if (3 == textures.size())
                {
                    textures[0]->copy(image->getData(), textures[0]->getImageInfo());
                    const std::size_t w = info.size.w;
                    const std::size_t h = info.size.h;
                    textures[1]->copy(image->getData() + (w * h), textures[1]->getImageInfo());
                    textures[2]->copy(image->getData() + (w * h) + (w * h), textures[2]->getImageInfo());
                }
                break;
            }
            case ImageType::YUV_420P_U16:
            {
                if (3 == textures.size())
                {
                    textures[0]->copy(image->getData(), textures[0]->getImageInfo());
                    const std::size_t w = info.size.w;
                    const std::size_t h = info.size.h;
                    const std::size_t w2 = w / 2;
                    const std::size_t h2 = h / 2;
                    textures[1]->copy(image->getData() + (w * h) * 2, textures[1]->getImageInfo());
                    textures[2]->copy(image->getData() + (w * h) * 2 + (w2 * h2) * 2, textures[2]->getImageInfo());
                }
                break;
            }
            case ImageType::YUV_422P_U16:
            {
                if (3 == textures.size())
                {
                    textures[0]->copy(image->getData(), textures[0]->getImageInfo());
                    const std::size_t w = info.size.w;
                    const std::size_t h = info.size.h;
                    const std::size_t w2 = w / 2;
                    textures[1]->copy(image->getData() + (w * h) * 2, textures[1]->getImageInfo());
                    textures[2]->copy(image->getData() + (w * h) * 2 + (w2 * h) * 2, textures[2]->getImageInfo());
                }
                break;
            }
            case ImageType::YUV_444P_U16:
            {
                if (3 == textures.size())
                {
                    textures[0]->copy(image->getData(), textures[0]->getImageInfo());
                    const std::size_t w = info.size.w;
                    const std::size_t h = info.size.h;
                    textures[1]->copy(image->getData() + (w * h) * 2, textures[1]->getImageInfo());
                    textures[2]->copy(image->getData() + (w * h) * 2 + (w * h) * 2, textures[2]->getImageInfo());
                }
                break;
            }
            default:
                if (1 == textures.size())
                {
                    textures[0]->copy(image);
                }
                break;
            }
        }

        void Render::_setActiveTextures(
            const ImageInfo& info,
            const std::vector<std::shared_ptr<Texture> >& textures,
            size_t offset)
        {
            switch (info.type)
            {
            case ImageType::YUV_420P_U8:
                if (3 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 1 + offset));
                    textures[1]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 2 + offset));
                    textures[2]->bind();
                }
                break;
            case ImageType::YUV_422P_U8:
                if (3 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 1 + offset));
                    textures[1]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 2 + offset));
                    textures[2]->bind();
                }
                break;
            case ImageType::YUV_444P_U8:
                if (3 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 1 + offset));
                    textures[1]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 2 + offset));
                    textures[2]->bind();
                }
                break;
            case ImageType::YUV_420P_U16:
                if (3 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 1 + offset));
                    textures[1]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 2 + offset));
                    textures[2]->bind();
                }
                break;
            case ImageType::YUV_422P_U16:
                if (3 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 1 + offset));
                    textures[1]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 2 + offset));
                    textures[2]->bind();
                }
                break;
            case ImageType::YUV_444P_U16:
                if (3 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 1 + offset));
                    textures[1]->bind();
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + 2 + offset));
                    textures[2]->bind();
                }
                break;
            default:
                if (1 == textures.size())
                {
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
                    textures[0]->bind();
                }
                break;
            }
        }
        
        void Render::_log()
        {
            FTK_P();
            if (auto logSystem = _logSystem.lock())
            {
                Private::Stats average;
                const size_t size = p.statsList.size();
                if (size)
                {
                    for (auto i : p.statsList)
                    {
                        average.renderTime   += i.renderTime;
                        average.triCount     += i.triCount;
                        average.textureCount += i.textureCount;
                        average.glyphCount   += i.glyphCount;
                    }
                    average.renderTime   /= size;
                    average.triCount     /= size;
                    average.textureCount /= size;
                    average.glyphCount   /= size;
                }
                logSystem->print(
                    "ftk::gl::Render",
                    Format(
                        "\n"
                        "    Averages:\n"
                        "    * Render time:    {0}ms\n"
                        "    * Triangle count: {1}\n"
                        "    * Texture count:  {2}\n"
                        "    * Glyph count:    {3}").
                        arg(average.renderTime).
                        arg(average.triCount).
                        arg(average.textureCount).
                        arg(average.glyphCount));
            }
        }

        std::shared_ptr<IRender> RenderFactory::createRender(
            const std::shared_ptr<LogSystem>& logSystem,
            const std::shared_ptr<FontSystem>& fontSystem)
        {
            return Render::create(logSystem, fontSystem);
        }
    }
}

