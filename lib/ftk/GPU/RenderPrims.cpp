// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/RenderPrivate.h>

#include <ftk/Core/RenderUtil.h>

#include <cstring>
#include <sstream>

namespace ftk
{
    namespace gpu
    {
        namespace
        {
            ColorUniforms colorUniforms(const Color4F& color)
            {
                ColorUniforms out;
                out.color[0] = color.r;
                out.color[1] = color.g;
                out.color[2] = color.b;
                out.color[3] = color.a;
                return out;
            }

            Blend getBlend(AlphaBlend value)
            {
                Blend out = Blend::Default;
                switch (value)
                {
                case AlphaBlend::Straight: out = Blend::Straight; break;
                case AlphaBlend::Premultiplied: out = Blend::Premultiplied; break;
                default: break;
                }
                return out;
            }

            std::string getTexturePoolKey(const ImageInfo& value)
            {
                std::stringstream ss;
                ss << value.size << " " << value.type;
                return ss.str();
            }
        }

        void Render::drawRect(
            const Box2F& rect,
            const Color4F& color)
        {
            drawRects({ rect }, color);
        }

        void Render::drawRects(
            const std::vector<Box2F>& rects,
            const Color4F& color)
        {
            TriMesh2F mesh;
            mesh.v.resize(rects.size() * 4);
            mesh.triangles.resize(rects.size() * 2);
            size_t v = 0;
            size_t t = 0;
            for (const auto& rect : rects)
            {
                mesh.v[v + 0] = rect.min;
                mesh.v[v + 1].x = rect.max.x;
                mesh.v[v + 1].y = rect.min.y;
                mesh.v[v + 2] = rect.max;
                mesh.v[v + 3].x = rect.min.x;
                mesh.v[v + 3].y = rect.max.y;
                mesh.triangles[t + 0] = { v + 1, v + 3, v + 2 };
                mesh.triangles[t + 1] = { v + 3, v + 1, v + 4 };
                v += 4;
                t += 2;
            }
            drawMesh(mesh, color);
        }

        void Render::drawLine(
            const V2F& v0,
            const V2F& v1,
            const Color4F& color,
            const LineOptions& options)
        {
            drawLines({ std::make_pair(v0, v1) }, color, options);
        }

        void Render::drawLines(
            const std::vector<std::pair<V2F, V2F> >& lines,
            const Color4F& color,
            const LineOptions& options)
        {
            TriMesh2F mesh;
            mesh.v.resize(lines.size() * 4);
            mesh.triangles.resize(lines.size() * 2);
            size_t v = 0;
            size_t t = 0;
            for (const auto& i : lines)
            {
                const V2F v2 = normalize(i.second - i.first);
                const V2F v2CW = perpCW(v2) * options.width / 2.F;
                const V2F v2CCW = perpCCW(v2) * options.width / 2.F;
                mesh.v[v + 0] = i.first + v2CCW;
                mesh.v[v + 1] = i.first + v2CW;
                mesh.v[v + 2] = i.second + v2CW;
                mesh.v[v + 3] = i.second + v2CCW;
                mesh.triangles[t + 0] = { v + 1, v + 3, v + 2 };
                mesh.triangles[t + 1] = { v + 3, v + 1, v + 4 };
                v += 4;
                t += 2;
            }
            drawMesh(mesh, color);
        }

        void Render::drawMesh(
            const TriMesh2F& mesh,
            const Color4F& color,
            const V2F& pos)
        {
            FTK_P();
            if (!mesh.triangles.empty())
            {
                const auto uniforms = colorUniforms(color);
                p.drawUV(
                    "mesh",
                    Blend::Default,
                    mesh,
                    p.transform * translate(V3F(pos.x, pos.y, 0.F)),
                    &uniforms,
                    sizeof(uniforms));
            }
        }

