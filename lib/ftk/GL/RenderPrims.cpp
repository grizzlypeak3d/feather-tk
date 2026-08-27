// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/RenderPrivate.h>

#include <ftk/GL/Util.h>

#include <sstream>

namespace ftk
{
    namespace gl
    {

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
            FTK_P();

            p.shaders["line"]->bind();
            p.shaders["line"]->setUniform("color", color);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            const V2F v2 = normalize(v1 - v0);
            const V2F v2CW = perpCW(v2) * options.width / 2.F;
            const V2F v2CCW = perpCCW(v2) * options.width / 2.F;
            TriMesh2F mesh;
            mesh.v.emplace_back(v0 + v2CCW);
            mesh.v.emplace_back(v0 + v2CW);
            mesh.v.emplace_back(v1 + v2CW);
            mesh.v.emplace_back(v1 + v2CCW);
            mesh.triangles.emplace_back(1, 3, 2);
            mesh.triangles.emplace_back(3, 1, 4);

            if (p.vbos["line"])
            {
                p.vbos["line"]->copy(convert(mesh, p.vbos["line"]->getType()));
                p.diag.triangles += mesh.triangles.size();
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
                    p.diag.triangles += mesh.triangles.size();
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
                    p.diag.triangles += mesh.triangles.size();
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
            p.diag.glyphs += glyphCount;

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

                        if ((*glyphIt)->image && (*glyphIt)->image->isValid())
                        {
                            BoxPackID id = boxPackInvalidID;
                            if (const auto j = p.glyphIDs.find((*glyphIt)->info);
                                j != p.glyphIDs.end())
                            {
                                id = j->second;
                            }
                            TextureAtlasItem item;
                            bool valid =
                                boxPackInvalidID != id &&
                                p.glyphAtlas->getItem(id, item);
                            if (!valid)
                            {
                                // Atlas insertion fails when the atlas is full;
                                // skip the glyph rather than drawing it with an
                                // invalid (degenerate) texture region.
                                valid = p.glyphAtlas->addItem((*glyphIt)->image, item);
                                if (valid)
                                {
                                    p.glyphIDs[(*glyphIt)->info] = item.id;
                                }
                            }

                            if (valid)
                            {
                                const V2I& offset = (*glyphIt)->offset;
                                //! \bug Off by one?
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

                                tP[0].x = item.u.min();
                                tP[0].y = item.v.min();
                                tP[1].x = item.u.max();
                                tP[1].y = item.v.min();
                                tP[2].x = item.u.max();
                                tP[2].y = item.v.max();
                                tP[3].x = item.u.min();
                                tP[3].y = item.v.max();

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
                        }

                        x += (*glyphIt)->advance;
                    }
                }
            }

            // Glyphs skipped by clipping leave unused slots at the end of the
            // (reused) mesh buffers; trim to the emitted counts so stale data
            // from a previous frame is not drawn.
            p.textMesh.v.resize(v);
            p.textMesh.t.resize(v);
            p.textMesh.triangles.resize(t);

