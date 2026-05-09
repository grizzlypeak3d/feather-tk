// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class CmdLineTest : public test::ITest
        {
        protected:
            CmdLineTest(const std::shared_ptr<Context>&);

        public:
            virtual ~CmdLineTest();

            static std::shared_ptr<CmdLineTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

