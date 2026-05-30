// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class FloatModelTest : public test::ITest
        {
        protected:
            FloatModelTest(const std::shared_ptr<Context>&);

        public:
            virtual ~FloatModelTest();

            static std::shared_ptr<FloatModelTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

