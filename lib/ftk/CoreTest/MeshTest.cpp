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

