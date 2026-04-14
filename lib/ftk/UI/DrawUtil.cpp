// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DrawUtil.h>

#include <ftk/Core/Math.h>

#include <cmath>

namespace ftk
{
    namespace
    {
        // Default circle resolution.
        constexpr size_t circleResolution = 16;

        // Compute the sin and cos values for the default circle resolution.
        struct CircleSample { float c, s; };
        using CircleTable = std::array<CircleSample, circleResolution>;

        CircleSample getCircleSample(float v)
        {
            static const CircleTable table = []
            {
                CircleTable t;
                for (size_t k = 0; k < circleResolution; ++k)
                {
                    const float a = k / static_cast<float>(circleResolution - 1) * pi2;
                    t[k] = { cosf(a), sinf(a) };
                }
                return t;
            }();
            const int i = clamp(
                static_cast<int>(v / pi2 * (circleResolution - 1)),
                0,
                static_cast<int>(circleResolution - 1));
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
        const auto& sinf = circleResolution == resolution ? &sinfTable : &::sinf;
        const auto& cosf = circleResolution == resolution ? &cosfTable : &::cosf;

        TriMesh2F out;

        const float x = box.x();
        const float y = box.y();
        const float w = box.w();
        const float h = box.h();

        if (0 == cornerRadius)
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
            const int r = cornerRadius;

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
        const auto& sinf = circleResolution == resolution ? &sinfTable : &::sinf;
        const auto& cosf = circleResolution == resolution ? &cosfTable : &::cosf;

        TriMesh2F out;

        const float x = box.x();
        const float y = box.y();
        const float w = box.w();
        const float h = box.h();

        if (0 == radius)
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
            const int r = radius;

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
                for (size_t k = 0; k < resolution; ++k)
                {
                    const float v = k / static_cast<float>(resolution - 1);
                    const float a = lerp(v, j * 90.F, j * 90.F + 90.F);
                    const float cos = cosf(deg2rad(a));
                    const float sin = sinf(deg2rad(a));
                    out.v.emplace_back(
                        c[j].x + cos * r,
                        c[j].y + sin * r);
                    out.v.emplace_back(
                        c[j].x + cos * (r - width),
                        c[j].y + sin * (r - width));
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