        void Render::drawColorMesh(
            const TriMesh2F& mesh,
            const Color4F& color,
            const V2F& pos)
        {
            FTK_P();
            if (!mesh.triangles.empty())
            {
                const size_t vSize = mesh.v.size();
                const size_t cSize = mesh.c.size();
                p.verticesColor.resize(mesh.triangles.size() * 3);
                VertexColor* out = p.verticesColor.data();
                for (const auto& triangle : mesh.triangles)
                {
                    for (size_t k = 0; k < 3; ++k, ++out)
                    {
                        const size_t v = triangle.v[k].v;
                        const size_t c = triangle.v[k].c;
                        out->x = v && v <= vSize ? mesh.v[v - 1].x : 0.F;
                        out->y = v && v <= vSize ? mesh.v[v - 1].y : 0.F;
                        const bool hasColor = c && c <= cSize;
                        out->r = hasColor ? mesh.c[c - 1].x : 1.F;
                        out->g = hasColor ? mesh.c[c - 1].y : 1.F;
                        out->b = hasColor ? mesh.c[c - 1].z : 1.F;
                        out->a = hasColor ? mesh.c[c - 1].w : 1.F;
                    }
                }
                p.diag.triangles += mesh.triangles.size();
                const auto uniforms = colorUniforms(color);
                p.draw(
                    "colorMesh",
                    Blend::Default,
                    p.verticesColor.data(),
                    p.verticesColor.size(),
                    sizeof(VertexColor),
                    p.transform * translate(V3F(pos.x, pos.y, 0.F)),
                    &uniforms,
                    sizeof(uniforms));
            }
        }

        void Render::drawTexture(
            unsigned int id,
            const Box2I& rect,
            bool mirrorV,
            const Color4F& color,
            AlphaBlend alphaBlend)
        {
            FTK_P();
            if (SDL_GPUTexture* texture = p.system->getTexture(id))
            {
                TextureUniforms uniforms;
                uniforms.color[0] = color.r;
                uniforms.color[1] = color.g;
                uniforms.color[2] = color.b;
                uniforms.color[3] = color.a;
                uniforms.opaque = AlphaBlend::None == alphaBlend;
                SDL_GPUTextureSamplerBinding binding = {};
                binding.texture = texture;
                binding.sampler = p.sampler;
                // What is drawn into a buffer here has its first row at
                // the top, where OpenGL's has it at the bottom. The callers
                // say whether to mirror with OpenGL's in mind, so it is
                // turned around for them.
                p.drawUV(
                    "texture",
                    getBlend(alphaBlend),
                    mesh(rect, !mirrorV),
                    p.transform,
                    &uniforms,
                    sizeof(uniforms),
                    &binding,
                    1);
            }
        }

        namespace
        {
            // Enough for a comparison of a few pictures at one zoom; a zoom
            // walks through sizes, so this is bounded rather than kept.
            const size_t scaleTableMax = 8;
            const size_t scaleBufferMax = 4;

            M44F bufferTransform(const Size2I& size)
            {
                return ortho(
                    0.F,
                    static_cast<float>(size.w),
                    static_cast<float>(size.h),
                    0.F,
                    -1.F,
                    1.F);
            }
        }

        const Render::Private::ScaleTable& Render::Private::scaleTable(int in, int out)
        {
            for (auto i = scaleTables.begin(); i != scaleTables.end(); ++i)
            {
                if (i->in == in && i->out == out)
                {
                    scaleTables.splice(scaleTables.begin(), scaleTables, i);
                    return scaleTables.front();
                }
            }
            ScaleTable table;
            table.in = in;
            table.out = out;
            const auto data = getScaleContrib(in, out, table.taps);
            TextureOptions options;
            options.filters.minify = ImageFilter::Nearest;
            options.filters.magnify = ImageFilter::Nearest;
            table.texture = Texture::create(system, data->getInfo(), options);
            table.texture->copy(data);
            scaleTables.push_front(table);
            while (scaleTables.size() > scaleTableMax)
            {
                scaleTables.pop_back();
            }
            return scaleTables.front();
        }

