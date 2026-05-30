// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class IntSliderTest : public test::ITest
        {
        protected:
            IntSliderTest(const std::shared_ptr<Context>&);

        public:
            virtual ~IntSliderTest();

            static std::shared_ptr<IntSliderTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

