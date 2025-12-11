// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Box.h>
#include <ftk/Core/Color.h>

#include <array>
#include <cstddef>
#include <vector>

namespace ftk
{
    //! \name Triangle Meshes
    ///@{

    //! Two-dimensional vertex.
    struct FTK_API_TYPE Vertex2
    {
        Vertex2() = default;
        constexpr Vertex2(size_t v, size_t t = 0, size_t c = 0);

        size_t v = 0;
        size_t t = 0;
        size_t c = 0;
    };

    //! Three-dimensional vertex.
    struct FTK_API_TYPE Vertex3
    {
        Vertex3() = default;
        constexpr Vertex3(size_t v, size_t t = 0, size_t n = 0, size_t c = 0);

        size_t v = 0;
        size_t t = 0;
        size_t n = 0;
        size_t c = 0;
    };

    //! Two-dimensional triangle.
    struct FTK_API_TYPE Triangle2
    {
        std::array<Vertex2, 3> v;
    };

    //! Three-dimensional triangle.
    struct FTK_API_TYPE Triangle3
    {
        std::array<Vertex3, 3> v;
    };

    //! Two-dimensional triangle mesh.
    template<typename T>
    struct TriangleMesh2
    {
        std::vector<Vector<2, T> > v;
        std::vector<Vector<4, T> > c;
        std::vector<Vector<2, T> > t;
        std::vector<Triangle2> triangles;
    };

    //! Three-dimensional triangle mesh.
    template<typename T>
    struct TriangleMesh3
    {
        std::vector<Vector<3, T> > v;
        std::vector<Vector<4, T> > c;
        std::vector<Vector<2, T> > t;
        std::vector<Vector<3, T> > n;
        std::vector<Triangle3> triangles;
    };

    typedef TriangleMesh2<float> TriMesh2F;
    typedef TriangleMesh3<float> TriMesh3F;

    //! Edge function.
    FTK_API float edge(const V2F& p, const V2F& v0, const V2F& v1);

    //! Compute the bounding box of a mesh.
    FTK_API Box3F bbox(const TriMesh3F&);

    //! Create a mesh from a box.
    FTK_API TriMesh2F mesh(const Box2I&, bool flipV = false);

    //! Create a mesh from a box.
    FTK_API TriMesh2F mesh(const Box2F&, bool flipV = false);

    //! Create a mesh for drawing checkers.
    FTK_API TriMesh2F checkers(
        const Box2I&,
        const Color4F& color0,
        const Color4F& color1,
        const Size2I& checkerSize);

    //! Create a sphere triangle mesh.
    FTK_API TriMesh3F sphere(
        float radius,
        size_t xResolution,
        size_t yResolution);
        
    ///@}
}

#include <ftk/Core/MeshInline.h>
