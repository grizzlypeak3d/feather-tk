// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class CommandTest : public test::ITest
        {
        protected:
            CommandTest(const std::shared_ptr<Context>&);

        public:
            virtual ~CommandTest();

            static std::shared_ptr<CommandTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

