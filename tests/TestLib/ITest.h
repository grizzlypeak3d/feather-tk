// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <memory>
#include <string>

namespace ftk
{
    class Context;

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

            virtual void run() = 0;

        protected:
            void _print(const std::string&);
            void _error(const std::string&);

            std::weak_ptr<Context> _context;
            std::string _name;
        };
    }
}

#define FTK_TEST_ENUM(ENUM) \
    for (auto i : get##ENUM##Enums()) \
    { \
        _print(Format(#ENUM": {0}").arg(getLabel(i))); \
    } \
    { \
        const ENUM v = ENUM::First; \
        const std::string s = to_string(v); \
        ENUM v2 = ENUM::First; \
        FTK_ASSERT(from_string(s, v2)); \
        FTK_ASSERT(v == v2); \
    } \
    { \
        const ENUM v = ENUM::First; \
        std::stringstream ss; \
        ss << v; \
    }