        std::shared_ptr<OffscreenBuffer> Render::Private::scaleBuffer(const Size2I& size)
        {
            for (auto i = scaleBuffers.begin(); i != scaleBuffers.end(); ++i)
            {
                if (*i && (*i)->getSize() == size)
                {
                    scaleBuffers.splice(scaleBuffers.begin(), scaleBuffers, i);
                    return scaleBuffers.front();
                }
            }
            auto out = OffscreenBuffer::create(system, size, BufferType::RGBA_F16);
            scaleBuffers.push_front(out);
            while (scaleBuffers.size() > scaleBufferMax)
            {
                scaleBuffers.pop_back();
            }
            return out;
        }

        void Render::drawTextureScaled(
            unsigned int id,
            const Size2I& sourceSize,
            const Box2I& rect,
            bool mirrorV)
        {
            FTK_P();
            const Size2I destSize = rect.size();
            SDL_GPUTexture* texture = p.system->getTexture(id);
            if (!texture ||
                !sourceSize.isValid() ||
                !destSize.isValid() ||
                (destSize.w >= sourceSize.w && destSize.h >= sourceSize.h))
            {
                drawTexture(id, rect, mirrorV);
                return;
            }
            // Copies, since making the second table may let go of the first.
            const Private::ScaleTable x = p.scaleTable(sourceSize.w, destSize.w);
            const Private::ScaleTable y = p.scaleTable(sourceSize.h, destSize.h);

            // Across first, into an intermediate that is already narrowed
            // but still full height.
            const Size2I tmpSize(destSize.w, sourceSize.h);
            const auto tmp = p.scaleBuffer(tmpSize);
            {
                pushTarget(tmp);
                TextureScaleUniforms uniforms;
                uniforms.scaleTaps = x.taps;
                uniforms.scaleVertical = 0;
                const SDL_GPUTextureSamplerBinding bindings[2] =
                {
                    { texture, p.samplerNearest },
                    { x.texture->getTexture(), x.texture->getSampler() }
                };
                p.drawUV(
                    "textureScale",
                    Blend::None,
                    mesh(Box2F(0.F, 0.F, tmpSize.w, tmpSize.h)),
                    bufferTransform(tmpSize),
                    &uniforms,
                    sizeof(uniforms),
                    bindings,
                    2);
                popTarget();
            }

            // Then down, over the destination.
            TextureScaleUniforms uniforms;
            uniforms.scaleTaps = y.taps;
            uniforms.scaleVertical = 1;
            const SDL_GPUTextureSamplerBinding bindings[2] =
            {
                { tmp->getTexture(), p.samplerNearest },
                { y.texture->getTexture(), y.texture->getSampler() }
            };
            p.drawUV(
                "textureScale",
                Blend::Default,
                mesh(Box2F(rect.min.x, rect.min.y, rect.w(), rect.h())),
                p.transform,
                &uniforms,
                sizeof(uniforms),
                bindings,
                2);
        }

