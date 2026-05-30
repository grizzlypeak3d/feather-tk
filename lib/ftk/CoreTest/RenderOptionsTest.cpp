// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/RenderOptionsTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/RenderOptions.h>

namespace ftk
{
    namespace core_test
    {
        RenderOptionsTest::RenderOptionsTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::RenderOptionsTest")
        {}

        RenderOptionsTest::~RenderOptionsTest()
        {}

        std::shared_ptr<RenderOptionsTest> RenderOptionsTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<RenderOptionsTest>(new RenderOptionsTest(context));
        }
        
        void RenderOptionsTest::run()
        {
            _enums();
            _operators();
        }
        
        void RenderOptionsTest::_enums()
        {
            FTK_TEST_ENUM(InputVideoLevels);
            FTK_TEST_ENUM(AlphaBlend);
            FTK_TEST_ENUM(ImageFilter);
        }
        
        void RenderOptionsTest::_operators()
        {
            {
                const ImageFilters a;
                ImageFilters b;
                FTK_ASSERT(a == b);
                b.minify = ImageFilter::Nearest;
                FTK_ASSERT(a != b);
            }
            {
                const ImageOptions a;
                ImageOptions b;
                FTK_ASSERT(a == b);
                b.cache = false;
                FTK_ASSERT(a != b);
            }
        }
    }
}

