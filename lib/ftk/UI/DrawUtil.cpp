// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DrawUtil.h>

#include <array>

#include <ftk/Core/Math.h>

#include <algorithm>
#include <cmath>

namespace ftk
{
    namespace
    {
        // Default circle resolution.
        constexpr size_t circleResolution = 16;

        // The sin and cos values for the default circle resolution, in whole
        // degrees. The angles asked for are the circle's (every 24 degrees)
        // and the rounded corners' (every 6 degrees), which are all whole
        // degrees, so the nearest sample is the exact value.
        struct CircleSample { float c, s; };
        constexpr size_t circleTableSize = 361;
        using CircleTable = std::array<CircleSample, circleTableSize>;

        CircleSample getCircleSample(float v)
        {
            static const CircleTable table = []
            {
                CircleTable t;
                for (size_t k = 0; k < circleTableSize; ++k)
                {
                    const float a = deg2rad(static_cast<float>(k));
                    t[k] = { cosf(a), sinf(a) };
                }
                return t;
            }();
            const int i = clamp(
                static_cast<int>(std::round(rad2deg(v))),
                0,
                static_cast<int>(circleTableSize - 1));
            return table[i];
        }

        // Return the cos using our table.
        float cosfTable(float v)
        {
            return getCircleSample(v).c;
        }

        // Return the sin using our table.
        float sinfTable(float v)
        {
            return getCircleSample(v).s;
        }
    }

    TriMesh2F rect(
        const Box2I& box,
        int cornerRadius,
        size_t resolution)
    {
        return rect(
            box,
            { cornerRadius, cornerRadius, cornerRadius, cornerRadius },
            resolution);
    }

    TriMesh2F rect(
        const Box2I& box,
        const std::array<int, 4>& cornerRadii,
        size_t resolution)
    {
        const auto& sinf = circleResolution == resolution ? &sinfTable : &::sinf;
        const auto& cosf = circleResolution == resolution ? &cosfTable : &::cosf;

        TriMesh2F out;

        const float x = box.x();
        const float y = box.y();
        const float w = box.w();
        const float h = box.h();

        if (0 == cornerRadii[0] &&
            0 == cornerRadii[1] &&
            0 == cornerRadii[2] &&
            0 == cornerRadii[3])
        {
            out.v.reserve(4);
            out.v.emplace_back(x, y);
            out.v.emplace_back(x + w, y);
            out.v.emplace_back(x + w, y + h);
            out.v.emplace_back(x, y + h);

            out.triangles.reserve(2);
            out.triangles.emplace_back(1, 3, 2);
            out.triangles.emplace_back(3, 1, 4);
        }
        else
        {
            // The corners are built in the order bottom right, bottom left,
            // top left, top right, each a fan around a center inset by its
            // own radius. A zero radius puts the center on the corner and
            // the fan collapses to it, so the topology is the same for any
            // mix of round and square corners.
            const std::array<float, 4> r =
            {
                static_cast<float>(cornerRadii[2]),
                static_cast<float>(cornerRadii[3]),
                static_cast<float>(cornerRadii[0]),
                static_cast<float>(cornerRadii[1])
            };
            const std::vector<V2F> c =
            {
                V2F(x + w - r[0], y + h - r[0]),
                V2F(x + r[1], y + h - r[1]),
                V2F(x + r[2], y + r[2]),
                V2F(x + w - r[3], y + r[3])
            };
            size_t i = 0;
            for (size_t j = 0; j < 4; ++j)
            {
                out.v.emplace_back(c[j]);
                for (size_t k = 0; k < resolution; ++k)
                {
                    const float v = k / static_cast<float>(resolution - 1);
                    const float a = lerp(v, j * 90.F, j * 90.F + 90.F);
                    const float cos = cosf(deg2rad(a));
                    const float sin = sinf(deg2rad(a));
                    out.v.emplace_back(
                        c[j].x + cos * r[j],
                        c[j].y + sin * r[j]);
                }
                for (size_t k = 0; k < resolution - 1; ++k)
                {
                    out.triangles.emplace_back(i + 1, i + k + 3, i + k + 2);
                }
                i += 1 + resolution;
            }

            i = 0;
            size_t j = resolution;
            out.triangles.emplace_back(i + 1, j + 2, j + 1);
            out.triangles.emplace_back(j + 1, j + 2, j + 3);

            i += 1 + resolution;
            j += 1 + resolution;
            out.triangles.emplace_back(i + 1, j + 2, j + 1);
            out.triangles.emplace_back(j + 1, j + 2, j + 3);

            i += 1 + resolution;
            j += 1 + resolution;
            out.triangles.emplace_back(i + 1, j + 2, j + 1);
            out.triangles.emplace_back(j + 1, j + 2, j + 3);

            i += 1 + resolution;
            j += 1 + resolution;
            out.triangles.emplace_back(i + 1, 2, j + 1);
            out.triangles.emplace_back(2, i + 1, 1);

            i = 0;
            j = 1 + resolution;
            size_t k = (1 + resolution) * 2;
            out.triangles.emplace_back(i + 1, k + 1, j + 1);
            i = k;
            j = k + 1 + resolution;
            k = 0;
            out.triangles.emplace_back(i + 1, k + 1, j + 1);
        }

        return out;
    }