        bool Render::_drawImageScaled(
            const std::shared_ptr<Image>& image,
            const TriMesh2F& mesh,
            const Color4F& color,
            const ImageOptions& imageOptions,
            const std::vector<std::shared_ptr<Texture> >& textures)
        {
            FTK_P();
            const auto& info = image->getInfo();

            // The destination this covers. Only an axis aligned rectangle
            // can be resampled one axis at a time.
            if (mesh.v.size() != 4 || mesh.triangles.size() != 2)
                return false;
            float minX = mesh.v[0].x, maxX = mesh.v[0].x;
            float minY = mesh.v[0].y, maxY = mesh.v[0].y;
            for (const auto& v : mesh.v)
            {
                minX = std::min(minX, v.x); maxX = std::max(maxX, v.x);
                minY = std::min(minY, v.y); maxY = std::max(maxY, v.y);
            }
            const int outW = static_cast<int>(std::round(maxX - minX));
            const int outH = static_cast<int>(std::round(maxY - minY));
            const int inW = info.size.w;
            const int inH = info.size.h;
            if (outW < 1 || outH < 1 || inW < 1 || inH < 1)
                return false;
            // A draw at the picture's own size has nothing to resample.
            if (outW == inW && outH == inH)
                return false;

            const Private::ScaleTable x = p.scaleTable(inW, outW);
            const Private::ScaleTable y = p.scaleTable(inH, outH);
            const Size2I tmpSize(outW, inH);
            const auto tmp = p.scaleBuffer(tmpSize);

            // Pass one, across, into the intermediate.
            {
                pushTarget(tmp);
                ImageScaleXUniforms uniforms;
                const V4F yuvCoefficients = getYUVCoefficients(info.yuvCoefficients);
                uniforms.yuvCoefficients[0] = yuvCoefficients.x;
                uniforms.yuvCoefficients[1] = yuvCoefficients.y;
                uniforms.yuvCoefficients[2] = yuvCoefficients.z;
                uniforms.yuvCoefficients[3] = yuvCoefficients.w;
                uniforms.imageType = static_cast<int32_t>(info.type);
                uniforms.channelCount = getChannelCount(info.type);
                VideoLevels videoLevels = info.videoLevels;
                switch (imageOptions.videoLevels)
                {
                case InputVideoLevels::FullRange: videoLevels = VideoLevels::FullRange; break;
                case InputVideoLevels::LegalRange: videoLevels = VideoLevels::LegalRange; break;
                default: break;
                }
                uniforms.videoLevels = static_cast<int32_t>(videoLevels);
                uniforms.mirrorX = info.layout.mirror.x;
                uniforms.scaleTaps = x.taps;
                SDL_GPUTextureSamplerBinding bindings[4] = {};
                for (size_t i = 0; i < 3; ++i)
                {
                    const auto& texture = textures[i < textures.size() ? i : 0];
                    bindings[i].texture = texture->getTexture();
                    bindings[i].sampler = texture->getSampler();
                }
                bindings[3].texture = x.texture->getTexture();
                bindings[3].sampler = x.texture->getSampler();
                p.drawUV(
                    "imageScaleX",
                    Blend::None,
                    ftk::mesh(Box2F(0.F, 0.F, tmpSize.w, tmpSize.h)),
                    bufferTransform(tmpSize),
                    &uniforms,
                    sizeof(uniforms),
                    bindings,
                    4);
                popTarget();
            }

            // Pass two, down, over the destination.
            ImageScaleYUniforms uniforms;
            uniforms.color[0] = color.r;
            uniforms.color[1] = color.g;
            uniforms.color[2] = color.b;
            uniforms.color[3] = color.a;
            uniforms.opaque = AlphaBlend::None == imageOptions.alphaBlend;
            uniforms.channelDisplay = static_cast<int32_t>(imageOptions.channelDisplay);
            uniforms.mirrorY = info.layout.mirror.y;
            uniforms.scaleTaps = y.taps;
            const SDL_GPUTextureSamplerBinding bindings[2] =
            {
                { tmp->getTexture(), p.samplerNearest },
                { y.texture->getTexture(), y.texture->getSampler() }
            };
            p.drawUV(
                "imageScaleY",
                getBlend(imageOptions.alphaBlend),
                ftk::mesh(Box2F(minX, minY, maxX - minX, maxY - minY)),
                p.transform,
                &uniforms,
                sizeof(uniforms),
                bindings,
                2);
            return true;
        }

