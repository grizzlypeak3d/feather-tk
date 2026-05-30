// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class ErrorTest : public test::ITest
        {
        protected:
            ErrorTest(const std::shared_ptr<Context>&);

        public:
            virtual ~ErrorTest();

            static std::shared_ptr<ErrorTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

