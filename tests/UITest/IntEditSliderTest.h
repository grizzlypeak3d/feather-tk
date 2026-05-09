// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class IntEditSliderTest : public test::ITest
        {
        protected:
            IntEditSliderTest(const std::shared_ptr<Context>&);

        public:
            virtual ~IntEditSliderTest();

            static std::shared_ptr<IntEditSliderTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

