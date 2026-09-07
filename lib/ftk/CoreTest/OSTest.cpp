// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/OSTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/OS.h>

#include <ftk/Core/String.h>
#include <ftk/Core/Path.h>

#include <filesystem>

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
            _exePath();
        }
        
        void OSTest::_exePath()
        {
            // Where the running program is, which is what an application
            // looks for the things installed beside it from.
            const std::filesystem::path path = getExePath();
            _print(Format("Executable: {0}").arg(fromFileSystem(path)));
            FTK_CHECK(!path.empty());
            FTK_CHECK(path.is_absolute());
            FTK_CHECK(std::filesystem::exists(path));

            // The test runner, whatever it is called and wherever it was
            // started from -- not the working directory, and not argv[0].
            FTK_CHECK(!path.filename().empty());
            FTK_CHECK(std::filesystem::is_directory(path.parent_path()));
        }

        void OSTest::_env()
        {
            setEnv("FTK_OSTEST_ENV", "ABC");
            std::string s;
            FTK_CHECK(getEnv("FTK_OSTEST_ENV", s));
            FTK_CHECK("ABC" == s);
            setEnv("FTK_OSTEST_ENV", "123");
            int i = 0;
            FTK_CHECK(getEnv("FTK_OSTEST_ENV", i));
            FTK_CHECK(123 == i);
            setEnv("FTK_OSTEST_ENV", join({ "A", "B", "C" }, envListSeparator));
            std::vector<std::string> l;
            FTK_CHECK(getEnv("FTK_OSTEST_ENV", l));
            FTK_CHECK(std::vector<std::string>({ "A", "B", "C" }) == l);
            delEnv("FTK_OSTEST_ENV");
            FTK_CHECK(!getEnv("FTK_OSTEST_ENV", s));
            FTK_CHECK(!getEnv("FTK_OSTEST_ENV", i));
            FTK_CHECK(!getEnv("FTK_OSTEST_ENV", l));
        }
    }
}