    TriMesh2F circle(
        const V2I& pos,
        int radius,
        size_t resolution)
    {
        const auto& sinf = circleResolution == resolution ? &sinfTable : &::sinf;
        const auto& cosf = circleResolution == resolution ? &cosfTable : &::cosf;

        TriMesh2F out;

        const int inc = 360 / resolution;
        for (int i = 0; i < 360; i += inc)
        {
            const size_t size = out.v.size();
            out.v.emplace_back(static_cast<float>(pos.x), static_cast<float>(pos.y));
            out.v.emplace_back(
                pos.x + cosf(deg2rad(i)) * radius,
                pos.y + sinf(deg2rad(i)) * radius);
            const int d = std::min(i + inc, 360);
            out.v.emplace_back(
                pos.x + cosf(deg2rad(d)) * radius,
                pos.y + sinf(deg2rad(d)) * radius);
            out.triangles.emplace_back(size + 1, size + 3, size + 2);
        }

        return out;
    }

    TriMesh2F border(
        const Box2I& box,
        int width,
        int radius,
        size_t resolution)
    {
        return border(box, width, { radius, radius, radius, radius }, resolution);
    }

    TriMesh2F border(
        const Box2I& box,
        int width,
        const std::array<int, 4>& radii,
        size_t resolution)
    {
        const auto& sinf = circleResolution == resolution ? &sinfTable : &::sinf;
        const auto& cosf = circleResolution == resolution ? &cosfTable : &::cosf;

        TriMesh2F out;

        const float x = box.x();
        const float y = box.y();
        const float w = box.w();
        const float h = box.h();

        if (0 == radii[0] && 0 == radii[1] && 0 == radii[2] && 0 == radii[3])
        {
            out.v.reserve(8);
            out.v.emplace_back(x, y);
            out.v.emplace_back(x + w, y);
            out.v.emplace_back(x + w, y + h);
            out.v.emplace_back(x, y + h);
            out.v.emplace_back(x + width, y + width);
            out.v.emplace_back(x + w - width, y + width);
            out.v.emplace_back(x + w - width, y + h - width);
            out.v.emplace_back(x + width, y + h - width);

            out.triangles.reserve(8);
            out.triangles.emplace_back(1, 5, 2);
            out.triangles.emplace_back(2, 5, 6);
            out.triangles.emplace_back(2, 6, 3);
            out.triangles.emplace_back(3, 6, 7);
            out.triangles.emplace_back(3, 7, 4);
            out.triangles.emplace_back(4, 7, 8);
            out.triangles.emplace_back(4, 8, 1);
            out.triangles.emplace_back(1, 8, 5);
        }
        else
        {
            // The corners in the order bottom right, bottom left, top left,
            // top right, each a pair of fans -- the outside and the inside
            // edge -- around a center inset by its own radius. A square
            // corner keeps the topology: both fans collapse to a point, the
            // corner itself outside and the corner inset by the width inside.
            const std::array<float, 4> r =
            {
                static_cast<float>(radii[2]),
                static_cast<float>(radii[3]),
                static_cast<float>(radii[0]),
                static_cast<float>(radii[1])
            };
            const std::vector<V2F> c =
            {
                V2F(x + w - r[0], y + h - r[0]),
                V2F(x + r[1], y + h - r[1]),
                V2F(x + r[2], y + r[2]),
                V2F(x + w - r[3], y + r[3])
            };
            const std::array<V2F, 4> inward =
            {
                V2F(-1.F, -1.F),
                V2F(1.F, -1.F),
                V2F(1.F, 1.F),
                V2F(-1.F, 1.F)
            };
            size_t i = 0;
            for (size_t j = 0; j < 4; ++j)
            {
                for (size_t k = 0; k < resolution; ++k)
                {
                    if (r[j] > 0.F)
                    {
                        const float v = k / static_cast<float>(resolution - 1);
                        const float a = lerp(v, j * 90.F, j * 90.F + 90.F);
                        const float cos = cosf(deg2rad(a));
                        const float sin = sinf(deg2rad(a));
                        out.v.emplace_back(
                            c[j].x + cos * r[j],
                            c[j].y + sin * r[j]);
                        out.v.emplace_back(
                            c[j].x + cos * (r[j] - width),
                            c[j].y + sin * (r[j] - width));
                    }
                    else
                    {
                        out.v.emplace_back(c[j].x, c[j].y);
                        out.v.emplace_back(
                            c[j].x + inward[j].x * width,
                            c[j].y + inward[j].y * width);
                    }
                }
                for (size_t k = 0; k < resolution - 1; ++k)
                {
                    out.triangles.emplace_back(i + 1, i + 2, i + 3);
                    out.triangles.emplace_back(i + 3, i + 2, i + 4);
                    i += 2;
                }
                i += 2;
            }

            i = resolution * 2 - 2;
            out.triangles.emplace_back(i + 1, i + 2, i + 3);
            out.triangles.emplace_back(i + 3, i + 2, i + 4);

            i = resolution * 4 - 2;
            out.triangles.emplace_back(i + 1, i + 2, i + 3);
            out.triangles.emplace_back(i + 3, i + 2, i + 4);

            i = resolution * 6 - 2;
            out.triangles.emplace_back(i + 1, i + 2, i + 3);
            out.triangles.emplace_back(i + 3, i + 2, i + 4);

            i = resolution * 8 - 2;
            out.triangles.emplace_back(i + 1, i + 2, 1);
            out.triangles.emplace_back(1, i + 2, 2);
        }

        return out;
    }

