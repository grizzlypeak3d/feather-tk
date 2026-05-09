// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/TestLib/ITest.h>

namespace ftk
{
    namespace ui_test
    {
        class DoubleEditSliderTest : public test::ITest
        {
        protected:
            DoubleEditSliderTest(const std::shared_ptr<Context>&);

        public:
            virtual ~DoubleEditSliderTest();

            static std::shared_ptr<DoubleEditSliderTest> create(
                const std::shared_ptr<Context>&);

            void run() override;
        };
    }
}