            _drawTextMesh(p.textMesh);
        }

        namespace
        {
            std::string getTexturePoolKey(const ImageInfo& value)
            {
                std::stringstream ss;
                ss << value.size << " " << value.type;
                return ss.str();
            }
        }

        namespace
        {
            // Lanczos, windowed at three lobes. Wider than a cubic and the
            // usual choice for reduction: it keeps detail a box or a triangle
            // would smear, at the cost of a little ringing on hard edges.
            const float lanczos3Support = 3.F;

            float lanczos3(float x)
            {
                x = std::fabs(x);
                if (x < 0.0001F)
                    return 1.F;
                if (x >= lanczos3Support)
                    return 0.F;
                const float pix = pi * x;
                return
                    (std::sin(pix) / pix) *
                    (std::sin(pix / lanczos3Support) / (pix / lanczos3Support));
            }

            // Mitchell-Netravali with B = C = 1/3, which is what enlarging
            // wants: Lanczos rings, and a halo along a hard edge is the last
            // thing to put in front of someone judging a picture.
            const float mitchellSupport = 2.F;

            float mitchell(float x)
            {
                const float b = 1.F / 3.F;
                const float c = 1.F / 3.F;
                x = std::fabs(x);
                const float x2 = x * x;
                const float x3 = x2 * x;
                if (x < 1.F)
                {
                    return (
                        (12.F - 9.F * b - 6.F * c) * x3 +
                        (-18.F + 12.F * b + 6.F * c) * x2 +
                        (6.F - 2.F * b)) / 6.F;
                }
                if (x < mitchellSupport)
                {
                    return (
                        (-b - 6.F * c) * x3 +
                        (6.F * b + 30.F * c) * x2 +
                        (-12.F * b - 48.F * c) * x +
                        (8.F * b + 24.F * c)) / 6.F;
                }
                return 0.F;
            }

            // For each output pixel, the source coordinate and weight of every
            // tap. Column = output pixel, row = tap; the shader walks the rows.
            //
            // The weights of a pixel are divided by their sum. That is not a
            // correction for anything wrong: the kernel integrates to one, but
            // the taps are a finite sample of it at whatever sub-pixel phase
            // the output pixel lands on, and that sum drifts by a few percent
            // as the phase moves. Left alone the drift beats against the output
            // grid and shows up as bands across a flat area.
            std::shared_ptr<Image> scaleContrib(int in, int out, int& taps)
            {
                // Which kernel is a question about this axis alone: an
                // anamorphic picture can be reduced across and enlarged down.
                const float scale = out / static_cast<float>(in);
                const bool reducing = scale < 1.F;
                float (*fnc)(float) = reducing ? lanczos3 : mitchell;
                const float support = reducing ? lanczos3Support : mitchellSupport;
                const float radius = reducing ? support / scale : support;
                taps = static_cast<int>(std::ceil(radius * 2.F + 1.F));

                auto data = Image::create(ImageInfo(out, taps, ImageType::LA_F32));
                float* p = reinterpret_cast<float*>(data->getData());
                for (int i = 0; i < out; ++i)
                {
                    // The centre of output pixel i in source pixels, and the
                    // source pixels its kernel reaches.
                    const float center = (i + .5F) / scale - .5F;
                    const int left = static_cast<int>(std::ceil(center - radius));
                    const int right = static_cast<int>(std::floor(center + radius));

                    float sum = 0.F;
                    int j = 0;
                    int pixel = 0;
                    for (int k = left; j < taps && k <= right; ++j, ++k)
                    {
                        // Outside the picture the edge pixel is repeated,
                        // rather than dropped, so that the weights of a pixel
                        // on the border still cover it.
                        pixel = std::clamp(k, 0, in - 1);
                        const float x = (center - k) * (reducing ? scale : 1.F);
                        const float w = reducing ? fnc(x) * scale : fnc(x);
                        // The texel's centre, which is what a nearest fetch of
                        // this coordinate returns.
                        p[(j * out + i) * 2 + 0] = (pixel + .5F) / in;
                        p[(j * out + i) * 2 + 1] = w;
                        sum += w;
                    }
                    for (; j < taps; ++j)
                    {
                        p[(j * out + i) * 2 + 0] = (pixel + .5F) / in;
                        p[(j * out + i) * 2 + 1] = 0.F;
                    }
                    if (sum > 0.F)
                    {
                        for (j = 0; j < taps; ++j)
                        {
                            p[(j * out + i) * 2 + 1] /= sum;
                        }
                    }
                }
                return data;
            }

            std::shared_ptr<Texture> contribTexture(const std::shared_ptr<Image>& data)
            {
                TextureOptions options;
                options.filters.minify = ImageFilter::Nearest;
                options.filters.magnify = ImageFilter::Nearest;
                auto out = Texture::create(data->getInfo(), options);
                out->copy(data);
                return out;
            }
        }

        namespace
        {
            // Enough for a comparison of a few pictures at one zoom; a zoom
            // walks through sizes, so this is bounded rather than kept.
            const size_t scaleTableMax = 8;
            const size_t scaleBufferMax = 4;
        }

        void Render::_scaleContribUpdate(const Size2I& source, const Size2I& dest)
        {
            FTK_P();
            const auto find = [this](int in, int out, std::shared_ptr<Texture>& texture, int& taps)
            {
                FTK_P();
                for (auto i = p.scale.tables.begin(); i != p.scale.tables.end(); ++i)
                {
                    if (i->in == in && i->out == out)
                    {
                        // To the front, so what a frame keeps asking for is
                        // not the thing that falls off the end.
                        p.scale.tables.splice(p.scale.tables.begin(), p.scale.tables, i);
                        texture = p.scale.tables.front().texture;
                        taps = p.scale.tables.front().taps;
                        return;
                    }
                }
                Render::Private::ScaleData::Table table;
                table.in = in;
                table.out = out;
                table.texture = contribTexture(scaleContrib(in, out, table.taps));
                p.scale.tables.push_front(table);
                while (p.scale.tables.size() > scaleTableMax)
                {
                    p.scale.tables.pop_back();
                }
                texture = table.texture;
                taps = table.taps;
            };
            find(source.w, dest.w, p.scale.xContrib, p.scale.xTaps);
            find(source.h, dest.h, p.scale.yContrib, p.scale.yTaps);
        }

        bool Render::_scaleBufferUpdate(const Size2I& size)
        {
            FTK_P();
            for (auto i = p.scale.buffers.begin(); i != p.scale.buffers.end(); ++i)
            {
                if (*i && (*i)->getSize() == size)
                {
                    p.scale.buffers.splice(p.scale.buffers.begin(), p.scale.buffers, i);
                    p.scale.buffer = p.scale.buffers.front();
                    return true;
                }
            }
            p.scale.buffer = OffscreenBuffer::create(size, TextureType::RGBA_F16);
            if (!p.scale.buffer)
                return false;
            p.scale.buffers.push_front(p.scale.buffer);
            while (p.scale.buffers.size() > scaleBufferMax)
            {
                p.scale.buffers.pop_back();
            }
            return true;
        }

        void Render::drawTextureScaled(
            unsigned int id,
            const Size2I& sourceSize,
            const Box2I& rect,
            bool mirrorV)
        {
#if defined(FTK_API_GLES_3)
            // Not served here. GLSL ES 1.00 requires a loop bound the compiler
            // can see, and the tap count is only known once the scale is; the
            // weights also want more precision than mediump, and a table of
            // them is a float texture. High quality falls back to Linear
            // rather than growing a second implementation.
            drawTexture(id, rect, mirrorV);
#else // FTK_API_GLES_3
            FTK_P();
            const Size2I destSize = rect.size();
            if (!sourceSize.isValid() || !destSize.isValid() ||
                (destSize.w >= sourceSize.w && destSize.h >= sourceSize.h))
            {
                drawTexture(id, rect, mirrorV);
                return;
            }

            if (!p.shaders["textureScale"])
            {
                p.shaders["textureScale"] = Shader::create(
                    vertexSource(), textureScaleFragmentSource());
                p.shaders["textureScale"]->bind();
                p.shaders["textureScale"]->setUniform("transform.mvp", p.transform);
            }
            _scaleContribUpdate(sourceSize, destSize);

            // Across first, into an intermediate that is already narrowed but
            // still full height.
            const Size2I tmpSize(destSize.w, sourceSize.h);
            if (!_scaleBufferUpdate(tmpSize))
            {
                drawTexture(id, rect, mirrorV);
                return;
            }

            // The viewport is taken as it is rather than through
            // setViewport(), which derives it from the size passed to begin();
            // this may be drawing into a buffer the caller sized itself.
            GLint savedViewport[4] = { 0, 0, 0, 0 };
            glGetIntegerv(GL_VIEWPORT, savedViewport);
            const M44F savedTransform = p.transform;
            auto& shader = p.shaders["textureScale"];

            {
                OffscreenBufferBinding binding(p.scale.buffer);
                glViewport(0, 0, tmpSize.w, tmpSize.h);
                glDisable(GL_BLEND);
                setTransform(ortho(
                    0.F, static_cast<float>(tmpSize.w),
                    static_cast<float>(tmpSize.h), 0.F, -1.F, 1.F));
                shader->bind();
                shader->setUniform("scaleVertical", false);
                shader->setUniform("scaleTaps", p.scale.xTaps);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, id);
                shader->setUniform("textureSampler", 0);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, p.scale.xContrib->getID());
                shader->setUniform("scaleContrib", 3);
                _drawScaleQuad(Box2F(0.F, 0.F, tmpSize.w, tmpSize.h));
            }

            glViewport(
                savedViewport[0], savedViewport[1],
                savedViewport[2], savedViewport[3]);
            setTransform(savedTransform);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            shader->bind();
            shader->setUniform("transform.mvp", p.transform);
            shader->setUniform("scaleVertical", true);
            shader->setUniform("scaleTaps", p.scale.yTaps);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, p.scale.buffer->getColorID());
            shader->setUniform("textureSampler", 0);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, p.scale.yContrib->getID());
            shader->setUniform("scaleContrib", 3);
            _drawScaleQuad(Box2F(rect.min.x, rect.min.y, rect.w(), rect.h()));

            // Back to the first unit. Texture::copy() and the Texture
            // constructor bind without choosing a unit, so they land on
            // whichever one was left active -- the glyph atlas uploads that
            // way, and would otherwise be bound over whatever this left on
            // unit three.
            glActiveTexture(GL_TEXTURE0);
