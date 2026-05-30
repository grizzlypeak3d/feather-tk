// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/TimeTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Time.h>

namespace ftk
{
    namespace core_test
    {
        TimeTest::TimeTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::TimeTest")
        {}

        TimeTest::~TimeTest()
        {}

        std::shared_ptr<TimeTest> TimeTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TimeTest>(new TimeTest(context));
        }
        
        void TimeTest::run()
        {
            _sleep();
            _rational();
        }
        
        void TimeTest::_sleep()
        {
            const auto t0 = std::chrono::steady_clock::now();
            sleep(std::chrono::seconds(1));
            const auto t1 = std::chrono::steady_clock::now();
            sleep(std::chrono::seconds(2), t0, t1);
        }

        void TimeTest::_rational()
        {
            FTK_ASSERT(toRational(24.0) == std::make_pair(24, 1));
            FTK_ASSERT(toRational(24.5) == std::make_pair(24, 1));
            FTK_ASSERT(toRational(30.0) == std::make_pair(30, 1));
        }
    }
}

