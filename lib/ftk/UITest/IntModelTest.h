// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class IntModelTest : public test::ITest
        {
        protected:
            IntModelTest(const std::shared_ptr<Context>&);

        public:
            virtual ~IntModelTest();

            static std::shared_ptr<IntModelTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

