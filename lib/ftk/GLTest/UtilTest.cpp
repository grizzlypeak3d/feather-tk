// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GLTest/UtilTest.h>

#include <ftk/GL/Util.h>

#include <ftk/Core/Assert.h>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        UtilTest::UtilTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::UtilTest")
        {}

        UtilTest::~UtilTest()
        {}

        std::shared_ptr<UtilTest> UtilTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<UtilTest>(new UtilTest(context));
        }
                
        void UtilTest::run()
        {
            FTK_ASSERT(4 == getMajorVersion("4.1.0 Driver 571.59"));
            FTK_ASSERT(3 == getMajorVersion("OpenGL ES 3.2 Mesa 25.0.7"));
        }
    }
}

