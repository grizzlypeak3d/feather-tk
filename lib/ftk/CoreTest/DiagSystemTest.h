// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace core_test
    {
        class DiagSystemTest : public test::ITest
        {
        protected:
            DiagSystemTest(const std::shared_ptr<Context>&);

        public:
            virtual ~DiagSystemTest();

            static std::shared_ptr<DiagSystemTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}