    Color4F checkedTint(const Color4F& checked)
    {
        Color4F out = checked;
        out.a *= .3F;
        return out;
    }

    Color4F checkedHighlight(const Color4F& checked)
    {
        // Away from what it sits on, keeping the hue: a dark accent, the
        // dark style's amber, is brightened; a light one, the light style's
        // blue, is deepened, since brightening it only washes it out.
        const float luminance =
            .2126F * checked.r +
            .7152F * checked.g +
            .0722F * checked.b;
        if (luminance < .5F)
        {
            return Color4F(
                std::min(checked.r * 1.6F, 1.F),
                std::min(checked.g * 1.6F, 1.F),
                std::min(checked.b * 1.6F, 1.F),
                checked.a);
        }
        float rgb[3] = { checked.r, checked.g, checked.b };
        float hsv[3] = { 0.F, 0.F, 0.F };
        rgbToHSV(rgb, hsv);
        hsv[1] = std::min(hsv[1] * 2.F, 1.F);
        hsv[2] *= .75F;
        hsvToRGB(hsv, rgb);
        return Color4F(rgb[0], rgb[1], rgb[2], checked.a);
    }

    TriMesh2F shadow(
        const Box2I& box,
        int cornerRadius,
        const float alpha,
        size_t resolution)
    {
        const auto& sinf = circleResolution == resolution ? &sinfTable : &::sinf;
        const auto& cosf = circleResolution == resolution ? &cosfTable : &::cosf;

        TriMesh2F out;

        const int x = box.x();
        const int y = box.y();
        const int w = box.w();
        const int h = box.h();
        const int r = cornerRadius;

        out.c.emplace_back(0.F, 0.F, 0.F, alpha);
        out.c.emplace_back(0.F, 0.F, 0.F, 0.F);

        const std::vector<V2F> c =
        {
            V2F(x + w - r, y + h - r),
            V2F(x + r, y + h - r),
            V2F(x + r, y + r),
            V2F(x + w - r, y + r)
        };
        size_t i = 0;
        for (size_t j = 0; j < 4; ++j)
        {
            out.v.emplace_back(c[j]);
            for (size_t k = 0; k < resolution; ++k)
            {
                const float v = k / static_cast<float>(resolution - 1);
                const float a = lerp(v, j * 90.F, j * 90.F + 90.F);
                const float cos = cosf(deg2rad(a));
                const float sin = sinf(deg2rad(a));
                out.v.emplace_back(
                    c[j].x + cos * r,
                    c[j].y + sin * r);
            }
            for (size_t k = 0; k < resolution - 1; ++k)
            {
                out.triangles.emplace_back(
                    Vertex2(i + 1, 0, 1),
                    Vertex2(i + k + 3, 0, 2),
                    Vertex2(i + k + 2, 0, 2));
            }
            i += 1 + resolution;
        }

        i = 0;
        size_t j = resolution;
        out.triangles.emplace_back(
            Vertex2(i + 1, 0, 1),
            Vertex2(j + 2, 0, 1),
            Vertex2(j + 1, 0, 2));
        out.triangles.emplace_back(
            Vertex2(j + 1, 0, 2),
            Vertex2(j + 2, 0, 1),
            Vertex2(j + 3, 0, 2));

        i += 1 + resolution;
        j += 1 + resolution;
        out.triangles.emplace_back(
            Vertex2(i + 1, 0, 1),
            Vertex2(j + 2, 0, 1),
            Vertex2(j + 1, 0, 2));
        out.triangles.emplace_back(
            Vertex2(j + 1, 0, 2),
            Vertex2(j + 2, 0, 1),
            Vertex2(j + 3, 0, 2));

        i += 1 + resolution;
        j += 1 + resolution;
        out.triangles.emplace_back(
            Vertex2(i + 1, 0, 1),
            Vertex2(j + 2, 0, 1),
            Vertex2(j + 1, 0, 2));
        out.triangles.emplace_back(
            Vertex2(j + 1, 0, 2),
            Vertex2(j + 2, 0, 1),
            Vertex2(j + 3, 0, 2));

        i += 1 + resolution;
        j += 1 + resolution;
        out.triangles.emplace_back(
            Vertex2(i + 1, 0, 1),
            Vertex2(2, 0, 2),
            Vertex2(j + 1, 0, 2));
        out.triangles.emplace_back(
            Vertex2(2, 0, 2),
            Vertex2(i + 1, 0, 1),
            Vertex2(1, 0, 1));

        i = 0;
        j = 1 + resolution;
        size_t k = (1 + resolution) * 2;
        out.triangles.emplace_back(
            Vertex2(i + 1, 0, 1),
            Vertex2(k + 1, 0, 1),
            Vertex2(j + 1, 0, 1));
        i = k;
        j = k + 1 + resolution;
        k = 0;
        out.triangles.emplace_back(
            Vertex2(i + 1, 0, 1),
            Vertex2(k + 1, 0, 1),
            Vertex2(j + 1, 0, 1));

        return out;
    }
}
