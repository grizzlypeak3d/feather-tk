// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class RandomTest : public test::ITest
        {
        protected:
            RandomTest(const std::shared_ptr<Context>&);

        public:
            virtual ~RandomTest();

            static std::shared_ptr<RandomTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

