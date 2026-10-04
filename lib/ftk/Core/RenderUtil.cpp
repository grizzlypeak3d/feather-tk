// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/RenderUtil.h>

#include <ftk/Core/IRender.h>
#include <ftk/Core/Math.h>

#include <algorithm>
#include <cmath>

namespace ftk
{
    struct RenderSizeState::Private
    {
        std::shared_ptr<IRender> render;
        Size2I size;
    };

    RenderSizeState::RenderSizeState(const std::shared_ptr<IRender>& render) :
        _p(new Private)
    {
        FTK_P();
        p.render = render;
        p.size = render->getRenderSize();
    }

    RenderSizeState::~RenderSizeState()
    {
        FTK_P();
        p.render->setRenderSize(p.size);
    }

    struct ViewportState::Private
    {
        std::shared_ptr<IRender> render;
        Box2I viewport;
    };

    ViewportState::ViewportState(const std::shared_ptr<IRender>& render) :
        _p(new Private)
    {
        FTK_P();
        p.render = render;
        p.viewport = render->getViewport();
    }

    ViewportState::~ViewportState()
    {
        FTK_P();
        p.render->setViewport(p.viewport);
    }

    struct ClipRectEnabledState::Private
    {
        std::shared_ptr<IRender> render;
        bool clipRectEnabled = false;
    };

    ClipRectEnabledState::ClipRectEnabledState(const std::shared_ptr<IRender>& render) :
        _p(new Private)
    {
        FTK_P();
        p.render = render;
        p.clipRectEnabled = render->getClipRectEnabled();
    }

    ClipRectEnabledState::~ClipRectEnabledState()
    {
        FTK_P();
        p.render->setClipRectEnabled(p.clipRectEnabled);
    }

    struct ClipRectState::Private
    {
        std::shared_ptr<IRender> render;
        Box2I clipRect;
    };

    ClipRectState::ClipRectState(const std::shared_ptr<IRender>& render) :
        _p(new Private)
    {
        FTK_P();
        p.render = render;
        p.clipRect = render->getClipRect();
    }

    ClipRectState::~ClipRectState()
    {
        FTK_P();
        p.render->setClipRect(p.clipRect);
    }

    const Box2I& ClipRectState::getClipRect() const
    {
        return _p->clipRect;
    }

    struct TransformState::Private
    {
        std::shared_ptr<IRender> render;
        M44F transform;
    };

    TransformState::TransformState(const std::shared_ptr<IRender>& render) :
        _p(new Private)
    {
        FTK_P();
        p.render = render;
        p.transform = render->getTransform();
    }

    TransformState::~TransformState()
    {
        FTK_P();
        p.render->setTransform(p.transform);
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
    std::shared_ptr<Image> getScaleContrib(int in, int out, int& taps)
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
}