#endif // FTK_API_GLES_3
        }

        bool Render::_drawImageScaled(
            const std::shared_ptr<Image>& image,
            const TriMesh2F& mesh,
            const Color4F& color,
            const ImageOptions& imageOptions,
            const std::vector<std::shared_ptr<Texture> >& textures)
        {
#if defined(FTK_API_GLES_3)
            // See drawTextureScaled().
            return false;
#else // FTK_API_GLES_3
            FTK_P();
            const auto& info = image->getInfo();

            // The destination this covers. Only an axis aligned rectangle can
            // be resampled one axis at a time, so anything else goes the
            // ordinary way.
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

            if (!p.shaders["imageScaleX"])
            {
                p.shaders["imageScaleX"] = Shader::create(
                    vertexSource(), imageScaleXFragmentSource());
                p.shaders["imageScaleX"]->bind();
                p.shaders["imageScaleX"]->setUniform("transform.mvp", p.transform);
            }
            if (!p.shaders["imageScaleY"])
            {
                p.shaders["imageScaleY"] = Shader::create(
                    vertexSource(), imageScaleYFragmentSource());
            }

            _scaleContribUpdate(Size2I(inW, inH), Size2I(outW, outH));

            // The intermediate: narrowed across, still full height.
            const Size2I tmpSize(outW, inH);
            if (!_scaleBufferUpdate(tmpSize))
                return false;

            GLint savedViewport[4] = { 0, 0, 0, 0 };
            glGetIntegerv(GL_VIEWPORT, savedViewport);
            const M44F savedTransform = p.transform;

            // Pass one, across, into the intermediate.
            {
                OffscreenBufferBinding binding(p.scale.buffer);
                glViewport(0, 0, tmpSize.w, tmpSize.h);
                glDisable(GL_BLEND);
                setTransform(ortho(
                    0.F,
                    static_cast<float>(tmpSize.w),
                    static_cast<float>(tmpSize.h),
                    0.F,
                    -1.F,
                    1.F));

                auto& shader = p.shaders["imageScaleX"];
                shader->bind();
                shader->setUniform("imageType", static_cast<int>(info.type));
                shader->setUniform("channelCount", getChannelCount(info.type));
                VideoLevels videoLevels = info.videoLevels;
                switch (imageOptions.videoLevels)
                {
                case InputVideoLevels::FullRange: videoLevels = VideoLevels::FullRange; break;
                case InputVideoLevels::LegalRange: videoLevels = VideoLevels::LegalRange; break;
                default: break;
                }
                shader->setUniform("videoLevels", static_cast<int>(videoLevels));
                shader->setUniform("yuvCoefficients", getYUVCoefficients(info.yuvCoefficients));
                shader->setUniform("mirrorX", info.layout.mirror.x);
                shader->setUniform("scaleTaps", p.scale.xTaps);
                _setActiveTextures(shader, info, textures);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, p.scale.xContrib->getID());
                shader->setUniform("scaleContrib", 3);

                _drawScaleQuad(Box2F(0.F, 0.F, tmpSize.w, tmpSize.h));
            }

            // Pass two, down, over the destination.
            glViewport(
                savedViewport[0], savedViewport[1],
                savedViewport[2], savedViewport[3]);
            setTransform(savedTransform);
            // Blending was turned off for the first pass, and setAlphaBlend()
            // only chooses the function -- it does not turn it back on, since
            // everything else relies on begin() having enabled it once. Turn
            // it on here or it stays off for the rest of the frame, and every
            // glyph drawn afterwards is a filled rectangle.
            glEnable(GL_BLEND);
            if (imageOptions.alphaBlend != AlphaBlend::None)
            {
                setAlphaBlend(imageOptions.alphaBlend);
            }
            else
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            auto& shader = p.shaders["imageScaleY"];
            shader->bind();
            shader->setUniform("transform.mvp", p.transform);
            shader->setUniform("color", color);
            shader->setUniform("opaque", AlphaBlend::None == imageOptions.alphaBlend);
            shader->setUniform("channelDisplay", static_cast<int>(imageOptions.channelDisplay));
            shader->setUniform("mirrorY", info.layout.mirror.y);
            shader->setUniform("scaleTaps", p.scale.yTaps);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, p.scale.buffer->getColorID());
            shader->setUniform("textureSampler0", 0);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, p.scale.yContrib->getID());
            shader->setUniform("scaleContrib", 3);

            _drawScaleQuad(Box2F(minX, minY, maxX - minX, maxY - minY));


            // See drawTextureScaled().
            glActiveTexture(GL_TEXTURE0);
            return true;
