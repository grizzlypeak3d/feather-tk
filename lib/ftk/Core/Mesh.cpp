// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Mesh.h>

#include <ftk/Core/Math.h>

namespace ftk
{
    Box3F bbox(const TriMesh3F& mesh)
    {
        Box3F out;
        const size_t size = mesh.v.size();
        if (size > 0)
        {
            out.min = out.max = mesh.v.front();
            for (size_t i = 1; i < size; ++i)
            {
                const V3F& v = mesh.v[i];
                out.min.x = std::min(out.min.x, v.x);
                out.min.y = std::min(out.min.y, v.y);
                out.min.z = std::min(out.min.z, v.z);
                out.max.x = std::max(out.max.x, v.x);
                out.max.y = std::max(out.max.y, v.y);
                out.max.z = std::max(out.max.z, v.z);
            }
        }
        return out;
    }

    TriMesh2F mesh(const Box2I& box, bool mirrorV)
    {
        TriMesh2F out;

        const auto& min = box.min;
        const auto& max = box.max;
        out.v.push_back(V2F(min.x, min.y));
        out.v.push_back(V2F(max.x + 1, min.y));
        out.v.push_back(V2F(max.x + 1, max.y + 1));
        out.v.push_back(V2F(min.x, max.y + 1));
        out.t.push_back(V2F(0.F, mirrorV ? 1.F : 0.F));
        out.t.push_back(V2F(1.F, mirrorV ? 1.F : 0.F));
        out.t.push_back(V2F(1.F, mirrorV ? 0.F : 1.F));
        out.t.push_back(V2F(0.F, mirrorV ? 0.F : 1.F));

        Triangle2 triangle;
        triangle.v[0].v = 1;
        triangle.v[1].v = 3;
        triangle.v[2].v = 2;
        triangle.v[0].t = 1;
        triangle.v[1].t = 3;
        triangle.v[2].t = 2;
        out.triangles.push_back(triangle);
        triangle.v[0].v = 3;
        triangle.v[1].v = 1;
        triangle.v[2].v = 4;
        triangle.v[0].t = 3;
        triangle.v[1].t = 1;
        triangle.v[2].t = 4;
        out.triangles.push_back(triangle);

        return out;
    }

    TriMesh2F mesh(const Box2F& box, bool mirrorV)
    {
        TriMesh2F out;

        const auto& min = box.min;
        const auto& max = box.max;
        out.v.push_back(V2F(min.x, min.y));
        out.v.push_back(V2F(max.x, min.y));
        out.v.push_back(V2F(max.x, max.y));
        out.v.push_back(V2F(min.x, max.y));
        out.t.push_back(V2F(0.F, mirrorV ? 1.F : 0.F));
        out.t.push_back(V2F(1.F, mirrorV ? 1.F : 0.F));
        out.t.push_back(V2F(1.F, mirrorV ? 0.F : 1.F));
        out.t.push_back(V2F(0.F, mirrorV ? 0.F : 1.F));

        Triangle2 triangle;
        triangle.v[0].v = 1;
        triangle.v[1].v = 3;
        triangle.v[2].v = 2;
        triangle.v[0].t = 1;
        triangle.v[1].t = 3;
        triangle.v[2].t = 2;
        out.triangles.push_back(triangle);
        triangle.v[0].v = 3;
        triangle.v[1].v = 1;
        triangle.v[2].v = 4;
        triangle.v[0].t = 3;
        triangle.v[1].t = 1;
        triangle.v[2].t = 4;
        out.triangles.push_back(triangle);

        return out;
    }

    TriMesh2F checkers(
        const Box2I& box,
        const Color4F& color0,
        const Color4F& color1,
        const Size2I& checkerSize)
    {
        TriMesh2F out;

        // X points.
        std::vector<int> xs;
        int x = box.min.x;
        for (; x < box.max.x; x += checkerSize.w)
        {
            xs.push_back(x);
        }
        if (x >= box.max.x)
        {
            xs.push_back(box.max.x);
        }

        // Y points.
        std::vector<int> ys;
        int y = box.min.y;
        for (; y < box.max.y; y += checkerSize.h)
        {
            ys.push_back(y);
        }
        if (y >= box.max.y)
        {
            ys.push_back(box.max.y);
        }

        if (!xs.empty() && !ys.empty())
        {
            // 2D points.
            for (int j = 0; j < ys.size(); ++j)
            {
                for (int i = 0; i < xs.size(); ++i)
                {
                    out.v.push_back(V2F(xs[i], ys[j]));
                }
            }

            // Colors.
            out.c.push_back(V4F(color0.r, color0.g, color0.b, color0.a));
            out.c.push_back(V4F(color1.r, color1.g, color1.b, color1.a));

            // Triangles.
            for (int j = 0; j < ys.size() - 1; ++j)
            {
                for (int i = 0; i < xs.size() - 1; ++i)
                {
                    const int v0 = j * xs.size() + i + 1;
                    const int v1 = j * xs.size() + (i + 1) + 1;
                    const int v2 = (j + 1) * xs.size() + (i + 1) + 1;
                    const int v3 = (j + 1) * xs.size() + i + 1;
                    const int c = (j + i) % 2 + 1;
                    out.triangles.push_back({
                        Vertex2(v0, 0, c),
                        Vertex2(v2, 0, c),
                        Vertex2(v1, 0, c) });
                    out.triangles.push_back({
                        Vertex2(v2, 0, c),
                        Vertex2(v0, 0, c),
                        Vertex2(v3, 0, c) });
                }
            }
        }

        return out;
    }

    TriMesh3F sphere(
        float radius,
        size_t xResolution,
        size_t yResolution)
    {
        TriMesh3F out;

        //! \bug Use only a single vertex at each pole.
        for (size_t v = 0; v <= yResolution; ++v)
        {
            const float v1 = static_cast<float>(v) / static_cast<float>(yResolution);

            for (size_t u = 0; u <= xResolution; ++u)
            {
                const float u1 = static_cast<float>(u) / static_cast<float>(xResolution);
                const float x = radius * sinf(v1 * pi) * cosf(u1 * pi2);
                const float y = radius * cosf(v1 * pi);
                const float z = radius * sinf(v1 * pi) * sinf(u1 * pi2);
                out.v.push_back(V3F(x, y, z));
                out.t.push_back(V2F(u1, 1.F - v1));
            }
        }

        Triangle3 triangle;
        for (size_t v = 0; v < yResolution; ++v)
        {
            for (size_t u = 0; u < xResolution; ++u)
            {
                const size_t i = u + v * (xResolution + 1);
                const size_t j = u + (v + 1) * (xResolution + 1);
                triangle.v[0].v = triangle.v[0].t = j + 1 + 1;
                triangle.v[1].v = triangle.v[1].t = j + 1;
                triangle.v[2].v = triangle.v[2].t = i + 1;
                out.triangles.push_back(triangle);
                triangle.v[0].v = triangle.v[0].t = i + 1;
                triangle.v[1].v = triangle.v[1].t = i + 1 + 1;
                triangle.v[2].v = triangle.v[2].t = j + 1 + 1;
                out.triangles.push_back(triangle);
            }
        }

        return out;
    }
}
