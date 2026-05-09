// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class DoubleModelTest : public test::ITest
        {
        protected:
            DoubleModelTest(const std::shared_ptr<Context>&);

        public:
            virtual ~DoubleModelTest();

            static std::shared_ptr<DoubleModelTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