        void Render::drawText(
            const std::vector<std::shared_ptr<Glyph> >& glyphs,
            const FontMetrics& fontMetrics,
            const V2F& pos,
            const Color4F& color)
        {
            FTK_P();

            size_t glyphCount = 0;
            for (const auto& glyph : glyphs)
            {
                if (glyph && glyph->image && glyph->image->isValid())
                {
                    ++glyphCount;
                }
            }
            p.diag.glyphs += glyphCount;

            // A glyph's place in the atlas, put there the first time.
            const int border = 1;
            const float atlasSize = static_cast<float>(p.glyphAtlasSize);
            const auto getItem = [&p, border, atlasSize](
                const std::shared_ptr<Glyph>& glyph,
                RangeF& u,
                RangeF& v)
            {
                std::shared_ptr<BoxPackNode> node;
                if (const auto i = p.glyphIDs.find(glyph->info); i != p.glyphIDs.end())
                {
                    node = p.glyphPack->getNode(i->second);
                }
                if (!node)
                {
                    node = p.glyphPack->insert(glyph->image->getSize() + border * 2);
                    if (node)
                    {
                        p.glyphIDs[glyph->info] = node->id;
                        // The glyph with its border, in one copy.
                        auto tmp = Image::create(node->box.size(), ImageType::L_U8);
                        tmp->zero();
                        const int w = glyph->image->getWidth();
                        const int h = glyph->image->getHeight();
                        const size_t srcRow = glyph->image->getByteCount() / h;
                        const size_t dstRow = tmp->getByteCount() / tmp->getHeight();
                        for (int y = 0; y < h; ++y)
                        {
                            std::memcpy(
                                tmp->getData() + (y + border) * dstRow + border,
                                glyph->image->getData() + y * srcRow,
                                w);
                        }
                        p.glyphTexture->copy(tmp, node->box.min.x, node->box.min.y);
                    }
                }
                if (node)
                {
                    u = RangeF(
                        (node->box.min.x + border) / atlasSize,
                        (node->box.max.x - 1 - border) / atlasSize);
                    v = RangeF(
                        (node->box.min.y + border) / atlasSize,
                        (node->box.max.y - 1 - border) / atlasSize);
                }
                return node.get() != nullptr;
            };

            int x = 0;
            int y = 0;
            int32_t rsbDeltaPrev = 0;
            p.textMesh.v.resize(glyphCount * 4);
            p.textMesh.t.resize(glyphCount * 4);
            p.textMesh.triangles.resize(glyphCount * 2);
            V2F* vP = p.textMesh.v.data();
            V2F* tP = p.textMesh.t.data();
            Triangle2* triP = p.textMesh.triangles.data();
            size_t v = 0;
            size_t t = 0;
            Box2I lineRect(p.clipRect.min.x, pos.y, p.clipRect.w(), fontMetrics.lineHeight);
            for (auto glyphIt = glyphs.begin(); glyphIt != glyphs.end(); ++glyphIt)
            {
                if (*glyphIt)
                {
                    if ('\n' == (*glyphIt)->info.code)
                    {
                        auto crIt = glyphIt + 1;
                        if (crIt != glyphs.end() && *crIt && '\r' == (*crIt)->info.code)
                        {
                            ++glyphIt;
                        }
                        x = 0;
                        y += fontMetrics.lineHeight;
                        rsbDeltaPrev = 0;
                        lineRect = Box2I(p.clipRect.min.x, pos.y + y, p.clipRect.w(), fontMetrics.lineHeight);
                    }
                    else if (!p.clipRectEnabled ||
                        (p.clipRectEnabled && intersects(p.clipRect, lineRect)))
                    {
                        if (rsbDeltaPrev - (*glyphIt)->lsbDelta > 32)
                        {
                            x -= 1;
                        }
                        else if (rsbDeltaPrev - (*glyphIt)->lsbDelta < -31)
                        {
                            x += 1;
                        }
                        rsbDeltaPrev = (*glyphIt)->rsbDelta;

                        RangeF tu;
                        RangeF tv;
                        if ((*glyphIt)->image && (*glyphIt)->image->isValid() &&
                            getItem(*glyphIt, tu, tv))
                        {
                            const V2I& offset = (*glyphIt)->offset;
                            const int extraOffset = 1;
                            const Box2I box(
                                pos.x + x + offset.x,
                                pos.y + y + fontMetrics.ascender - offset.y - extraOffset,
                                (*glyphIt)->image->getWidth(),
                                (*glyphIt)->image->getHeight());

                            vP[0].x = box.min.x;
                            vP[0].y = box.min.y;
                            vP[1].x = box.max.x + 1;
                            vP[1].y = box.min.y;
                            vP[2].x = box.max.x + 1;
                            vP[2].y = box.max.y + 1;
                            vP[3].x = box.min.x;
                            vP[3].y = box.max.y + 1;

                            tP[0].x = tu.min();
                            tP[0].y = tv.min();
                            tP[1].x = tu.max();
                            tP[1].y = tv.min();
                            tP[2].x = tu.max();
                            tP[2].y = tv.max();
                            tP[3].x = tu.min();
                            tP[3].y = tv.max();

                            triP[0].v[0] = { v + 1, v + 1 };
                            triP[0].v[1] = { v + 3, v + 3 };
                            triP[0].v[2] = { v + 2, v + 2 };
                            triP[1].v[0] = { v + 3, v + 3 };
                            triP[1].v[1] = { v + 1, v + 1 };
                            triP[1].v[2] = { v + 4, v + 4 };

                            v += 4;
                            t += 2;
                            vP += 4;
                            tP += 4;
                            triP += 2;
                        }

                        x += (*glyphIt)->advance;
                    }
                }
            }
            p.textMesh.v.resize(v);
            p.textMesh.t.resize(v);
            p.textMesh.triangles.resize(t);

            if (!p.textMesh.triangles.empty())
            {
                const auto uniforms = colorUniforms(color);
                SDL_GPUTextureSamplerBinding binding = {};
                binding.texture = p.glyphTexture->getTexture();
                binding.sampler = p.glyphTexture->getSampler();
                p.drawUV(
                    "text",
                    Blend::Default,
                    p.textMesh,
                    p.transform,
                    &uniforms,
                    sizeof(uniforms),
                    &binding,
                    1);
            }
        }

