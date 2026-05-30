// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class TimeTest : public test::ITest
        {
        protected:
            TimeTest(const std::shared_ptr<Context>&);

        public:
            virtual ~TimeTest();

            static std::shared_ptr<TimeTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        
        private:
            void _sleep();
            void _rational();
        };
    }
}

