// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/OSTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/OS.h>
#include <ftk/Core/String.h>

namespace ftk
{
    namespace core_test
    {
        OSTest::OSTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::OSTest")
        {}

        OSTest::~OSTest()
        {}

        std::shared_ptr<OSTest> OSTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<OSTest>(new OSTest(context));
        }
        
        void OSTest::run()
        {
            _env();
        }
        
        void OSTest::_env()
        {
            setEnv("FTK_OSTEST_ENV", "ABC");
            std::string s;
            FTK_ASSERT(getEnv("FTK_OSTEST_ENV", s));
            FTK_ASSERT("ABC" == s);
            setEnv("FTK_OSTEST_ENV", "123");
            int i = 0;
            FTK_ASSERT(getEnv("FTK_OSTEST_ENV", i));
            FTK_ASSERT(123 == i);
            setEnv("FTK_OSTEST_ENV", join({ "A", "B", "C" }, envListSeparator));
            std::vector<std::string> l;
            FTK_ASSERT(getEnv("FTK_OSTEST_ENV", l));
            FTK_ASSERT(std::vector<std::string>({ "A", "B", "C" }) == l);
            delEnv("FTK_OSTEST_ENV");
            FTK_ASSERT(!getEnv("FTK_OSTEST_ENV", s));
            FTK_ASSERT(!getEnv("FTK_OSTEST_ENV", i));
            FTK_ASSERT(!getEnv("FTK_OSTEST_ENV", l));
        }
    }
}