        std::vector<std::shared_ptr<Texture> > Render::_getTextures(
            const ImageInfo& info,
            const ImageFilters& imageFilters)
        {
            FTK_P();
            std::vector<std::shared_ptr<Texture> > out;
            TextureOptions options;
            options.filters = imageFilters;
            // The two pass path weighs the texels itself, so it wants them as
            // they are rather than blended in pairs first.
            if (ImageFilter::HighQuality == options.filters.minify)
            {
                options.filters.minify = ImageFilter::Nearest;
            }
            if (ImageFilter::HighQuality == options.filters.magnify)
            {
                options.filters.magnify = ImageFilter::Nearest;
            }
            const int w = info.size.w;
            const int h = info.size.h;
            const auto planes = [&](ImageType type, int cw, int ch, ImageType chromaType, int count)
            {
                out.push_back(Texture::create(p.system, ImageInfo(w, h, type), options));
                for (int i = 0; i < count; ++i)
                {
                    out.push_back(Texture::create(p.system, ImageInfo(cw, ch, chromaType), options));
                }
            };
            switch (info.type)
            {
            case ImageType::YUV_420P_U8: planes(ImageType::L_U8, w / 2, h / 2, ImageType::L_U8, 2); break;
            case ImageType::YUV_422P_U8: planes(ImageType::L_U8, w / 2, h, ImageType::L_U8, 2); break;
            case ImageType::YUV_444P_U8: planes(ImageType::L_U8, w, h, ImageType::L_U8, 2); break;
            case ImageType::YUV_420P_U16: planes(ImageType::L_U16, w / 2, h / 2, ImageType::L_U16, 2); break;
            case ImageType::YUV_422P_U16: planes(ImageType::L_U16, w / 2, h, ImageType::L_U16, 2); break;
            case ImageType::YUV_444P_U16: planes(ImageType::L_U16, w, h, ImageType::L_U16, 2); break;
            case ImageType::YUV_420SP_U8: planes(ImageType::L_U8, w / 2, h / 2, ImageType::LA_U8, 1); break;
            case ImageType::YUV_420SP_U16: planes(ImageType::L_U16, w / 2, h / 2, ImageType::LA_U16, 1); break;
            case ImageType::RGB_F16_P: planes(ImageType::L_F16, w, h, ImageType::L_F16, 2); break;
            default:
                if (isTextureSupported(info.type))
                {
                    out.push_back(Texture::create(p.system, info, options));
                }
                break;
            }
            return out;
        }

