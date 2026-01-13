// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/RenderPrivate.h>

#include <ftk/GL/Util.h>

namespace ftk
{
    namespace gl
    {
        void Render::drawRect(
            const Box2F& rect,
            const Color4F& color)
        {
            FTK_P();

            p.shaders["rect"]->bind();
            p.shaders["rect"]->setUniform("color", color);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (p.vbos["rect"])
            {
                const auto mesh = ftk::mesh(rect);
                p.vbos["rect"]->copy(convert(mesh, p.vbos["rect"]->getType()));
                p.stats.triCount += mesh.triangles.size();
            }
            if (p.vaos["rect"])
            {
                p.vaos["rect"]->bind();
                p.vaos["rect"]->draw(GL_TRIANGLES, 0, p.vbos["rect"]->getSize());
            }
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
            FTK_P();

            p.shaders["line"]->bind();
            p.shaders["line"]->setUniform("color", color);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            const V2F v2 = normalize(v1 - v0);
            const V2F v2CW = perpCW(v2) * options.width / 2.F;
            const V2F v2CCW = perpCCW(v2) * options.width / 2.F;
            TriMesh2F mesh;
            mesh.v.push_back(v0 + v2CCW);
            mesh.v.push_back(v0 + v2CW);
            mesh.v.push_back(v1 + v2CW);
            mesh.v.push_back(v1 + v2CCW);
            mesh.triangles.push_back({ 1, 3, 2 });
            mesh.triangles.push_back({ 3, 1, 4 });

            if (p.vbos["line"])
            {
                p.vbos["line"]->copy(convert(mesh, p.vbos["line"]->getType()));
                p.stats.triCount += mesh.triangles.size();
            }
            if (p.vaos["line"])
            {
                p.vaos["line"]->bind();
                p.vaos["line"]->draw(GL_TRIANGLES, 0, p.vbos["line"]->getSize());
            }
        }

        void Render::drawLines(
            const std::vector<std::pair<V2F, V2F> >& lines,
            const Color4F& color,
            const LineOptions& options)
        {
            FTK_P();
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
            const size_t size = mesh.triangles.size();
            if (size > 0)
            {
                p.shaders["mesh"]->bind();
                const auto transform =
                    p.transform *
                    translate(V3F(pos.x, pos.y, 0.F));
                p.shaders["mesh"]->setUniform("transform.mvp", transform);
                p.shaders["mesh"]->setUniform("color", color);

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                if (!p.vbos["mesh"] || (p.vbos["mesh"] && p.vbos["mesh"]->getSize() < size * 3))
                {
                    p.vbos["mesh"] = VBO::create(size * 3, VBOType::Pos2_F32);
                    p.vaos["mesh"].reset();
                }
                if (p.vbos["mesh"])
                {
                    p.vbos["mesh"]->copy(convert(mesh, VBOType::Pos2_F32));
                    p.stats.triCount += mesh.triangles.size();
                }

                if (!p.vaos["mesh"] && p.vbos["mesh"])
                {
                    p.vaos["mesh"] = VAO::create(p.vbos["mesh"]->getType(), p.vbos["mesh"]->getID());
                }
                if (p.vaos["mesh"] && p.vbos["mesh"])
                {
                    p.vaos["mesh"]->bind();
                    p.vaos["mesh"]->draw(GL_TRIANGLES, 0, size * 3);
                }
            }
        }
        
        void Render::drawColorMesh(
            const TriMesh2F& mesh,
            const Color4F& color,
            const V2F& pos)
        {
            FTK_P();
            const size_t size = mesh.triangles.size();
            if (size > 0)
            {
                p.shaders["colorMesh"]->bind();
                const auto transform =
                    p.transform *
                    translate(V3F(pos.x, pos.y, 0.F));
                p.shaders["colorMesh"]->setUniform("transform.mvp", transform);
                p.shaders["colorMesh"]->setUniform("color", color);

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                if (!p.vbos["colorMesh"] || (p.vbos["colorMesh"] && p.vbos["colorMesh"]->getSize() < size * 3))
                {
                    p.vbos["colorMesh"] = VBO::create(size * 3, VBOType::Pos2_F32_Color_F32);
                    p.vaos["colorMesh"].reset();
                }
                if (p.vbos["colorMesh"])
                {
                    p.vbos["colorMesh"]->copy(convert(mesh, VBOType::Pos2_F32_Color_F32));
                    p.stats.triCount += mesh.triangles.size();
                }

                if (!p.vaos["colorMesh"] && p.vbos["colorMesh"])
                {
                    p.vaos["colorMesh"] = VAO::create(p.vbos["colorMesh"]->getType(), p.vbos["colorMesh"]->getID());
                }
                if (p.vaos["colorMesh"] && p.vbos["colorMesh"])
                {
                    p.vaos["colorMesh"]->bind();
                    p.vaos["colorMesh"]->draw(GL_TRIANGLES, 0, size * 3);
                }
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
            p.shaders["texture"]->bind();
            p.shaders["texture"]->setUniform("color", color);
            p.shaders["texture"]->setUniform("opaque", AlphaBlend::None == alphaBlend);
            p.shaders["texture"]->setUniform("textureSampler", 0);

            if (alphaBlend != AlphaBlend::None)
            {
                setAlphaBlend(alphaBlend);
            }
            else
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0));
            glBindTexture(GL_TEXTURE_2D, id);

