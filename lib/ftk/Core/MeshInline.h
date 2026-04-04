// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    constexpr Vertex2::Vertex2(size_t v, size_t t, size_t c) :
        v(v),
        t(t),
        c(c)
    {}

    constexpr Vertex3::Vertex3(size_t v, size_t t, size_t n, size_t c) :
        v(v),
        t(t),
        n(n),
        c(c)
    {}

    constexpr Triangle2::Triangle2(
        const Vertex2 & v0,
        const Vertex2 & v1,
        const Vertex2 & v2) :
        v({ v0, v1, v2 })
    {}

    constexpr Triangle3::Triangle3(
        const Vertex3& v0,
        const Vertex3& v1,
        const Vertex3& v2) :
        v({ v0, v1, v2 })
    {}

    inline float edge(const V2F& p, const V2F& v0, const V2F& v1)
    {
        return
            (p.x - v0.x) * (v1.y - v0.y) -
            (p.y - v0.y) * (v1.x - v0.x);
    }
}
