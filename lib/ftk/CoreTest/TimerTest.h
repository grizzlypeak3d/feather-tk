// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class TimerTest : public test::ITest
        {
        protected:
            TimerTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TimerTest();

            static std::shared_ptr<TimerTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