            if (p.vbos["texture"])
            {
                p.vbos["texture"]->copy(convert(mesh(rect, mirrorV), p.vbos["texture"]->getType()));
            }
            if (p.vaos["texture"])
            {
                p.vaos["texture"]->bind();
                p.vaos["texture"]->draw(GL_TRIANGLES, 0, p.vbos["texture"]->getSize());
            }
        }

        void Render::drawText(
            const std::vector<std::shared_ptr<Glyph> >& glyphs,
            const FontMetrics& fontMetrics,
            const V2F& pos,
            const Color4F& color)
        {
            FTK_P();

            p.shaders["text"]->bind();
            p.shaders["text"]->setUniform("color", color);
            p.shaders["text"]->setUniform("textureSampler", 0);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0));
            glBindTexture(GL_TEXTURE_2D, p.glyphAtlas->getTexture());

            size_t glyphCount = 0;
            for (const auto& glyph : glyphs)
            {
                if (glyph && glyph->image && glyph->image->isValid())
                {
                    ++glyphCount;
                }
            }
            p.stats.glyphCount += glyphCount;

            int x = 0;
            int y = 0;
            int32_t rsbDeltaPrev = 0;
            p.textMesh.v.resize(glyphCount * 4);
            p.textMesh.t.resize(glyphCount * 4);
            p.textMesh.triangles.resize(glyphCount * 2);
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
                        if (crIt != glyphs.end() && '\r' == (*glyphIt)->info.code)
                        {
                            ++glyphIt;
                        }
                        x = 0;
                        y += fontMetrics.lineHeight;
                        rsbDeltaPrev = 0;
                        lineRect = Box2I(p.clipRect.min.x, pos.y + y, p.clipRect.w(), fontMetrics.lineHeight);
                    }
                    else if (!p.clipRectEnabled ||
                        p.clipRectEnabled && intersects(p.clipRect, lineRect))
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

                        if ((*glyphIt)->image && (*glyphIt)->image->isValid())
                        {
                            BoxPackID id = boxPackInvalidID;
                            const auto j = p.glyphIDs.find((*glyphIt)->info);
                            if (j != p.glyphIDs.end())
                            {
                                id = j->second;
                            }
                            TextureAtlasItem item;
                            if (boxPackInvalidID == id ||
                                !p.glyphAtlas->getItem(id, item))
                            {
                                p.glyphAtlas->addItem((*glyphIt)->image, item);
                                p.glyphIDs[(*glyphIt)->info] = item.id;
                            }

                            const V2I& offset = (*glyphIt)->offset;
                            //! \bug Off by one?
                            const int extraOffset = 1;
                            const Box2I box(
                                pos.x + x + offset.x,
                                pos.y + y + fontMetrics.ascender - offset.y - extraOffset,
                                (*glyphIt)->image->getWidth(),
                                (*glyphIt)->image->getHeight());
                            const V2I& min = box.min;
                            const V2I& max = box.max;

                            p.textMesh.v[v + 0].x = min.x;
                            p.textMesh.v[v + 0].y = min.y;
                            p.textMesh.v[v + 1].x = max.x + 1;
                            p.textMesh.v[v + 1].y = min.y;
                            p.textMesh.v[v + 2].x = max.x + 1;
                            p.textMesh.v[v + 2].y = max.y + 1;
                            p.textMesh.v[v + 3].x = min.x;
                            p.textMesh.v[v + 3].y = max.y + 1;
                            p.textMesh.t[v + 0].x = item.u.min();
                            p.textMesh.t[v + 0].y = item.v.min();
                            p.textMesh.t[v + 1].x = item.u.max();
                            p.textMesh.t[v + 1].y = item.v.min();
                            p.textMesh.t[v + 2].x = item.u.max();
                            p.textMesh.t[v + 2].y = item.v.max();
                            p.textMesh.t[v + 3].x = item.u.min();
                            p.textMesh.t[v + 3].y = item.v.max();

                            p.textMesh.triangles[t + 0].v[0] = { v + 1, v + 1 };
                            p.textMesh.triangles[t + 0].v[1] = { v + 3, v + 3 };
                            p.textMesh.triangles[t + 0].v[2] = { v + 2, v + 2 };
                            p.textMesh.triangles[t + 1].v[0] = { v + 3, v + 3 };
                            p.textMesh.triangles[t + 1].v[1] = { v + 1, v + 1 };
                            p.textMesh.triangles[t + 1].v[2] = { v + 4, v + 4 };

                            v += 4;
                            t += 2;
                        }

                        x += (*glyphIt)->advance;
                    }
                }
            }
            _drawTextMesh(p.textMesh);
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
                textures = _getTextures(info, imageOptions.imageFilters);
                _copyTextures(image, textures);
            }
            else if (!p.textureCache->get(image, textures))
            {
                textures = _getTextures(info, imageOptions.imageFilters);
                _copyTextures(image, textures);
                p.textureCache->add(image, textures, image->getByteCount());
            }
            _setActiveTextures(info, textures);
            p.stats.textureCount += textures.size();

            p.shaders["image"]->bind();
            p.shaders["image"]->setUniform("color", color);
            p.shaders["texture"]->setUniform("opaque", AlphaBlend::None == imageOptions.alphaBlend);
            p.shaders["image"]->setUniform("imageType", static_cast<int>(info.type));
            p.shaders["image"]->setUniform("channelCount", getChannelCount(info.type));
            p.shaders["image"]->setUniform("channelDisplay", static_cast<int>(imageOptions.channelDisplay));
            VideoLevels videoLevels = info.videoLevels;
            switch (imageOptions.videoLevels)
            {
            case InputVideoLevels::FullRange:
                videoLevels = VideoLevels::FullRange;
                break;
            case InputVideoLevels::LegalRange:
                videoLevels = VideoLevels::LegalRange;
                break;
            default: break;
            }
            p.shaders["image"]->setUniform("videoLevels", static_cast<int>(videoLevels));
            p.shaders["image"]->setUniform("yuvCoefficients", getYUVCoefficients(info.yuvCoefficients));
            p.shaders["image"]->setUniform("mirrorX", info.layout.mirror.x);
            p.shaders["image"]->setUniform("mirrorY", info.layout.mirror.y);
            switch (info.type)
            {
            case ImageType::YUV_420P_U8:
            case ImageType::YUV_422P_U8:
            case ImageType::YUV_444P_U8:
            case ImageType::YUV_420P_U16:
            case ImageType::YUV_422P_U16:
            case ImageType::YUV_444P_U16:
                p.shaders["image"]->setUniform("textureSampler1", 1);
                p.shaders["image"]->setUniform("textureSampler2", 2);
            default:
                p.shaders["image"]->setUniform("textureSampler0", 0);
                break;
            }

            if (imageOptions.alphaBlend != AlphaBlend::None)
            {
                setAlphaBlend(imageOptions.alphaBlend);
            }
            else
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            const size_t size = mesh.triangles.size();
            if (!p.vbos["image"] || (p.vbos["image"] && p.vbos["image"]->getSize() < size * 3))
            {
                p.vbos["image"] = VBO::create(size * 3, VBOType::Pos2_F32_UV_U16);
                p.vaos["image"].reset();
            }
            if (p.vbos["image"])
            {
                p.vbos["image"]->copy(convert(mesh, VBOType::Pos2_F32_UV_U16));
                p.stats.triCount += mesh.triangles.size();
            }

            if (!p.vaos["image"] && p.vbos["image"])
            {
                p.vaos["image"] = VAO::create(p.vbos["image"]->getType(), p.vbos["image"]->getID());
            }
            if (p.vaos["image"] && p.vbos["image"])
            {
                p.vaos["image"]->bind();
                p.vaos["image"]->draw(GL_TRIANGLES, 0, size * 3);
            }
        }

        void Render::drawImage(
            const std::shared_ptr<Image>& image,
            const Box2F& box,
            const Color4F& color,
            const ImageOptions& imageOptions)
        {
            drawImage(image, mesh(box), color, imageOptions);
        }

        void Render::_drawTextMesh(const TriMesh2F& mesh)
        {
            FTK_P();
            const size_t size = mesh.triangles.size();
            if (size > 0)
            {
                if (!p.vbos["text"] || (p.vbos["text"] && p.vbos["text"]->getSize() < size * 3))
                {
                    p.vbos["text"] = VBO::create(size * 3, VBOType::Pos2_F32_UV_U16);
                    p.vaos["text"].reset();
                }
                if (p.vbos["text"])
                {
                    p.vbos["text"]->copy(convert(mesh, p.vbos["text"]->getType()));
                    p.stats.triCount += mesh.triangles.size();
                }
                if (!p.vaos["text"] && p.vbos["text"])
                {
                    p.vaos["text"] = VAO::create(p.vbos["text"]->getType(), p.vbos["text"]->getID());
                }
                if (p.vaos["text"] && p.vbos["text"])
                {
                    p.vaos["text"]->bind();
                    p.vaos["text"]->draw(GL_TRIANGLES, 0, size * 3);
                }
            }
        }
    }
}

