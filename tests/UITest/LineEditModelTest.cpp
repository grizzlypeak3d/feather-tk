// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/LineEditModelTest.h>

namespace ftk
{
    namespace ui_test
    {
        LineEditModelTest::LineEditModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::LineEditModelTest")
        {}

        LineEditModelTest::~LineEditModelTest()
        {}

        std::shared_ptr<LineEditModelTest> LineEditModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<LineEditModelTest>(new LineEditModelTest(context));
        }
                
        void LineEditModelTest::run()
        {
            if (auto context = _context.lock())
            {
            }
        }
    }
}

