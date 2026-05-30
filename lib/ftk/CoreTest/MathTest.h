// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class MathTest : public test::ITest
        {
        protected:
            MathTest(const std::shared_ptr<Context>&);

        public:
            virtual ~MathTest();

            static std::shared_ptr<MathTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

