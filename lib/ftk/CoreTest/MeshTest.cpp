// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/MeshTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Mesh.h>

namespace ftk
{
    namespace core_test
    {
        MeshTest::MeshTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::MeshTest")
        {}

        MeshTest::~MeshTest()
        {}

        std::shared_ptr<MeshTest> MeshTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<MeshTest>(new MeshTest(context));
        }
        
        void MeshTest::run()
        {
            _members();
            _checkers();
        }

        void MeshTest::_checkers()
        {
            // Two by two checkers of ten pixels: four quads.
            const TriMesh2F mesh = checkers(
                Box2I(0, 0, 20, 20),
                Color4F(0.F, 0.F, 0.F),
                Color4F(1.F, 1.F, 1.F),
                Size2I(10, 10));
            FTK_ASSERT(8 == mesh.triangles.size());

            // A size of zero, or less, is a pixel rather than a hang.
            for (const auto& size : { Size2I(0, 0), Size2I(-5, 10), Size2I(10, 0) })
            {
                const TriMesh2F m = checkers(
                    Box2I(0, 0, 4, 4),
                    Color4F(0.F, 0.F, 0.F),
                    Color4F(1.F, 1.F, 1.F),
                    size);
                FTK_ASSERT(!m.triangles.empty());
            }
        }
        
        void MeshTest::_members()
        {
            {
                const Vertex2 v;
                FTK_CHECK(0 == v.v);
                FTK_CHECK(0 == v.t);
                FTK_CHECK(0 == v.c);
            }
            {
                const Vertex2 v(1, 2, 3);
                FTK_CHECK(1 == v.v);
                FTK_CHECK(2 == v.t);
                FTK_CHECK(3 == v.c);
            }
            {
                const Vertex3 v;
                FTK_CHECK(0 == v.v);
                FTK_CHECK(0 == v.t);
                FTK_CHECK(0 == v.n);
                FTK_CHECK(0 == v.c);
            }
            {
                const Vertex3 v(1, 2, 3, 4);
                FTK_CHECK(1 == v.v);
                FTK_CHECK(2 == v.t);
                FTK_CHECK(3 == v.n);
                FTK_CHECK(4 == v.c);
            }
        }
    }
}

