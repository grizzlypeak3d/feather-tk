// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <filesystem>
#include <memory>
#include <string>

namespace ftk
{
    class Context;
    class TmpDir;

    namespace test
    {
        //! Base class for tests.
        class ITest : public std::enable_shared_from_this<ITest>
        {
            FTK_NON_COPYABLE(ITest);

        protected:
            ITest(
                const std::shared_ptr<Context>&,
                const std::string& name);

        public:
            virtual ~ITest() = 0;

            const std::string& getName() const;

            //! Get the number of failures reported through _fail().
            size_t getFailureCount() const;

            virtual void run() = 0;

        protected:
            //! Get a temporary directory for this test's file output. The
            //! directory is created on first use and removed automatically when
            //! the test is destroyed.
            const std::filesystem::path& _getTempDir();

            void _print(const std::string&);

            //! Report something the test tolerates: an expected exception, or
            //! a capability the environment does not have. This does not fail
            //! the test, which is why _fail() exists alongside it.
            void _error(const std::string&);

            //! Report a genuine failure, with a message saying what was
            //! expected and what happened. Use this where the message can say
            //! something the expression cannot -- the values that did not
            //! match, or which case failed. Where the expression is the whole
            //! story, FTK_CHECK is shorter and names the line for free.
            void _fail(const std::string&);

            //! Report a failed check. Use FTK_CHECK rather than calling this.
            void _checkFailed(
                const char* expression,
                const char* file,
                int line);

            std::shared_ptr<Context> _context;
            std::string _name;

        private:
            std::unique_ptr<TmpDir> _tmpDir;
            size_t _failureCount = 0;
        };
    }
}

//! Check a condition in a test.
//!
//! Unlike FTK_ASSERT this is compiled into every build, and it counts the
//! failure and carries on rather than stopping the run -- one broken check
//! should not hide every test after it. It reports through the test, so a
//! helper that uses it has to be a member of one.
#define FTK_CHECK(value) \
    do { \
        if (!(value)) \
            _checkFailed(#value, __FILE__, __LINE__); \
    } while (0)

#define FTK_TEST_ENUM(ENUM) \
    for (auto i : get##ENUM##Enums()) \
    { \
        _print(ftk::Format(#ENUM": {0}").arg(getLabel(i))); \
    } \
    { \
        const ENUM v = ENUM::First; \
        const std::string s = to_string(v); \
        ENUM v2 = ENUM::First; \
        FTK_CHECK(from_string(s, v2)); \
        FTK_CHECK(v == v2); \
    } \
    { \
        const ENUM v = ENUM::First; \
        std::stringstream ss; \
        ss << v; \
    }

