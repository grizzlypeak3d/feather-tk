// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class NoiseTest : public test::ITest
        {
        protected:
            NoiseTest(const std::shared_ptr<Context>&);

        public:
            virtual ~NoiseTest();

            static std::shared_ptr<NoiseTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