        void Render::_copyTextures(
            const std::shared_ptr<Image>& image,
            const std::vector<std::shared_ptr<Texture> >& textures)
        {
            const auto& info = image->getInfo();
            switch (info.type)
            {
            case ImageType::YUV_420P_U8:
            case ImageType::YUV_422P_U8:
            case ImageType::YUV_444P_U8:
            case ImageType::YUV_420P_U16:
            case ImageType::YUV_422P_U16:
            case ImageType::YUV_444P_U16:
            case ImageType::YUV_420SP_U8:
            case ImageType::YUV_420SP_U16:
            case ImageType::RGB_F16_P:
            {
                // One plane after another, each the size its texture is.
                const uint8_t* data = image->getData();
                for (const auto& texture : textures)
                {
                    const ImageInfo& planeInfo = texture->getInfo();
                    texture->copy(data, planeInfo);
                    data += planeInfo.getByteCount();
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

        void Render::drawImage(
            const std::shared_ptr<Image>& image,
            const TriMesh2F& mesh,
            const Color4F& color,
            const ImageOptions& imageOptions)
        {
            FTK_P();

            const auto& info = image->getInfo();
            if (!info.isValid())
                return;

            std::vector<std::shared_ptr<Texture> > textures;
            if (!imageOptions.cache)
            {
                const std::string texturePoolKey = getTexturePoolKey(info);
                if (!p.texturePool.get(texturePoolKey, textures))
                {
                    textures = _getTextures(info, imageOptions.imageFilters);
                    p.texturePool.add(texturePoolKey, textures, image->getByteCount());
                }
                _copyTextures(image, textures);
            }
            else if (!p.textureCache.get(image, textures))
            {
                textures = _getTextures(info, imageOptions.imageFilters);
                _copyTextures(image, textures);
                p.textureCache.add(image, textures, image->getByteCount());
            }
            if (textures.empty())
                return;
            p.diag.textures += textures.size();

            if (ImageFilter::HighQuality == imageOptions.imageFilters.minify &&
                _drawImageScaled(image, mesh, color, imageOptions, textures))
            {
                return;
            }

            ImageUniforms uniforms;
            uniforms.color[0] = color.r;
            uniforms.color[1] = color.g;
            uniforms.color[2] = color.b;
            uniforms.color[3] = color.a;
            const V4F yuvCoefficients = getYUVCoefficients(info.yuvCoefficients);
            uniforms.yuvCoefficients[0] = yuvCoefficients.x;
            uniforms.yuvCoefficients[1] = yuvCoefficients.y;
            uniforms.yuvCoefficients[2] = yuvCoefficients.z;
            uniforms.yuvCoefficients[3] = yuvCoefficients.w;
            uniforms.opaque = AlphaBlend::None == imageOptions.alphaBlend;
            uniforms.imageType = static_cast<int32_t>(info.type);
            uniforms.channelCount = getChannelCount(info.type);
            uniforms.channelDisplay = static_cast<int32_t>(imageOptions.channelDisplay);
            VideoLevels videoLevels = info.videoLevels;
            switch (imageOptions.videoLevels)
            {
            case InputVideoLevels::FullRange: videoLevels = VideoLevels::FullRange; break;
            case InputVideoLevels::LegalRange: videoLevels = VideoLevels::LegalRange; break;
            default: break;
            }
            uniforms.videoLevels = static_cast<int32_t>(videoLevels);
            uniforms.mirrorX = info.layout.mirror.x;
            uniforms.mirrorY = info.layout.mirror.y;

            // All three are bound whatever the image has: the shader
            // declares them, and it is the uniforms that say which are read.
            SDL_GPUTextureSamplerBinding bindings[3] = {};
            for (size_t i = 0; i < 3; ++i)
            {
                const auto& texture = textures[i < textures.size() ? i : 0];
                bindings[i].texture = texture->getTexture();
                bindings[i].sampler = texture->getSampler();
            }

            p.drawUV(
                "image",
                getBlend(imageOptions.alphaBlend),
                mesh,
                p.transform,
                &uniforms,
                sizeof(uniforms),
                bindings,
                3);
        }

        void Render::drawImage(
            const std::shared_ptr<Image>& image,
            const Box2F& box,
            const Color4F& color,
            const ImageOptions& imageOptions)
        {
            drawImage(image, mesh(box), color, imageOptions);
        }
    }
}