#endif // FTK_API_GLES_3
        }

        void Render::_drawScaleQuad(const Box2F& box)
        {
            FTK_P();
            const auto m = mesh(box);
            const size_t size = m.triangles.size();
            if (!p.vbos["imageScale"] ||
                (p.vbos["imageScale"] && p.vbos["imageScale"]->getSize() < size * 3))
            {
                p.vbos["imageScale"] = VBO::create(size * 3, VBOType::Pos2_F32_UV_U16);
                p.vaos["imageScale"].reset();
            }
            if (p.vbos["imageScale"])
            {
                p.vbos["imageScale"]->copy(convert(m, VBOType::Pos2_F32_UV_U16));
                p.diag.triangles += size;
            }
            if (!p.vaos["imageScale"] && p.vbos["imageScale"])
            {
                p.vaos["imageScale"] = VAO::create(
                    p.vbos["imageScale"]->getType(), p.vbos["imageScale"]->getID());
            }
            if (p.vaos["imageScale"] && p.vbos["imageScale"])
            {
                p.vaos["imageScale"]->bind();
                p.vaos["imageScale"]->draw(GL_TRIANGLES, 0, size * 3);
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
            p.diag.textures += textures.size();

            if (ImageFilter::HighQuality == imageOptions.imageFilters.minify &&
                _drawImageScaled(image, mesh, color, imageOptions, textures))
            {
                return;
            }

            p.shaders["image"]->bind();
            _setActiveTextures(p.shaders["image"], info, textures);
            p.shaders["image"]->setUniform("color", color);
            p.shaders["image"]->setUniform("opaque", AlphaBlend::None == imageOptions.alphaBlend);
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
                p.diag.triangles += mesh.triangles.size();
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
                    p.diag.triangles += mesh.triangles.size();
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

